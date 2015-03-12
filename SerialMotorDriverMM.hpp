//
//  SerialClass.h
//  RoboServerA
//
//  Created by h on 09/03/2015.
//
//

#ifndef __RoboServerA__SerialClass__
#define __RoboServerA__SerialClass__

#include <stdio.h>
#include "SerialPort.hpp"

class SerialMotorDriverMM {

public:
    SerialMotorDriverMM(char * myPortName);
    ~SerialMotorDriverMM();
    
    bool initDriver();
    int  checkDriverRev();
    long getCurrentPosition();
    void moveAtVelocity(int newVelocity);

private:
    SerialPort* _mySerialPort;
    char*   _myPortName; // should end up being "/dev/blah"
    int     _myAddress;  // the channel or joint number
    
    bool    _waitForOK();
};










#endif /* defined(__RoboServerA__SerialClass__) */
