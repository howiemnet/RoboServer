//
//  RobotChannel.cpp
//  RoboServerA
//
//  Created by h on 09/03/2015.
//
//

#include "RobotChannel.hpp"
#include "Timer.hpp"
#include "JointStateStruct.hpp"


/*
.enabled = false,
.jointNumber = 1,
.jointControllerType = JCT_MOTIONMIND,
.controllerSerialAddress = (char *) "/dev/",
.friendlyName = (char *) "Base",
.PTerm = 200,
.ITerm = 0,
.DTerm = 0,
.PIDScalar = 8,
.minCount = 0,
.maxCount = 10000


*/


RobotChannel::RobotChannel (jointSetupData mySetupData)  // the channel or joint number


{
    _myPortName = mySetupData.controllerSerialAddress;
    _enabled = mySetupData.enabled;
    _driverType = mySetupData.jointControllerType;
    _myAddress = mySetupData.jointNumber;
    
    _myJointState = new JOINTSTATESTRUCT;
    _myJointState->currentPosition = 0;
    _myJointState->currentPositionTimeStamp = 0;
    _myJointState->currentLatency = 0;
    _myJointState->positionalError = 0;
    _myJointState->currentVelocity = 0;
    // set up appropriate driver. Only one type at the mo
    if (_enabled) {
        printf("Setting up channel %d\n", mySetupData.jointNumber);
        if (_driverType == JCT_MOTIONMIND) {
            // Motion Mind 3
            _myDriver = new SerialMotorDriverMM(_myPortName);
            _myDriver->initDriver();
            //
            // set up PID stuff
            //
            _myDriver->setPIDS(mySetupData.PTerm, mySetupData.ITerm, mySetupData.DTerm, mySetupData.PIDScalar);
        }
    } else {
        printf("Ignoring disabled channel %d\n", mySetupData.jointNumber);
    }
}





RobotChannel::~RobotChannel()
{
    delete _myDriver;
   
}

JOINTSTATESTRUCT * RobotChannel::getJointState() {
    return _myJointState;
}

void RobotChannel::moveToPosition(long newPosition) {
    _myDriver->moveToPosition(newPosition);
}

signed long RobotChannel::getCurrentPosition()
{
    
   // Timer timer = Timer();
   // timer.start();
   
        _myJointState->currentPosition = _myDriver->getCurrentPosition();
  
  //  double duration = timer.stop();
  //  timer.printTime(duration);
    
    return _myJointState->currentPosition;
}

void RobotChannel::printCurrentPosition() {
    if (_enabled) {
        printf("CurrentPosition of joint %d: %ld\n", _myAddress, _myJointState->currentPosition);
    }
}

void RobotChannel::moveAtVelocity(int newVelocity)
{
    
    //Timer timer = Timer();
    //timer.start();
    
    _myDriver->moveAtVelocity(newVelocity);
    
    //double duration = timer.stop();
    //timer.printTime(duration);
    
    // _currentPosition;
}

long RobotChannel::testCommsCycleTime() {
    Timer timer = Timer();
    timer.start();
    for (int i = 0; i < 20; i++) {
        getCurrentPosition();
        moveAtVelocity(2000);
    }
    double duration = timer.stop();
    timer.printTime(duration);
    
    return (long) duration;

}

void RobotChannel::startLoop() {
    if (_enabled) {
        _running = true;
        _myThread = new std::thread(&RobotChannel::runLoop,this);
    }
}

void RobotChannel::runLoop() {
    Timer timer = Timer();
    FILE * pFile;
    
    pFile = fopen ("myLog.csv","w");
    
    while (_running) {
        timer.start();
        getCurrentPosition();
        long despos = (long) 100 * (myCoordsHandler->getCoordinateAtTime(_myAddress, myTimeHandler->getPlaybackTime()));
        moveToPosition(despos);
        //moveAtVelocity(0);
        _myJointState->currentLatency = timer.stop();
        //
        // log to file
        long myTempTime = myTimeHandler->getPlaybackTime();
        if ((myTempTime > 0) && (myTempTime < 2000)) {
            fprintf(pFile,"%ld,%ld,%ld\n",myTempTime,_myJointState->currentPosition,despos);
        }
    }
    fclose(pFile);
    
}

void RobotChannel::stopLoop() {
    _running = false;
}


void RobotChannel::linkCoordsHandler(CoordinatesHandler * theCoordsHandler) {
   myCoordsHandler = theCoordsHandler;
    
}

void RobotChannel::linkTimeHandler(PlaybackTimeHandler * theTimeHandler) {
   myTimeHandler = theTimeHandler;
    
    
}