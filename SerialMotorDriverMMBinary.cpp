//
//  SerialClass.cpp
//  RoboServerA
//
//  Created by h on 09/03/2015.
//
//

#include "SerialMotorDriverMMBinary.hpp"
#include <sys/types.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#define MMGetPosString       "R01 00\r\n"
#define MMSetVelString       "V01 "
#define MMMoveToPosString	 "P01 "
#define MMWritePRegString    "W01 04 "
#define MMWriteIRegString    "W01 05 "
#define MMWriteDRegString    "W01 06 "
#define MMWriteSRegString    "W01 08 "
#define MMOKResponseString   "OK"


        //
        //   CONSTANTS for BINARY COMMS
        //

#define RESPONSE_OK          0x06

#define MSG_CHANGE_SPEED     0x14
#define MSG_MOVETO_ABSOLUTE  0x15
#define MSG_MOVETO_RELATIVE  0x16
#define MSG_MOVEAT_VELOCITY  0x17
#define MSG_WRITE_REGISTER   0x18
#define MSG_WRITE_REG_STORE  0x19
#define MSG_RESTORE          0x1B
#define MSG_RESET            0x1C
#define MSG_READ_REGISTER    0x1D

        //  REGISTERS

#define REG_POSITION         0x00
#define REG_VELOCITYLIMIT    0x01
#define REG_VELOCITYFF       0x02
#define REG_FUNCTION         0x03
#define REG_PTERM            0x04
#define REG_ITERM            0x05
#define REG_DTERM            0x06
#define REG_ADDRESS          0x07
#define REG_PIDSCALAR        0x08
#define REG_TIMER            0x09
#define REG_RCMAX            0x0A
#define REG_RCMIN            0x0B
#define REG_RCBAND           0x0C
#define REG_RCCOUNT          0x0D
#define REG_VELOCITY         0x0E
#define REG_TIME             0x0F

#define REG_STATUS           0x10
#define REG_REVISION         0x11
#define REG_MODE             0x12
#define REG_ANALOGCON        0x13
#define REG_ANALOGFBCK       0x14
#define REG_PWMOUT           0x15
#define REG_INDEXPOS         0x16
#define REG_VNLIMIT          0x17
#define REG_VPLIMIT          0x18
#define REG_PWMLIMIT         0x19
#define REG_DEADBAND         0x1A
#define REG_DESIREDPOSITION  0x1B
#define REG_AMPSLIMIT        0x1C
#define REG_AMPS             0x1D
#define REG_FUNCTION2        0x1E
#define REG_THERMISTOR       0x1F

        // REGISTER SIZES (in bytes)

#define REG_SIZE_POSITION         4
#define REG_SIZE_VELOCITYLIMIT    2
#define REG_SIZE_VELOCITYFF       1
#define REG_SIZE_FUNCTION         2
#define REG_SIZE_PTERM            2
#define REG_SIZE_ITERM            2
#define REG_SIZE_DTERM            2
#define REG_SIZE_ADDRESS          1
#define REG_SIZE_PIDSCALAR        1
#define REG_SIZE_TIMER            1
#define REG_SIZE_RCMAX            2
#define REG_SIZE_RCMIN            2
#define REG_SIZE_RCBAND           2
#define REG_SIZE_RCCOUNT          2
#define REG_SIZE_VELOCITY         2
#define REG_SIZE_TIME             4

#define REG_SIZE_STATUS           2
#define REG_SIZE_REVISION         1
#define REG_SIZE_MODE             1
#define REG_SIZE_ANALOGCON        2
#define REG_SIZE_ANALOGFBCK       2
#define REG_SIZE_PWMOUT           2
#define REG_SIZE_INDEXPOS         4
#define REG_SIZE_VNLIMIT          4
#define REG_SIZE_VPLIMIT          4
#define REG_SIZE_PWMLIMIT         2
#define REG_SIZE_DEADBAND         2
#define REG_SIZE_DESIREDPOSITION  4
#define REG_SIZE_AMPSLIMIT        2
#define REG_SIZE_AMPS             2
#define REG_SIZE_FUNCTION2        2
#define REG_SIZE_THERMISTOR       2



        // FUNCTION bits

#define FNC1_POSPWRUP        0x0001
#define FNC1_RETPOS          0x0002
#define FNC1_RETVEL          0x0004
#define FNC1_RETTIME         0x0008
#define FNC1_SATPROT         0x0010
#define FNC1_SAVEPOS         0x0020
#define FNC1_VELLIMIT        0x0040
#define FNC1_ACTIVESTOP      0x0080
#define FNC1_LASTRC          0x0100
#define FNC1_ADSTEP          0x0200
#define FNC1_ADSERIAL        0x0400
#define FNC1_ENABLEDB        0x0800
#define FNC1_RCPOS_ENCFDBCK  0x1000
#define FNC1_VIRTLIMIT       0x2000
#define FNC1_DISABLEPID      0x4000
#define FNC1_DISABLEBLINK    0x8000

        // FUNCTION2 bits

