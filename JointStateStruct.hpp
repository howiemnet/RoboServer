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
    /*JOINTSTATESTRUCT() {
       currentPosition = 0;
       currentPositionTimeStamp = 0;
       currentLatency = 0;
       positionalError = 0;
       currentVelocity = 0;
    }*/
    
} JOINTSTATESTRUCT;


typedef struct {
    //
    char *  portName;
    int     jointNumber;
    long    minCount;
    long    maxCount;
    
    
    // DH parameters
    double d;
    double a;
    double alpha;
    double angle; // offset from "0" in the DH map
    
    // Mechanical details about the joint
    double coupled; // amount this axis is cross coupled to the previous
    double scale; // rads to counter ticks
    int min;
    int max;
} joint_cfg_t;




#endif
