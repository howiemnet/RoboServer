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
        .enabled = true,
        .jointNumber = 1,
        .jointControllerType = JCT_MOTIONMIND,
        .controllerSerialAddress = (char *) "/dev/tty.usbserial-A50285BI",
        .friendlyName = (char *) "Base",
        .jointPIDS.PTerm = 200,
        .jointPIDS.ITerm = 0,
        .jointPIDS.DTerm = 0,
        .jointPIDS.PIDScalar = 10,
        .minCount = -160000,
        .maxCount = 160000,
        .scale = -233600,
        .milliAmpsLimit = 3500
    },
    [1] = {
        .enabled = true,
        .jointNumber = 2,
        .jointControllerType = JCT_MOTIONMIND,
        .controllerSerialAddress = (char *) "/dev/tty.usbserial-A50285BI17",
        .friendlyName = (char *) "Shoulder",
        .jointPIDS.PTerm = 200,
        .jointPIDS.ITerm = 0,
        .jointPIDS.DTerm = 0,
        .jointPIDS.PIDScalar = 10,
        .minCount = 0,
        .maxCount = 10000,
        .scale = -349866,
        .milliAmpsLimit = 3500
    },
    [2] = {
        .enabled = true,
        .jointNumber = 3,
        .jointControllerType = JCT_MOTIONMIND,
        .controllerSerialAddress = (char *) "/dev/tty.usbserial-A50285BI18",
        .friendlyName = (char *) "Elbow",
        .jointPIDS.PTerm = 200,
        .jointPIDS.ITerm = 0,
        .jointPIDS.DTerm = 0,
        .jointPIDS.PIDScalar = 11,
        .minCount = 0,
        .maxCount = 10000,
        .scale = -214933,
        .milliAmpsLimit = 3500
    },
    [3] = {
        .enabled = true,
        .jointNumber = 4,
        .jointControllerType = JCT_MOTIONMIND,
        .controllerSerialAddress = (char *) "/dev/tty.usbserial-A50285BI19",
        .friendlyName = (char *) "Wrist-twist",
        .jointPIDS.PTerm = 400,
        .jointPIDS.ITerm = 4,
        .jointPIDS.DTerm = 8,
        .jointPIDS.PIDScalar = 10,
        .minCount = 0,
        .maxCount = 16000,
        .scale = 21755,
        .milliAmpsLimit = 2500
    },
    [4] = {
        .enabled = true,
        .jointNumber = 5,
        .jointControllerType = JCT_MOTIONMIND,
        .controllerSerialAddress = (char *) "/dev/tty.usbserial-A50285BI20",
        .friendlyName = (char *) "Wrist-tilt",
        .jointPIDS.PTerm = 600,
        .jointPIDS.ITerm = 4,
        .jointPIDS.DTerm = 8,
        .jointPIDS.PIDScalar = 10,
        .minCount = 0,
        .maxCount = 19692,
        .milliAmpsLimit = 2500
    },
    [5] = {
        .enabled = true,
        .jointNumber = 6,
        .jointControllerType = JCT_MOTIONMIND,
        .controllerSerialAddress = (char *) "/dev/tty.usbserial-A50285BI21",
        .friendlyName = (char *) "End-effector-twist",
        .jointPIDS.PTerm = 400,
        .jointPIDS.ITerm = 2,
        .jointPIDS.DTerm = 4,
        .jointPIDS.PIDScalar = 10,
        .minCount = 0,
        .maxCount = 15885,
        .milliAmpsLimit = 2500
    },
    
};

#endif
