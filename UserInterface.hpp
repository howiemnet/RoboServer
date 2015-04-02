//
//  UserInterface.h
//  RoboServerA
//
//  Created by h on 24/03/2015.
//
//

#ifndef __RoboServerA__UserInterface__
#define __RoboServerA__UserInterface__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

int getch(void);
char waitForKey();
void menuPrintMain();

#endif /* defined(__RoboServerA__UserInterface__) */
