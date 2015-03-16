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
#include "JointStateStruct.hpp"
#include "Puma260Robot.hpp"
#include "PlaybackTimeHandler.hpp"
#include "CoordinatesHandler.hpp"
#include <sys/time.h>

class Robot {
    
public:
    Robot();
    ~Robot();
    bool initComms();
    bool homeChannels();
    void printPositions();
    long getSingleJointPosition(int jointNumber);
    void testCommsCycleTime();
    void startRunning();
    void stopRunning();
    void runCycle();
    bool running();
    JOINTSTATESTRUCT * getJointData(int channel);
    void linkTimeHandler(PlaybackTimeHandler * theTimeHandler);
    void linkCoordsHandler(CoordinatesHandler * theCoordsHandler);
    
private:
    RobotChannel* myChannels[6];
    bool _cycleRunning;
   
    
};




#endif /* defined(__RoboServerA__Robot__) */
