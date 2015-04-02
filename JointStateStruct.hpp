//
//  JointStateStruct.hpp
//  RoboServerA
//
//  Created by h on 12/03/2015.
//
//

#ifndef RoboServerA_JointStateStruct_hpp
#define RoboServerA_JointStateStruct_hpp
#include <stdint.h>

typedef struct {
    int16_t PTerm;
    int16_t ITerm;
    int16_t DTerm;
    int8_t PIDScalar;

} PIDS;

typedef struct {
    volatile signed long     currentPosition;
    volatile long long       currentPositionTimeStamp ;
    volatile int             currentLatency ;
    volatile signed long     positionalError;
    volatile signed long     currentVelocity;
  
} JOINTSTATESTRUCT;

#define JCT_MOTIONMIND 0x01


typedef struct {
    bool enabled;
    int jointNumber;
    int jointControllerType;
    char * controllerSerialAddress;
    char * friendlyName;
    //
    //  driver setup
    //
   
    PIDS jointPIDS;
    
//    int16_t PTerm;
//    int16_t ITerm;
//    int16_t DTerm;
//    int8_t PIDScalar;
    //
    //
    //
    long minCount;
    long maxCount;
    long scale;
    long countOffset;
    int  milliAmpsLimit;
    
} jointSetupData;


#endif
