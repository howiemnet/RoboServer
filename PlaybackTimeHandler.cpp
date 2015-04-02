//
//  PlaybackTimeHandler.cpp
//  RoboServerA
//
//  Created by h on 16/03/2015.
//
//

#include "PlaybackTimeHandler.hpp"



double PlaybackTimeHandler::_timeInUSecs() {
    timeval currentTime;
    gettimeofday(&currentTime, NULL);
   return (currentTime.tv_sec * 1000) + (currentTime.tv_usec / 1000);
}



long   PlaybackTimeHandler::getPlaybackTime() {
    if (_currentState == PTH_PLAYING) {
        return (_timeInUSecs() - _startTime)*0.5;
    } else {
        return -1;
    }
}


void    PlaybackTimeHandler::startPlayback() {
    _startTime = _timeInUSecs();
    _currentState = PTH_PLAYING;
}


void    PlaybackTimeHandler::stopPlayback() {
    _currentState = PTH_STOPPED;
}

void PlaybackTimeHandler::printCurrentTime() {
    printf("CurrentTime %ld\n", getPlaybackTime());
}