//
//  RobotReadout.h
//  RoboServerA
//
//  Created by h on 12/03/2015.
//
//

#ifndef __RoboServerA__RobotReadout__
#define __RoboServerA__RobotReadout__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "SerialDisplay.hpp"
#include "JointStateStruct.hpp"

class RobotReadout {
    
    SerialDisplay * _myDisplay;
    int _currentChannel;
    void showChannelScreen();
    
    
public:
    RobotReadout(char * _myportName);
    void updateChannelScreen(JOINTSTATESTRUCT * myJointState);
};

#endif /* defined(__RoboServerA__RobotReadout__) */
