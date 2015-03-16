//
//  RobotChannel.h
//  RoboServerA
//
//  Created by h on 09/03/2015.
//
//

#ifndef __RoboServerA__RobotChannel__
#define __RoboServerA__RobotChannel__

#include <stdio.h>
#include <chrono>
#include <thread>

#include "SerialMotorDriverMM.hpp"
#include "JointStateStruct.hpp"
#include "PlaybackTimeHandler.hpp"
#include "CoordinatesHandler.hpp"


class RobotChannel {
protected:

    //
    // Driver stuff
    //

    char*   _myPortName; // should be summat like "/dev/usb-serial2374"
    int     _myAddress;  // the channel or joint number
    int     _driverType;
    bool    _enabled = false;
    volatile bool    _running = false;
    

    
    SerialMotorDriverMM * _myDriver;
    std::thread * _myThread;

    //
    // Joint stuff
    //

    JOINTSTATESTRUCT * _myJointState;
    PlaybackTimeHandler * myTimeHandler;
    CoordinatesHandler * myCoordsHandler;
    
    
    
public:
    RobotChannel(jointSetupData mySetupData);
    ~RobotChannel();
    
    long getCurrentPosition();
    void printCurrentPosition();
    void moveAtVelocity(int newVelocity);
    void moveToPosition(long newPosition);
    
    long testCommsCycleTime();
    
    bool isRunning();
    bool isEnabled();
    
    void startLoop();
    void runLoop();
    void stopLoop();
    JOINTSTATESTRUCT * getJointState();
    void linkTimeHandler(PlaybackTimeHandler * theTimeHandler);
    void linkCoordsHandler(CoordinatesHandler * theCoordsHandler);

    
    
};

#endif /* defined(__RoboServerA__RobotChannel__) */
