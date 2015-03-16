//
//  PlaybackTimeHandler.h
//  RoboServerA
//
//  Created by h on 16/03/2015.
//
//

#ifndef __RoboServerA__PlaybackTimeHandler__
#define __RoboServerA__PlaybackTimeHandler__

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#define PTH_STOPPED 0x00
#define PTH_PLAYING 0x01


class PlaybackTimeHandler {
    int     _currentState = PTH_STOPPED;
    double  _startTime; // in mSecs
    double  _timeInUSecs();
    
public:
    
    long   getPlaybackTime();
    void    startPlayback();
    void    stopPlayback();
    void    printCurrentTime();
};









#endif /* defined(__RoboServerA__PlaybackTimeHandler__) */
