//
//  SerialSetup.h
//  RoboServerA
//
//  Created by h on 24/03/2015.
//
//

#ifndef __RoboServerA__SerialSetup__
#define __RoboServerA__SerialSetup__

#include <stdio.h>
#include <stdint.h>

extern char myPorts[7][36];
extern uint8_t portMapping[7];


int doSerialSetup();
bool getSerialPorts();



#endif /* defined(__RoboServerA__SerialSetup__) */
