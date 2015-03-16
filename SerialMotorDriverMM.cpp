//
//  SerialClass.cpp
//  RoboServerA
//
//  Created by h on 09/03/2015.
//
//

#include "SerialMotorDriverMM.hpp"
#include <sys/types.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#define MMGetPosString	"R01 00\r\n"
#define MMSetVelString	"V01 "
#define MMMoveToPosString	"P01 "
#define MMWritePRegString    "W01 04 "
#define MMWriteIRegString    "W01 05 "
#define MMWriteDRegString    "W01 06 "
#define MMWriteSRegString    "W01 08 "
#define MMOKResponseString "OK"



SerialMotorDriverMM::SerialMotorDriverMM(char * myPortName) {
    _myPortName = myPortName;
    printf("SerialMotorDriver constructor called - ");
    printf("%s", _myPortName);
    printf("\n\n");
    _mySerialPort = new SerialPort(myPortName, B115200);

}

SerialMotorDriverMM::~SerialMotorDriverMM() {
    delete _mySerialPort;
}


bool SerialMotorDriverMM::initDriver() {
    printf("SMD::initDriver");
    _mySerialPort->openPort();
    
    return true;
  }

int SerialMotorDriverMM::checkDriverRev() {
    
    
    
    return 0;
}

signed long SerialMotorDriverMM::getCurrentPosition() {

    _mySerialPort->writeToPort((char *) MMGetPosString, sizeof(MMGetPosString)-1);

    char readBuffer[64];
    _mySerialPort->readFromPortUntilLF(readBuffer);
    char * endPtr = readBuffer + 10;
    signed long myPos = strtol(&readBuffer[5],&endPtr,10);
    //printf("\nPOSITION: %ld\n", myPos);
    return myPos;
}

void SerialMotorDriverMM::moveAtVelocity(int newVelocity) {
    _mySerialPort->writeToPort((char *) MMSetVelString, sizeof(MMSetVelString)-1);
    char numberBuffer[20];
    int numLength = sprintf(numberBuffer, "%04d", newVelocity);
    _mySerialPort->writeToPort(numberBuffer, numLength);
    _mySerialPort->writeToPort((char *) "\r\n", 2);
    
    _waitForOK();
}

void SerialMotorDriverMM::moveToPosition(long newPosition) {
    _mySerialPort->writeToPort((char *) MMMoveToPosString, sizeof(MMMoveToPosString)-1);
    char numberBuffer[20];
    int numLength = sprintf(numberBuffer, "%06ld", newPosition);
    _mySerialPort->writeToPort(numberBuffer, numLength);
    _mySerialPort->writeToPort((char *) "\r\n", 2);
    
    _waitForOK();
}

bool SerialMotorDriverMM::_waitForOK() {
    char readBuffer[64];
    _mySerialPort->readFromPortUntilLF(readBuffer);

    if (strncmp(readBuffer, MMOKResponseString, 2) == 0) {
        return true;
    } else {
        return false;
    }
}


void SerialMotorDriverMM::setPIDS(int PTerm, int ITerm, int DTerm, int Scalar) {
    char numberBuffer[20];

    // P
    
    _mySerialPort->writeToPort((char *) MMWritePRegString, sizeof(MMWritePRegString)-1);
    int numLength = sprintf(numberBuffer, "%04d", PTerm);
    _mySerialPort->writeToPort(numberBuffer, numLength);
    _mySerialPort->writeToPort((char *) "\r\n", 2);
    _waitForOK();
    
    // I
    
    _mySerialPort->writeToPort((char *) MMWriteIRegString, sizeof(MMWriteIRegString)-1);
    numLength = sprintf(numberBuffer, "%04d", ITerm);
    _mySerialPort->writeToPort(numberBuffer, numLength);
    _mySerialPort->writeToPort((char *) "\r\n", 2);
    _waitForOK();
    
    // D
    
    _mySerialPort->writeToPort((char *) MMWriteDRegString, sizeof(MMWriteDRegString)-1);
    numLength = sprintf(numberBuffer, "%04d", DTerm);
    _mySerialPort->writeToPort(numberBuffer, numLength);
    _mySerialPort->writeToPort((char *) "\r\n", 2);
    _waitForOK();
    
    // S
    
    _mySerialPort->writeToPort((char *) MMWriteSRegString, sizeof(MMWriteSRegString)-1);
    numLength = sprintf(numberBuffer, "%04d", Scalar);
    _mySerialPort->writeToPort(numberBuffer, numLength);
    _mySerialPort->writeToPort((char *) "\r\n", 2);
    _waitForOK();
    
    
    
}