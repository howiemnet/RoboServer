//
//  SerialClass.h
//  RoboServerA
//
//  Created by h on 09/03/2015.
//
//

#ifndef __RoboServerABINARY__SerialClass__
#define __RoboServerABINARY__SerialClass__

#include <stdio.h>
#include <stdint.h>
#include "SerialPort.hpp"
#include "JointStateStruct.hpp"

class SerialMotorDriverMMBinary {

public:
    SerialMotorDriverMMBinary(char * myPortName);
    ~SerialMotorDriverMMBinary();
    
    bool initDriver();
    int  checkDriverRev();
    long getCurrentPosition();
    long getCurrentSetpoint();
    void setOrigin();
    
    void moveAtVelocity(int16_t newVelocity);
    void moveToPosition(long newPosition);
    void setPIDS(PIDS newPIDS);
    PIDS getPIDS();
    void setVelocityLimit(int velLimit);
    void setVelocityFF(int8_t);
    void setAmpsLimit(int16_t milliAmpsLimit);
    void setDeadband(int16_t deadband);

private:
    SerialPort* _mySerialPort;
    char*   _myPortName; // should end up being "/dev/blah"
    int     _myAddress;  // the channel or joint number
    void    sendDataWithChecksum (int length, char * sendBuffer);
    bool    readSerialCharsWithChecksum (int length, char * sendBuffer);
    bool    _velLimitEnabled = false;
    
    
    
    
    int32_t readRegister4Bytes(int8_t registerNumber);
    int16_t readRegister2Bytes(int8_t registerNumber);
    int8_t  readRegister1Byte (int8_t registerNumber);
    
    bool    writeRegister1Byte   (int8_t registerNumber, int8_t  theValue);
    bool    writeRegister2Bytes   (int8_t registerNumber, int16_t theValue);
    bool    writeRegister4Bytes   (int8_t registerNumber, int32_t theValue);
    
    bool    _waitForACK();
};










#endif /* defined(__RoboServerA__SerialClass__) */
