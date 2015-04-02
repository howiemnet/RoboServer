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

extern char myPorts[7][36];
extern int portMapping[7];

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
    _myPortName = ( myPorts[portMapping[mySetupData.jointNumber]]);
    _enabled = mySetupData.enabled;
    _driverType = mySetupData.jointControllerType;
    _myAddress = mySetupData.jointNumber;
    _scale = mySetupData.scale;
    _offset = 0;
    
    _myJointState = new JOINTSTATESTRUCT;
    _myJointState->currentPosition = 0;
    _myJointState->currentPositionTimeStamp = 0;
    _myJointState->currentLatency = 0;
    _myJointState->positionalError = 0;
    _myJointState->currentVelocity = 0;
    // set up appropriate driver. Only one type at the mo
    if (_enabled) {
        printf(" +- Setting up channel %d, which is portmapped to %d \n", mySetupData.jointNumber, portMapping[mySetupData.jointNumber]);
        if (_driverType == JCT_MOTIONMIND) {
            // Motion Mind 3
            printf(" +- Shouls be portname %s\n", _myPortName);
            
            _myDriver = new SerialMotorDriverMMBinary(myPorts[mySetupData.jointNumber-1]);
            _myDriver->initDriver();
            //
            // set up PID stuff
            //
            _myDriver->setAmpsLimit(mySetupData.milliAmpsLimit);
            _myDriver->setPIDS(mySetupData.jointPIDS);
            _myDriver->setVelocityLimit(375);
            _myDriver->setVelocityFF(50);
            _myDriver->setDeadband(3);
        }
    } else {
        printf(" +- Ignoring disabled channel %d\n", mySetupData.jointNumber);
    }
}

void RobotChannel::setVelocityLimit(int16_t velLimit) {
    if (_enabled) {
        _myDriver->setVelocityLimit(velLimit);
    }
}

void RobotChannel::setOrigin(){
    _myDriver->setOrigin();
}

void RobotChannel::setPIDS(PIDS newPIDS) {
    _myDriver->setPIDS(newPIDS);
}

PIDS RobotChannel::getPIDS() {
    return _myDriver->getPIDS();
}


RobotChannel::~RobotChannel()
{
    delete _myDriver;
   
}

JOINTSTATESTRUCT * RobotChannel::getJointState() {
    return _myJointState;
}

void RobotChannel::moveToPosition(long newPosition) {
    if (_enabled) {
        _myDriver->moveToPosition(newPosition);
    }
}

signed long RobotChannel::getCurrentPosition()
{
   _myJointState->currentPosition = _myDriver->getCurrentPosition();
   return _myJointState->currentPosition;
}


signed long RobotChannel::getCurrentSetpoint()
{
    _myJointState->currentPosition = _myDriver->getCurrentSetpoint();
    return _myJointState->currentPosition;
}

void RobotChannel::setStartPosition() {
    getCurrentPosition();
    float firstPos = _scale * myCoordsHandler->getCoordinateAtTime(_myAddress, 0);
    _offset = firstPos - _myJointState->currentPosition;
}


void RobotChannel::printCurrentPosition() {
    if (_enabled) {
        Timer timer = Timer();
        timer.start();

        _myJointState->currentPosition = _myDriver->getCurrentPosition();
        
        double duration = timer.stop();
        timer.printTime(duration);

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

void RobotChannel::nudge(int nudgeAmount) {
    if (_enabled) {
    getCurrentPosition();
    moveToPosition(_myJointState->currentPosition + nudgeAmount);
    }
}


void RobotChannel::runLoop() {
    Timer timer = Timer();
    FILE * pFile;
    int _maxVelocity = 500;
    int _maxAccel = 500;
    int _maxDeccel = -5000;
    pFile = fopen ("myLogJ2.csv","w");
    
    while (_running) {
        timer.start();
        
        //long myTempPos = getCurrentPosition();
        //_myJointState->currentPosition + (_myJointState->currentVelocity * 0); // take account of last
        //getCurrentPosition();
        long despos = ((_scale / (3.14159265)) * (myCoordsHandler->getCoordinateAtTime(_myAddress, myTimeHandler->getPlaybackTime()))) - _offset;
        //long desposB = (myCoordsHandler->getCoordinateAtTime(_myAddress, 20+myTimeHandler->getPlaybackTime()));
        
        //long distInNextPeriod = despos - myTempPos;
        long tempVel = 0;
        
        if (false) { //true) { //distInNextPeriod > 4) {
            // move at vel
         /*   tempVel = distInNextPeriod / (20 / 5);
            if (tempVel > _maxVelocity) {
                tempVel = _maxVelocity;
            }
            if (tempVel < (-_maxVelocity)) {
                tempVel = -_maxVelocity;
            }
            int deltaVel = (int) tempVel - (int) _myJointState->currentVelocity;
            
            if (deltaVel > _maxAccel) {
                tempVel = _myJointState->currentVelocity + _maxAccel;
            }
            if (deltaVel < _maxDeccel) {
                tempVel = _myJointState->currentVelocity + _maxDeccel;
            
            }
           */ _myJointState->currentVelocity = tempVel;
            //_myDriver->setVelocityLimit((int) tempVel); //
            moveAtVelocity((int) tempVel/2);
        } else {
            // move to pos
            moveToPosition(despos);
            
        }
        //moveToPosition(despos);
        //usleep(10000);
        
        //moveAtVelocity((int) ((despos - _myJointState->currentPosition)/100.0));
        //
        _myJointState->currentLatency = timer.stop();
        //
        // log to file
        long myTempTime = myTimeHandler->getPlaybackTime();
        if (_myAddress == 2) {
                if ((myTempTime > 0) && (myTempTime < 8000)) {
                    fprintf(pFile,"%ld,%ld,%ld,%ld\n",myTempTime,_myJointState->currentPosition,despos,tempVel);
                }
        }
    }
    
    moveAtVelocity(0);
    
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