//
//  UserInterface.cpp
//  RoboServerA
//
//  Created by h on 24/03/2015.
//
//

#include "UserInterface.hpp"



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

void menuPrintMain() {
    printf("\n\nMAIN MENU\n\n");
    printf("[c] Check communications");
    printf("[h] Home robot\n");
    printf("[p] Print positions\n");
    printf("[r] Run program\n");
    printf("[x] Exit\n");
    
}