//
//  Robot.cpp
//  RoboServerA
//
//  Created by h on 09/03/2015.
//
//

#include "Robot.hpp"



Robot::Robot() {
    _cycleRunning = false;
}

Robot::~Robot() {
    // set up channels / joints:
    for (int i=0; i<NUMOFCHANNELS; i++) {
        delete myChannels[i];
    }
}


bool Robot::initComms(){
    // set up channels / joints:

    for (int i=0; i<NUMOFCHANNELS; i++) {
        myChannels[i] = new RobotChannel ( jointData[i] ) ;
    }
    return true;
}

void Robot::printPositions() {
    for (int i=0; i<NUMOFCHANNELS; i++)
    {
        myChannels[i]->printCurrentPosition();
    }
}

long Robot::getSingleJointPosition(int jointNumber) {
    return myChannels[jointNumber]->getCurrentPosition();
}

void Robot::nudge(int channel, int nudgeAmount) {
    myChannels[channel]->nudge(nudgeAmount);
}


void Robot::testCommsCycleTime() {
    myChannels[0]->testCommsCycleTime();
    
}

void Robot::setVelocityLimits(int16_t velLimit) {
    for (int i=0; i<NUMOFCHANNELS; i++)
    {
        myChannels[i]->setVelocityLimit(velLimit);
    }
 
}

void Robot::startRunning() {
    if (!_cycleRunning) {
        for (int i=0; i<NUMOFCHANNELS; i++) {
            myChannels[i]->startLoop();
        }
        _cycleRunning = true;
        printf("Robot RUNNING\n");
    } else {
        printf("Robot already running\n");        
    }
}

void Robot::stopRunning() {
    for (int i=0; i<NUMOFCHANNELS; i++) {
        myChannels[i]->stopLoop();
    }
    printf("Robot STOPPED\n");

}

bool Robot::homeChannels() {
    return false;
}

JOINTSTATESTRUCT * Robot::getJointData(int channel) {
    return myChannels[channel]->getJointState();
}

bool Robot::running() {
    return _cycleRunning;
}

void Robot::runCycle() {
    timeval myStartTime;
    gettimeofday(&myStartTime, NULL);
    

}

void Robot::setAllChannelOrigins() {
    for (int i=0; i < 6; i++) {
        myChannels[i]->setOrigin();
    }
}

void Robot::gotoZero() {
    for (int i=0; i < 6; i++) {
        myChannels[i]->moveToPosition(0);
    }
}

void Robot::setChannelOrigin(int channel) {
    myChannels[channel]->setOrigin();
}

void Robot::setStartPositions(){
    for (int i=0; i<NUMOFCHANNELS; i++)
    {
        myChannels[i]->setStartPosition();
    }
    
}

void Robot::linkCoordsHandler(CoordinatesHandler * theCoordsHandler) {
    for (int i=0; i<NUMOFCHANNELS; i++)
    {
        myChannels[i]->linkCoordsHandler(theCoordsHandler);
    }

}

void Robot::linkTimeHandler(PlaybackTimeHandler * theTimeHandler) {
    for (int i=0; i<NUMOFCHANNELS; i++)
    {
        myChannels[i]->linkTimeHandler(theTimeHandler);
    }

}