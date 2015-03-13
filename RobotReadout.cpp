//
//  RobotReadout.cpp
//  RoboServerA
//
//  Created by h on 12/03/2015.
//
//

#include "RobotReadout.hpp"
#include <time.h>
#include <unistd.h>

RobotReadout::RobotReadout(char * _myPortName) {
    _myDisplay = new SerialDisplay(_myPortName);
    sleep(1);
    showChannelScreen();
}

void RobotReadout::showChannelScreen() {
    _myDisplay->clearDisplay();
    _myDisplay->writeToDisplay((char *) "Channel Info:   x   ");
    _myDisplay->writeToDisplay((char *) "Pos: xxxxx Err: xxxx");
    _myDisplay->writeToDisplay((char *) "Velocity:  xxxx     ");
    _myDisplay->writeToDisplay((char *) "Refresh: xx ms      ");
   // _myDisplay->moveCursor(0, 2);
   // _myDisplay->writeToDisplay((char *) "asdfa");
}

void RobotReadout::updateChannelScreen(JOINTSTATESTRUCT * myJointData) {
    char myBuff[20];
    sprintf(myBuff, "Pos: %d", (int) myJointData->currentPosition);
    _myDisplay->moveCursor(0, 2);
    _myDisplay->writeToDisplay((char *) "tre");
}
