//
//  CoordinatesHandler.h
//  RoboServerA
//
//  Created by h on 11/03/2015.
//
// Coordinates file should be a CSV file, no header:
// (float) time, (float) angle_1, (float) angle_2, ...
//

#ifndef __RoboServerA__CoordinatesHandler__
#define __RoboServerA__CoordinatesHandler__

#include <stdio.h>
#include <vector>


using std::vector;

class CoordinatesHandler {
    char * _filename;
    int _channelCount;
    float _frameTime; // in ms: so 25fps == 40ms
    bool _isLoaded;
    
    // coords
    long _numberOfCoordinates;
    vector<vector<float> > _myCoords;
    
    double interpolateSingleCR(double* p, double* time, double t);
    
    
public:
    CoordinatesHandler(char * filename, int channels);
    float getCoordinateAtTime(float theTime);
    bool load();
    bool isLoaded();
    
};

#endif /* defined(__RoboServerA__CoordinatesHandler__) */
