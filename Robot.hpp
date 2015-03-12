//
//  Robot.h
//  RoboServerA
//
//  Created by h on 09/03/2015.
//
//

#ifndef __RoboServerA__Robot__
#define __RoboServerA__Robot__

#include <stdio.h>
#include "RobotChannel.hpp"
#include <thread>

class Robot {
    
public:
    Robot();
    ~Robot();
    bool init();
    void printPositions();
    long getSingleJointPosition(int jointNumber);
    void testCommsCycleTime();
    void startRunning();
    void stopRunning();
    
private:
    RobotChannel* myChannels[1];
   
    
};




#endif /* defined(__RoboServerA__Robot__) */
