//
//  RobotChannel.cpp
//  RoboServerA
//
//  Created by h on 09/03/2015.
//
//

#include "RobotChannel.hpp"
#include "Timer.hpp"

RobotChannel::RobotChannel (char*   portName, // should end up being "/dev/blah"
                            int     address,
                            int     driverType)  // the channel or joint number
{
    _myPortName = portName;
    _myAddress = address;
    _driverType = driverType;
    
    // set up appropriate driver. Only one type at the mo
    if (driverType == 1) {
        // Motion Mind 3
        _myDriver = new SerialMotorDriverMM(portName);
        _myDriver->initDriver();
    }
}

RobotChannel::~RobotChannel()
{
    delete _myDriver;
   
}

signed long RobotChannel::getCurrentPosition()
{
    
   // Timer timer = Timer();
   // timer.start();
   
        _currentPosition = _myDriver->getCurrentPosition();
  
  //  double duration = timer.stop();
  //  timer.printTime(duration);
    
    return _currentPosition;
}

void RobotChannel::printCurrentPosition() {
    printf("CurrentPosition of joint %d: %ld\n", _myAddress, _currentPosition);
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
    _running = true;
    _myThread = new std::thread(&RobotChannel::runLoop,this);
}

void RobotChannel::runLoop() {

    while (true) {
        if (_running) {
          getCurrentPosition();
        }
    }
}

void RobotChannel::stopLoop() {
    _running = false;
}