#define FNC2_FREQ_DIV_2      0x0001
#define FNC2_FREQX2          0x0002
#define FNC2_ADX2            0x0004
#define FNC2_ADX4            0x0008
#define FNC2_ADX8            0x0010
#define FNC2_ADX16           0x0020
#define FNC2_ADX32           0x0040
#define FNC2_ADX64           0x0080
#define FNC2_OVERTEMP        0x0100
#define FNC2_PULSEFB         0x0200
#define FNC2_BAUD38400       0x0400
#define FNC2_NEWVELMODE      0x0800
#define FNC2_BAUD115200      0x1000
#define FNC2_FASTPID         0x2000


SerialMotorDriverMMBinary::SerialMotorDriverMMBinary(char * myPortName) {
    _myPortName = myPortName;
    printf("SerialMotorDriver constructor called - ");
    printf("%s", _myPortName);
    printf("\n\n");
    _mySerialPort = new SerialPort(myPortName, B115200);
    // TODO: need to add some checking here. See if we can get the driver revision back etc.
    // also, set baud rate and reset the MM if necessary
}

SerialMotorDriverMMBinary::~SerialMotorDriverMMBinary() {
    
    delete _mySerialPort;
}


bool SerialMotorDriverMMBinary::initDriver() {
    printf("SMD::initDriver");
    _mySerialPort->openPort();
    
    return true;
  }


void SerialMotorDriverMMBinary::setAmpsLimit(int16_t milliAmpsLimit) {
    uint16_t myValue = milliAmpsLimit / 20;
    writeRegister2Bytes(REG_AMPSLIMIT, (int16_t) myValue);
}



int SerialMotorDriverMMBinary::checkDriverRev() {
    
    
    
    return 0;
}




signed long SerialMotorDriverMMBinary::getCurrentPosition() {
    return readRegister4Bytes(REG_POSITION);
}

signed long SerialMotorDriverMMBinary::getCurrentSetpoint() {
    return readRegister4Bytes(REG_DESIREDPOSITION);
}

void SerialMotorDriverMMBinary::setOrigin() {
    
    writeRegister4Bytes(REG_POSITION, ((int32_t) 0));
}


void SerialMotorDriverMMBinary::moveAtVelocity(int16_t newVelocity) {
    char sendBuffer[6];
    
    sendBuffer[0] = MSG_MOVEAT_VELOCITY;
    sendBuffer[1] = 1;
    sendBuffer[2] = (char) ( newVelocity        & 0xFF);
    sendBuffer[3] = (char) ((newVelocity >>  8) & 0xFF);
    
    int8_t retries = 3;
    bool   validResponseReceived = false;
    
    while ((retries > 0) && (!validResponseReceived)) {
        sendDataWithChecksum(4, sendBuffer);
        validResponseReceived = _waitForACK();
        retries--;
    }
    assert(validResponseReceived);
}
    

void SerialMotorDriverMMBinary::moveToPosition(long newPosition) {
    //_mySerialPort->flushInput();

    char sendBuffer[7];
    
    sendBuffer[0] = MSG_MOVETO_ABSOLUTE;
    sendBuffer[1] = 1;
    sendBuffer[2] = (char) ( newPosition        & 0xFF);
    sendBuffer[3] = (char) ((newPosition >>  8) & 0xFF);
    sendBuffer[4] = (char) ((newPosition >> 16) & 0xFF);
    sendBuffer[5] = (char) ((newPosition >> 24) & 0xFF);
    
    int8_t retries = 3;
    bool   validResponseReceived = false;
    
    while ((retries > 0) && (!validResponseReceived)) {
        sendDataWithChecksum(6, sendBuffer);
        validResponseReceived = _waitForACK();
        retries--;
    }
    //assert(validResponseReceived);
}





bool SerialMotorDriverMMBinary::_waitForACK() {
    char readBuffer[3];
    _mySerialPort->readNumberOfCharactersFromPort(1, readBuffer);

    if (readBuffer[0] == RESPONSE_OK) {
        return true;
    } else {
        return false;
    }
}










void SerialMotorDriverMMBinary::sendDataWithChecksum(int length, char * sendBuffer) {
    uint8_t checksum = 0;
    for (int i=0; i<length; i++)
    {
        checksum += (uint8_t) sendBuffer[i];
        _mySerialPort->writeChar((char)sendBuffer[i]);
        //    println (hex( sendBuffer[i]));
    }
    _mySerialPort->writeChar((char) checksum & 0xFF);
}


