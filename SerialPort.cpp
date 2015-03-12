//
//  SerialPort.cpp
//  RoboServerA
//
//  Created by h on 09/03/2015.
//
//

#include "SerialPort.hpp"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <paths.h>
#include <termios.h>
#include <sysexits.h>
#include <sys/param.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/serial/IOSerialKeys.h>
#include <IOKit/serial/ioss.h>
#include <IOKit/IOBSD.h>


SerialPort::SerialPort(char * portName, long baudRate) {
    _myPortName = portName;
    _myBaudRate = baudRate;

}

SerialPort::~SerialPort() {
    closePort();
}

// Given the path to a serial device, open the device and configure it.
// Return the file descriptor associated with the device.
bool SerialPort::openPort()
{
    int				fileDescriptor = -1;
    int				handshake;
    struct termios	options;
    unsigned long mics = 1UL;
    
    
    // Open the serial port read/write, with no controlling terminal, and don't wait for a connection.
    // The O_NONBLOCK flag also causes subsequent I/O on the device to be non-blocking.
    // See open(2) <x-man-page://2/open> for details.
    
    _fileDescriptor = open(_myPortName, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (_fileDescriptor == -1) {
        printf("Error opening serial port %s - %s(%d).\n",
               _myPortName, strerror(errno), errno);
        goto error;
    }
    
    // Note that open() follows POSIX semantics: multiple open() calls to the same file will succeed
    // unless the TIOCEXCL ioctl is issued. This will prevent additional opens except by root-owned
    // processes.
    // See tty(4) <x-man-page//4/tty> and ioctl(2) <x-man-page//2/ioctl> for details.
    
    if (ioctl(_fileDescriptor, TIOCEXCL) == -1) {
        printf("Error setting TIOCEXCL on %s - %s(%d).\n",
               _myPortName, strerror(errno), errno);
        goto error;
    }
    
    // Now that the device is open, clear the O_NONBLOCK flag so subsequent I/O will block.
    // See fcntl(2) <x-man-page//2/fcntl> for details.
    
    if (fcntl(_fileDescriptor, F_SETFL, 0) == -1) {
        printf("Error clearing O_NONBLOCK %s - %s(%d).\n",
               _myPortName, strerror(errno), errno);
        goto error;
    }
    
    // Get the current options and save them so we can restore the default settings later.
    if (tcgetattr(_fileDescriptor, &gOriginalTTYAttrs) == -1) {
        printf("Error getting tty attributes %s - %s(%d).\n",
               _myPortName, strerror(errno), errno);
        goto error;
    }
    
    // The serial port attributes such as timeouts and baud rate are set by modifying the termios
    // structure and then calling tcsetattr() to cause the changes to take effect. Note that the
    // changes will not become effective without the tcsetattr() call.
    // See tcsetattr(4) <x-man-page://4/tcsetattr> for details.
    
    options = gOriginalTTYAttrs;
    
    // Print the current input and output baud rates.
    // See tcsetattr(4) <x-man-page://4/tcsetattr> for details.
    
    printf("Current input baud rate is %d\n", (int) cfgetispeed(&options));
    printf("Current output baud rate is %d\n", (int) cfgetospeed(&options));
    
    // Set raw input (non-canonical) mode, with reads blocking until either a single character
    // has been received or a one second timeout expires.
    // See tcsetattr(4) <x-man-page://4/tcsetattr> and termios(4) <x-man-page://4/termios> for details.
    
    cfmakeraw(&options);
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 0;
    
    // The baud rate, word length, and handshake options can be set as follows:
    
    cfsetspeed(&options, _myBaudRate);		// Set 19200 baud
    /* options.c_cflag |= (CS8 	   | 	// Use 7 bit words
     PARENB	   | 	// Parity enable (even parity if PARODD not also set)
     CCTS_OFLOW | 	// CTS flow control of output
     CRTS_IFLOW);	// RTS flow control of input
     */
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cflag &= ~CCTS_OFLOW;
    options.c_cflag &= ~CRTS_IFLOW;
    options.c_cflag &= ~CDTR_IFLOW;
    options.c_cflag &= ~CDSR_OFLOW;
    options.c_cflag &= ~CCAR_OFLOW;
    options.c_cflag |= CLOCAL;
    
    
    
    // The IOSSIOSPEED ioctl can be used to set arbitrary baud rates
    // other than those specified by POSIX. The driver for the underlying serial hardware
    // ultimately determines which baud rates can be used. This ioctl sets both the input
    // and output speed.
    
    /*speed_t speed = 14400; // Set 14400 baud
    if (ioctl(_fileDescriptor, IOSSIOSPEED, &speed) == -1) {
        printf("Error calling ioctl(..., IOSSIOSPEED, ...) %s - %s(%d).\n",
               _myPortName, strerror(errno), errno);
    }*/
    
    // Print the new input and output baud rates. Note that the IOSSIOSPEED ioctl interacts with the serial driver
    // directly bypassing the termios struct. This means that the following two calls will not be able to read
    // the current baud rate if the IOSSIOSPEED ioctl was used but will instead return the speed set by the last call
    // to cfsetspeed.
    
    printf("Input baud rate changed to %d\n", (int) cfgetispeed(&options));
    printf("Output baud rate changed to %d\n", (int) cfgetospeed(&options));
    
    // Cause the new options to take effect immediately.
    if (tcsetattr(_fileDescriptor, TCSANOW, &options) == -1) {
        printf("Error setting tty attributes %s - %s(%d).\n",
               _myPortName, strerror(errno), errno);
        goto error;
    }
    
    // To set the modem handshake lines, use the following ioctls.
    // See tty(4) <x-man-page//4/tty> and ioctl(2) <x-man-page//2/ioctl> for details.
    
    // Assert Data Terminal Ready (DTR)
   // if (ioctl(_fileDescriptor, TIOCSDTR) == -1) {
      //  printf("Error asserting DTR %s - %s(%d).\n",
   //            _myPortName, strerror(errno), errno);
   // }
    
    // Clear Data Terminal Ready (DTR)
 /*   if (ioctl(_fileDescriptor, TIOCCDTR) == -1) {
        printf("Error clearing DTR %s - %s(%d).\n",
               _myPortName, strerror(errno), errno);
    }
    
    // Set the modem lines depending on the bits set in handshake
    handshake = TIOCM_DTR | TIOCM_RTS | TIOCM_CTS | TIOCM_DSR;
    if (ioctl(_fileDescriptor, TIOCMSET, &handshake) == -1) {
        printf("Error setting handshake lines %s - %s(%d).\n",
               _myPortName, strerror(errno), errno);
    }*/
    
    // To read the state of the modem lines, use the following ioctl.
    // See tty(4) <x-man-page//4/tty> and ioctl(2) <x-man-page//2/ioctl> for details.
    
    // Store the state of the modem lines in handshake
    if (ioctl(_fileDescriptor, TIOCMGET, &handshake) == -1) {
        printf("Error getting handshake lines %s - %s(%d).\n",
               _myPortName, strerror(errno), errno);
    }
    
    printf("Handshake lines currently set to %d\n", handshake);
    
    
    // Set the receive latency in microseconds. Serial drivers use this value to determine how often to
    // dequeue characters received by the hardware. Most applications don't need to set this value: if an
    // app reads lines of characters, the app can't do anything until the line termination character has been
    // received anyway. The most common applications which are sensitive to read latency are MIDI and IrDA
    // applications.
    
    if (ioctl(_fileDescriptor, IOSSDATALAT, &mics) == -1) {
        // set latency to 1 microsecond
        printf("Error setting read latency %s - %s(%d).\n",
               _myPortName, strerror(errno), errno);
        goto error;
    }
    
    // Success
    return fileDescriptor;
    
    // Failure path
error:
    if (fileDescriptor != -1) {
        close(fileDescriptor);
    }
    
    return -1;
}

void SerialPort::closePort()
{
    // Block until all written output has been sent from the device.
    // Note that this call is simply passed on to the serial device driver.
    // See tcsendbreak(3) <x-man-page://3/tcsendbreak> for details.
    if (tcdrain(_fileDescriptor) == -1) {
        printf("Error waiting for drain - %s(%d).\n",
               strerror(errno), errno);
    }
    
    // Traditionally it is good practice to reset a serial port back to
    // the state in which you found it. This is why the original termios struct
    // was saved.
    if (tcsetattr(_fileDescriptor, TCSANOW, &gOriginalTTYAttrs) == -1) {
        printf("Error resetting tty attributes - %s(%d).\n",
               strerror(errno), errno);
    }
    
    close(_fileDescriptor);
    printf("PortClosed\n");
}

bool SerialPort::writeToPort(char * writeBuffer, int dataLength) {
    //
    
    //flushInput();
    int numBytesWritten = (int) write(_fileDescriptor, writeBuffer, dataLength);
    
    if (numBytesWritten == -1) {
        printf("Error writing to port - %s(%d).\n", strerror(errno), errno);
        return false;
    }
    else {
        //printf("Bytes written to port: %d\n",numBytesWritten);
        return true;
    }

}

void SerialPort::writeUntilNull(char * writeBuffer) {
    //printf("Writing to LCD\n");
    for (int i = 0; writeBuffer[i] != '\0'; i++){
        writeChar(writeBuffer[i]);
    }
}

void SerialPort::writeChar(char theChar) {
    write(_fileDescriptor, &theChar, 1);
}


bool SerialPort::readFromPortUntilLF(char * buffer) {
    char * bufPtr =  buffer;
    ssize_t numBytes;
    //printf ("Waiting for serial...\n");
    do {
        // wait for a byte
        numBytes = read(_fileDescriptor, bufPtr, 1);//&buffer[sizeof(buffer)] - bufPtr - 1);
        if (numBytes == -1) {
            printf("Error reading from Serial port - %s(%d).\n", strerror(errno), errno);
            return false;
        }
        else if (numBytes > 0)
        {
            bufPtr += numBytes;
        }
        
        
    } while (*(bufPtr - 1) != '\n');
    
    
    // NUL terminate the string and see if we got an OK response
    *(bufPtr-2) = '\0';
    
    //printf("Read \"%s\"\n", buffer);
    
 
    
    return true;
}

void SerialPort::flushInput() {
    char throwAway[20];
    while (read(_fileDescriptor, throwAway,1) > 0);
    
}