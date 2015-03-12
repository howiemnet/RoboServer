//
//  SerialDisplay.h
//  RoboServerA
//
//  Created by h on 10/03/2015.
//
//

#ifndef __RoboServerA__SerialDisplay__
#define __RoboServerA__SerialDisplay__

#include <stdio.h>
#include "SerialPort.hpp"

class SerialDisplay {
    SerialPort* _mySerialPort;
    
public:
    SerialDisplay(char * myPortName);
    void clearDisplay();
    void writeToDisplay(char * displayString);
        
    
};



#endif /* defined(__RoboServerA__SerialDisplay__) */