bool SerialMotorDriverMMBinary::readSerialCharsWithChecksum(int length, char * receiveBuffer) {
    _mySerialPort->readNumberOfCharactersFromPort(length+1, receiveBuffer);
    uint16_t checksum = 0;
    for (int i=0; i<= (length-1); i++) {
        checksum += (unsigned int) receiveBuffer[i];
    }
    if ((checksum & 0xFF) == (uint8_t) receiveBuffer[length]) {
        return true;
    } else {
        printf("Checksum expected: %d; checksum received: %d\n", (uint8_t) receiveBuffer[length], checksum);
        //assert (false);
        return false;
    }
}


int32_t SerialMotorDriverMMBinary::readRegister4Bytes(int8_t registerNumber)
{
    
    char sendBuffer[4];
    char recvBuffer[10];
    
    sendBuffer[0] = MSG_READ_REGISTER;
    sendBuffer[1] = 1;
    sendBuffer[2] = registerNumber;
    
    int8_t retries = 3;
    bool   validResponseReceived = false;
    
    while ((retries > 0) && (!validResponseReceived)) {
        _mySerialPort->flushInput();
        sendDataWithChecksum(3, sendBuffer);
        validResponseReceived = readSerialCharsWithChecksum(5,recvBuffer);
        retries--;
    }
    
    assert(validResponseReceived);
    int32_t myValue = ((recvBuffer[1] & 0xFF)) | ((recvBuffer[2] & 0xFF) << 8) | ((recvBuffer[3] & 0xFF) << 16) | ((recvBuffer[4] & 0xFF) << 24);
    return myValue;
}


int16_t SerialMotorDriverMMBinary::readRegister2Bytes(int8_t registerNumber)
{
    
    char sendBuffer[4];
    char recvBuffer[10];
    
    sendBuffer[0] = MSG_READ_REGISTER;
    sendBuffer[1] = 1;
    sendBuffer[2] = registerNumber;
    
    int8_t retries = 3;
    bool   validResponseReceived = false;
    
    while ((retries > 0) && (!validResponseReceived)) {
        _mySerialPort->flushInput();
        sendDataWithChecksum(3, sendBuffer);
        validResponseReceived = readSerialCharsWithChecksum(3,recvBuffer);
        retries--;
    }
    
    assert(validResponseReceived);
    int16_t myValue = ((recvBuffer[1] & 0xFF)) | ((recvBuffer[2] & 0xFF) << 8);
    return myValue;
}

int8_t SerialMotorDriverMMBinary::readRegister1Byte(int8_t registerNumber)
{
    
    char sendBuffer[4];
    char recvBuffer[10];
    
    sendBuffer[0] = MSG_READ_REGISTER;
    sendBuffer[1] = 1;
    sendBuffer[2] = registerNumber;
    
    int8_t retries = 3;
    bool   validResponseReceived = false;
    
    while ((retries > 0) && (!validResponseReceived)) {
        _mySerialPort->flushInput();
        sendDataWithChecksum(2, sendBuffer);
        validResponseReceived = readSerialCharsWithChecksum(3,recvBuffer);
        retries--;
    }
    
    assert(validResponseReceived);
    int8_t myValue = ((recvBuffer[1] & 0xFF));
    return myValue;
}






bool    SerialMotorDriverMMBinary::writeRegister1Byte   (int8_t registerNumber, int8_t  theValue) {
    char sendBuffer[6];
    sendBuffer[0] = MSG_WRITE_REGISTER;
    sendBuffer[1] = 1;
    sendBuffer[2] = (char) registerNumber;
    sendBuffer[3] = (char) ( theValue         & 0xFF);
    
    int8_t retries = 3;
    bool   validResponseReceived = false;
    
    while ((retries > 0) && (!validResponseReceived)) {
        sendDataWithChecksum(4, sendBuffer);
        validResponseReceived = _waitForACK();
        retries--;
    }
    assert(validResponseReceived);
    return validResponseReceived;
}



bool    SerialMotorDriverMMBinary::writeRegister2Bytes    (int8_t registerNumber, int16_t  theValue) {
    char sendBuffer[6];
    sendBuffer[0] = MSG_WRITE_REGISTER;
    sendBuffer[1] = 1;
    sendBuffer[2] = (char) registerNumber;
    sendBuffer[3] = (char) ( theValue         & 0xFF);
    sendBuffer[4] = (char) ((theValue >>   8) & 0xFF);
    
    int8_t retries = 3;
    bool   validResponseReceived = false;
    
    while ((retries > 0) && (!validResponseReceived)) {
        sendDataWithChecksum(5, sendBuffer);
        validResponseReceived = _waitForACK();
        retries--;
    }
    assert(validResponseReceived);
    return validResponseReceived;
}



