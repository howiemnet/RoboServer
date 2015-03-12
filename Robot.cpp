//
//  Robot.cpp
//  RoboServerA
//
//  Created by h on 09/03/2015.
//
//

#include "Robot.hpp"



Robot::Robot() {
    // set up channels / joints:
    myChannels[0] = new RobotChannel((char *) "/dev/cu.usbserial-A50285BI",0,1);
}

Robot::~Robot() {
    // set up channels / joints:
    delete myChannels[0];
}


bool Robot::init(){
    return true;

}

void Robot::printPositions() {
    //for (int i=0; i<myChannels.)
    myChannels[0]->printCurrentPosition();
}

long Robot::getSingleJointPosition(int jointNumber) {
    return myChannels[jointNumber]->getCurrentPosition();
}

void Robot::testCommsCycleTime() {
    myChannels[0]->testCommsCycleTime();
    
}

void Robot::startRunning() {
    myChannels[0]->startLoop();
    }

void Robot::stopRunning() {
    myChannels[0]->stopLoop();
}