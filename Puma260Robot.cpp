//
//  Puma260Robot.cpp
//  RoboServerA
//
//  Created by h on 16/03/2015.
//
//


#ifndef RoboServerA_Puma260Robot_cpp
#define RoboServerA_Puma260Robot_cpp

#include <stdio.h>
#include "Puma260Robot.hpp"

jointSetupData jointData[] =
{
    [0] = {
        .enabled = false,
        .jointNumber = 1,
        .jointControllerType = JCT_MOTIONMIND,
        .controllerSerialAddress = (char *) "/dev/",
        .friendlyName = (char *) "Base",
        .PTerm = 200,
        .ITerm = 0,
        .DTerm = 0,
        .PIDScalar = 8,
        .minCount = 0,
        .maxCount = 10000
    },
    [1] = {
        .enabled = false,
        .jointNumber = 2,
        .jointControllerType = JCT_MOTIONMIND,
        .controllerSerialAddress = (char *) "/dev/",
        .friendlyName = (char *) "Shoulder",
        .PTerm = 200,
        .ITerm = 0,
        .DTerm = 0,
        .PIDScalar = 8,
        .minCount = 0,
        .maxCount = 10000
    },
    [2] = {
        .enabled = false,
        .jointNumber = 3,
        .jointControllerType = JCT_MOTIONMIND,
        .controllerSerialAddress = (char *) "/dev/",
        .friendlyName = (char *) "Elbow",
        .PTerm = 200,
        .ITerm = 0,
        .DTerm = 0,
        .PIDScalar = 8,
        .minCount = 0,
        .maxCount = 10000
    },
    [3] = {
        .enabled = true,
        .jointNumber = 4,
        .jointControllerType = JCT_MOTIONMIND,
        .controllerSerialAddress = (char *) "/dev/cu.usbserial-A50285BI",
        .friendlyName = (char *) "Wrist-twist",
        .PTerm = 200,
        .ITerm = 0,
        .DTerm = 0,
        .PIDScalar = 8,
        .minCount = 0,
        .maxCount = 10000
    },
    [4] = {
        .enabled = false,
        .jointNumber = 5,
        .jointControllerType = JCT_MOTIONMIND,
        .controllerSerialAddress = (char *) "/dev/",
        .friendlyName = (char *) "Wrist-tilt",
        .PTerm = 200,
        .ITerm = 0,
        .DTerm = 0,
        .PIDScalar = 8,
        .minCount = 0,
        .maxCount = 10000
    },
    [5] = {
        .enabled = false,
        .jointNumber = 6,
        .jointControllerType = JCT_MOTIONMIND,
        .controllerSerialAddress = (char *) "/dev/",
        .friendlyName = (char *) "End-effector-twist",
        .PTerm = 200,
        .ITerm = 0,
        .DTerm = 0,
        .PIDScalar = 8,
        .minCount = 0,
        .maxCount = 10000
    },
    
};

#endif
