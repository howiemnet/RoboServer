//
//  JointStateStruct.hpp
//  RoboServerA
//
//  Created by h on 12/03/2015.
//
//

#ifndef RoboServerA_JointStateStruct_hpp
#define RoboServerA_JointStateStruct_hpp
struct JOINTSTATESTRUCT {
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
    
};

#endif
