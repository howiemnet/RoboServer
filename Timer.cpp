//
//  Timer.cpp
//  RoboServerA
//
//  Created by h on 09/03/2015.
//
//

#include "Timer.hpp"




    
    void Timer::start(){
        gettimeofday(&startTime, NULL);
    }
    
    double Timer::stop(){
        timeval endTime;
        long seconds = 0;
        long useconds = 0;
        double duration;
        
        gettimeofday(&endTime, NULL);
        
        seconds  = endTime.tv_sec  - startTime.tv_sec;
        useconds = endTime.tv_usec - startTime.tv_usec;
        
        duration = seconds + useconds/1000000.0;
        
        return duration;
    }
    
   void Timer::printTime(double duration){
        printf("%5.6f seconds\n", duration);
    }

