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

bool SerialMotorDriverMM::_waitForOK() {
    char readBuffer[64];
    _mySerialPort->readFromPortUntilLF(readBuffer);

    if (strncmp(readBuffer, MMOKResponseString, 2) == 0) {
        return true;
    } else {
        return false;
    }
}
