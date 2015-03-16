//
//  Robot.cpp
//  RoboServerA
//
//  Created by h on 09/03/2015.
//
//

#include "Robot.hpp"



Robot::Robot() {
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

void Robot::testCommsCycleTime() {
    myChannels[0]->testCommsCycleTime();
    
}

void Robot::startRunning() {
    for (int i=0; i<NUMOFCHANNELS; i++) {
        myChannels[i]->startLoop();
    }
}

void Robot::stopRunning() {
    for (int i=0; i<NUMOFCHANNELS; i++) {
        myChannels[i]->stopLoop();
    }
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