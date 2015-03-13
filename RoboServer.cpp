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

void showMenu() {
    printf("\033[2J\033[5;5HRobo Server MAIN MENU\n");
    printf("[t] Test serial throughput\n");
    printf("[v] Check Validiti of Chess Notation\n");
    printf("[s] Save Moves to e Text File\n");
    printf("[e] End program\n");
    printf("\033[1;37mSelection [d, v, s, f, e]: ");
    
}


char waitForKey() {
    char theHitKey = ' ';
    while (theHitKey == ' ') {
        
        theHitKey = getch();
        
        
    }
    return theHitKey;
    
}



int main(int argc, const char * argv[])
{
    printf("Starting!\n");
    printf("Loading coordinates... \n");
    
    
    CoordinatesHandler * myCoordsHandler = new CoordinatesHandler((char *) "CoordinateList.csv", 1);
    
    if (!myCoordsHandler->load()) {
        printf("Failed to load coords\n");
        return -1;
    } else {
        printf("Loaded OK.");
    }
    
   
    
    printf("Hit a key to start running...\n");
    
    
    RobotReadout * myReadout = new RobotReadout((char *) "/dev/cu.usbmodem3a21");
    
    
    
    
    
    Robot * myRobot = new Robot();
    /*
    //myRobot->testCommsCycleTime();
    
    
    // test threads...
    myRobot->startRunning();
    // start running thread
    */
    char myCommand = 'r';
    
    while (myCommand != 'x') {
        myCommand = waitForKey();
        if (myCommand == 's') {
            myReadout->updateChannelScreen(myRobot->getJointData(0));
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
    
    
    /*
    //
    
    myDisplay->writeToDisplay((char *) "TESTING!!!\0");
    sleep(2);
    myDisplay->clearDisplay();
    myDisplay->writeToDisplay((char *) "TESTING!!!\0");
    
    
    
    for (int i = 0; i < 10; i++) {
        myRobot->printPositions();
        long x = myRobot->getSingleJointPosition(0);
        char buffer[10];
        sprintf(buffer,"%ld",x);
        myDisplay->writeToDisplay(buffer);
        sleep(0.5);
     
    }*/
    
    
    
    return 0;
    
    
    //myDrivers[0] = new SerialMotorDriver((char *) "/dev/cu.usbserial-A50285BI", 1);
    //myDrivers[0]->initDriver();
    //showMenu();
    //return 0; //mainUDP();
    //return serialStuff();
}