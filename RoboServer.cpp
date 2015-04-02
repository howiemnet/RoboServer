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
#include <sys/time.h>
#include <unistd.h>

#include "UserInterface.hpp"

#include "JointStateStruct.hpp"
#include "Robot.hpp"
#include "RobotReadout.hpp"
#include "CoordinatesHandler.hpp"
#include "PlaybackTimeHandler.hpp"
#include "SerialSetup.hpp"

#include <vector>

void manualControl(Robot * theRobot);
void doPIDTuning(Robot * theRobot);

bool dontQuitYet = true;
bool robotChecked = false;
bool robotHomed = false;

char myPorts[7][36];
uint8_t portMapping[7];

int main(int argc, const char * argv[])
{
    
    
    
    
    printf("Checking comms\n");
    bool success = getSerialPorts();
    
    for (int i=0; i < 6; i++) {
        printf("-- %s\n",myPorts[portMapping[i]]);
    }
    for (int i=0; i < 6; i++) {
        printf("-- %d\n",portMapping[i]);
    }
    
    //return(success);
    
    
    // ----------------------------
    //
    //   Load coordinates
    //
    // ----------------------------
    
    printf("Loading coordinates... \n");
    CoordinatesHandler * myCoordsHandler = new CoordinatesHandler((char *) "CoordinateList.csv", 1);
    if (!myCoordsHandler->load()) {
        printf(" +- Failed to load coords\n");
        return -1;
    } else {
        printf(" +- Loaded coords OK.\n");
    }
    
 /*   for (int i=0; i < 4000; i+=10) {
        printf("Coord at time %d should be %f\n",i,myCoordsHandler->getCoordinateAtTime(1, i));
    }
    return (0);
   */ // ----------------------------
    //
    //   Start the playback time handler
    //
    // ----------------------------
    
    
    PlaybackTimeHandler * myTimeHandler = new PlaybackTimeHandler();
    
    
    
    
    // ----------------------------
    //
    //   Initialise the robot
    //
    // ----------------------------
    
    
    
    
    
    
    
    printf("Loading robot... \n");

    
    Robot * myRobot = new Robot();
    robotChecked = myRobot->initComms();
    myRobot->linkTimeHandler(myTimeHandler);
    myRobot->linkCoordsHandler(myCoordsHandler);
    //myRobot->startRunning();
    
    menuPrintMain();
    
    
    while (dontQuitYet) {
        switch (waitForKey()) {
            case 't':
                // home robot
                doPIDTuning(myRobot);
                break;
            case 'h':
                // home robot
                robotHomed = myRobot->homeChannels();
                break;
            case 'w':
                // mark current positions as home for current sequence
               // robotHomed = myRobot->setOriginsToCurrentPlayback();
                break;
            case 'm':
                // manual control
                manualControl(myRobot);
                //myRobot->setStartPositions();
                //myRobot->startRunning();
                break;
            case 'p':
                // print positions
                myTimeHandler->printCurrentTime();
                //printf("Coordinate now: %f\n",myCoordsHandler->getCoordinateAtTime(myTimeHandler->getPlaybackTime()));
                myRobot->printPositions();
                
                break;
            case 'R':
                // run program
                myRobot->startRunning();
                //myTimeHandler->startPlayback();
                break;

            case 'r':
                // run program
                //myRobot->startRunning();
                myTimeHandler->startPlayback();
                break;
            case 's':
                // run program
                myRobot->stopRunning();//->stopPlayback();
                break;
            case 'x':
                // exit
                dontQuitYet = false;
                break;
            }
    }
}

void manualControl(Robot * theRobot) {
    bool finishManualControl = false;
    int stepSize = 500;
    theRobot->setVelocityLimits(50);
    while (!finishManualControl) {
        switch (waitForKey()) {
            case 'p':
                theRobot->printPositions();
                break;
            case 'y':
                theRobot->nudge(5,stepSize);
                break;
            case 'h':
                theRobot->nudge(5,-stepSize);
                break;
            case 't':
                theRobot->nudge(4,stepSize);
                break;
            case 'g':
                theRobot->nudge(4,-stepSize);
                break;
            case 'r':
                theRobot->nudge(3,stepSize);
                break;
            case 'f':
                theRobot->nudge(3,-stepSize);
                break;
            case 'e':
                theRobot->nudge(2,stepSize);
                break;
            case 'd':
                theRobot->nudge(2,-stepSize);
                break;
            case 'w':
                theRobot->nudge(1,stepSize);
                break;
            case 's':
                theRobot->nudge(1,-stepSize);
                break;
            case 'q':
                theRobot->nudge(0,stepSize);
                break;
            case 'a':
                theRobot->nudge(0,-stepSize);
                break;
            case 'Z':
                theRobot->setAllChannelOrigins();
                break;
            case 'z':
                theRobot->gotoZero();
                break;
            case ']':
                stepSize *= 2;
                printf("\nNudge size now: %d\n", stepSize);
                break;
            case '[':
                stepSize /= 2;
                printf("\nNudge size now: %d\n", stepSize);
                break;
            case 'x':
                finishManualControl = true;
                theRobot->setVelocityLimits(1000);

                break;
            default:
                break;
        }
    }
}


void doPIDTuning(Robot * theRobot) {
    bool finishTuningMode = false;
    int currentJointToTune = 0;
    char myKey;
    while (!finishTuningMode) {
        // select a joint
        printf("\n\nTUNING MODE...\n\nChoose a joint (1-6) or hit q to finish:\n");
        
        
        do {
            myKey = waitForKey();
        } while (((myKey < '1')||(myKey > '6')) && (myKey != 'q'));
        
        
        
        if (myKey == 'q') {
            finishTuningMode = true;
        } else {
            currentJointToTune = myKey - '1';
            bool finishWithThisJoint = false;
            
            printf("Joint: %d\n", currentJointToTune+1);
            
            PIDS currentPIDS;
            int stepSize = 16;
            
            do {
                currentPIDS = theRobot->myChannels[currentJointToTune]->getPIDS();
                printf("Current PIDS is P:%d, I:%d, D:%d, S:%d\n", currentPIDS.PTerm, currentPIDS.ITerm, currentPIDS.DTerm, currentPIDS.PIDScalar );
                printf("Step size: %d", stepSize);
                
                switch (waitForKey()) {
                    case 'q':
                        currentPIDS.PTerm += stepSize;
                        break;
                    case 'a':
                        currentPIDS.PTerm -= stepSize;
                        break;
                    case 'w':
                        currentPIDS.ITerm += stepSize;
                        break;
                    case 's':
                        currentPIDS.ITerm -= stepSize;
                        break;
                    case 'e':
                        currentPIDS.DTerm += stepSize;
                        break;
                    case 'd':
                        currentPIDS.DTerm -= stepSize;
                        break;
                    case 'r':
                        currentPIDS.PIDScalar += 1;
                        break;
                    case 'f':
                        currentPIDS.PIDScalar -= 1;
                        break;
                    case ']':
                        stepSize *= 2;
                        printf("\nNudge size now: %d\n", stepSize);
                        break;
                    case '[':
                        stepSize /= 2;
                        printf("\nNudge size now: %d\n", stepSize);
                        break;
                    case 'x':
                        finishWithThisJoint = true;
                        //theRobot->setVelocityLimits(1000);
                        break;
                    default:
                        break;
                }
                
                //theRobot->myChannels[currentJointToTune]->setPIDS(currentPIDS);
            } while (!finishWithThisJoint);
            
            
            
            
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