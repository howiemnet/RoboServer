//
//  serialStuff.c
//  SerialPortSample
//
//  Created by h on 09/03/2015.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#include "JointStateStruct.hpp"
#include "Robot.hpp"
#include "RobotReadout.hpp"
#include "CoordinatesHandler.hpp"


int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}


char waitForKey() {
    char theHitKey = ' ';
    while (theHitKey == ' ') {
        theHitKey = getch();
    }
    return theHitKey;
}


bool runProgram() {
    printf("Loading coordinates... \n");
    CoordinatesHandler * myCoordsHandler = new CoordinatesHandler((char *) "CoordinateList.csv", 1);
    if (!myCoordsHandler->load()) {
        printf("Failed to load coords\n");
        return -1;
    } else {
        printf("Loaded OK.");
    }
    printf("Hit a key to start running...\n");
    return true;
}

void menuPrintMain() {
    printf("\n\nMAIN MENU\n\n");
    printf("[c] Check robot comms\n");
    printf("[h] Home robot\n");
    printf("[r] Run program\n");
    printf("[x] Exit\n");
    
}


bool dontQuitYet = true;
bool robotChecked = false;
bool robotHomed = false;

int main(int argc, const char * argv[])
{

    Robot * myRobot = new Robot();
    
    while (dontQuitYet) {
        menuPrintMain();
        switch (waitForKey()) {
            case 'c':
                // Check robot
                robotChecked = myRobot->initComms();
                break;
            case 'h':
                // home robot
                robotHomed = myRobot->homeChannels();
                break;
            case 'r':
                // run program
                runProgram();
                break;
            case 'x':
                // exit
                dontQuitYet = false;
                break;
            }
    }
}



    
//    RobotReadout * myReadout = new RobotReadout((char *) "/dev/cu.usbmodem3a21");
    
    
    
    
    
    /*
    //myRobot->testCommsCycleTime();
    
    
    // test threads...
    myRobot->startRunning();
    // start running thread
    */
  /*  char myCommand = 'r';
    
    while (myCommand != 'x') {
        myCommand = waitForKey();
        if (myCommand == 's') {
            myReadout->showChannelScreen(myRobot->getJointData(0));
        }
        
    }
    
    //myRobot->stopRunning();
    
    myCommand = 'r';
    
    while (myCommand != 'x') {
        myCommand = waitForKey();
        if (myCommand == 's') {
     //       myRobot->printPositions();
        }
        
    }
    
   
   
    
    
    return 0;
}

*/



/*
 
 
 MM: Check
     Home
     Run prog
     Live
 
 bool commsChecked = false;
 bool robotHomed = false;
 
 Check:
 - init robot check for OK from all serial drivers
 
 Home:
 - Go through each axis, calling home
 
 Run prog:
 Menu: Load coords
        Move to start
        Run
        Rewind
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
*/