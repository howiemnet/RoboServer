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
#include <thread>
#include "SerialDisplay.hpp"
#include "JointStateStruct.hpp"

class RobotReadout {
    
    SerialDisplay * _myDisplay;
    
    int _currentChannel;
    bool _threadEnabled;
    void _updateScreen();

    std::thread * _readoutThread;
    JOINTSTATESTRUCT * _jointData;
    
public:
    RobotReadout(char * _myportName);
    void showChannelScreen(JOINTSTATESTRUCT * theJointData);
    void startReadoutThread();
};

#endif /* defined(__RoboServerA__RobotReadout__) */
