//
//  SerialPort.h
//  RoboServerA
//
//  Created by h on 09/03/2015.
//
//

#ifndef __RoboServerA__SerialPort__
#define __RoboServerA__SerialPort__

#include <stdio.h>
#include <termios.h>


class SerialPort {
    char *  _myPortName;
    int     _fileDescriptor;
    long    _myBaudRate;
    struct termios gOriginalTTYAttrs;
    
    
public:
    SerialPort(char * portName, long myBaudRate);
    ~SerialPort();
    bool openPort();
    void closePort();
    void writeChar(char theChar);
    bool writeToPort(char * writeBuffer, int bufferLength);
    void writeUntilNull(char * writeBuffer);
    void flushInput();

    bool readFromPortUntilLF(char * readBuffer);
    
};





#endif /* defined(__RoboServerA__SerialPort__) */