bool    SerialMotorDriverMMBinary::writeRegister4Bytes    (int8_t registerNumber, int32_t  theValue) {
    char sendBuffer[8];
    sendBuffer[0] = MSG_WRITE_REGISTER;
    sendBuffer[1] = 1;
    sendBuffer[2] = (char) registerNumber;
    sendBuffer[3] = (char) ( theValue         & 0xFF);
    sendBuffer[4] = (char) ((theValue >>   8) & 0xFF);
    sendBuffer[5] = (char) ((theValue >>  16) & 0xFF);
    sendBuffer[6] = (char) ((theValue >>  24) & 0xFF);
    
    int8_t retries = 3;
    bool   validResponseReceived = false;
    
    while ((retries > 0) && (!validResponseReceived)) {
        sendDataWithChecksum(7, sendBuffer);
        validResponseReceived = _waitForACK();
        retries--;
    }
    assert(validResponseReceived);
    return validResponseReceived;
}


PIDS SerialMotorDriverMMBinary::getPIDS() {
    PIDS tempPIDS;
    tempPIDS.PTerm = readRegister2Bytes(REG_PTERM);
    tempPIDS.ITerm = readRegister2Bytes(REG_ITERM);
    tempPIDS.DTerm = readRegister2Bytes(REG_DTERM);
    tempPIDS.PIDScalar = readRegister1Byte(REG_PIDSCALAR);
    return (tempPIDS);
}



/*

void SerialMotorDriverMMBinary::setPIDS(int16_t PTerm, int16_t ITerm, int16_t DTerm, int8_t Scalar) {
    long _p = readRegister2Bytes(REG_PTERM);
    long _i = readRegister2Bytes(REG_ITERM);
    long _d = readRegister2Bytes(REG_DTERM);
    int  _s = readRegister1Byte(REG_PIDSCALAR);
    printf(" +--- Channel %d PID was set to P:%lu I:%lu D:%lu Scalar:%d\n", _myAddress, _p, _i, _d, _s);


    // P
    
    assert(writeRegister(REG_PTERM, (int16_t) PTerm));
    
    // I
    
    assert(writeRegister(REG_ITERM, (int16_t) ITerm));
    
    // D
    
    assert(writeRegister(REG_DTERM, (int16_t) DTerm));

    // S
    
    assert(writeRegister(REG_PIDSCALAR, (int8_t) Scalar));
    
    // Check values written OK:
    
     _p = readRegister2Bytes(REG_PTERM);
    _i = readRegister2Bytes(REG_ITERM);
    _d = readRegister2Bytes(REG_DTERM);
    _s = readRegister1Byte(REG_PIDSCALAR);
    printf(" +--- Channel %d PID now set to P:%lu I:%lu D:%lu Scalar:%d\n", _myAddress, _p, _i, _d, _s);
    
}
 */


void SerialMotorDriverMMBinary::setPIDS(PIDS newPIDS) {
    // P
    
    assert(writeRegister2Bytes(REG_PTERM, (int16_t) newPIDS.PTerm));
    
    // I
    
    assert(writeRegister2Bytes(REG_ITERM, (int16_t) newPIDS.ITerm));
    
    // D
    
    assert(writeRegister2Bytes(REG_DTERM, (int16_t) newPIDS.DTerm));
    
    // S
    
    assert(writeRegister1Byte(REG_PIDSCALAR, (int8_t) newPIDS.PIDScalar));
    
}


void SerialMotorDriverMMBinary::setVelocityLimit(int maxVelocity) {
    
    assert(writeRegister2Bytes(REG_VELOCITYLIMIT, (int16_t) maxVelocity));
    
    if (!_velLimitEnabled) {
        _velLimitEnabled = true;
    
    // Get function bits
   int16_t functionBits = readRegister2Bytes(REG_FUNCTION);
 
    // Set VELLLIMIT bit
    functionBits = functionBits | FNC1_VELLIMIT;
    
    // Write function bits back
    assert(writeRegister2Bytes (REG_FUNCTION, (int16_t) functionBits));
    }
}

void SerialMotorDriverMMBinary::setVelocityFF(int8_t velFF) {
    assert(writeRegister1Byte(REG_VELOCITYFF, (int8_t) velFF));
}

void SerialMotorDriverMMBinary::setDeadband(int16_t deadband) {
    assert(writeRegister2Bytes(REG_DEADBAND, deadband));
           
}







