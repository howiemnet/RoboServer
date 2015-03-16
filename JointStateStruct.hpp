//
//  JointStateStruct.hpp
//  RoboServerA
//
//  Created by h on 12/03/2015.
//
//

#ifndef RoboServerA_JointStateStruct_hpp
#define RoboServerA_JointStateStruct_hpp



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
    long PTerm;
    long ITerm;
    long DTerm;
    long PIDScalar;
    //
    //
    //
    long minCount;
    long maxCount;
    
    
} jointSetupData;




#endif
