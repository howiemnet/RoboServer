//
//  Timer.h
//  RoboServerA
//
//  Created by h on 09/03/2015.
//
//

#ifndef __RoboServerA__Timer__
#define __RoboServerA__Timer__


#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

class Timer {
private:
    
    timeval startTime;
    
public:
    
    void start();
    
    double stop();
    
    static void printTime(double duration);

};




#endif /* defined(__RoboServerA__Timer__) */
