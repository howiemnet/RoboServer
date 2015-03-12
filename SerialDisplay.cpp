//
//  SerialDisplay.cpp
//  RoboServerA
//
//  Created by h on 10/03/2015.
//
//

#include "SerialDisplay.hpp"


SerialDisplay::SerialDisplay(char * myPortName) {
     _mySerialPort = new SerialPort(myPortName, B115200);
     _mySerialPort->openPort();
    
    
}

void SerialDisplay::clearDisplay() {
    _mySerialPort->writeChar('\x1A');
    
};

void SerialDisplay::writeToDisplay(char * displayString) {
    _mySerialPort->writeUntilNull(displayString);
}
