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

#include <iostream>
#include <cmath>
#include <vector>

struct CubicPoly
{
    float c0, c1, c2, c3;
    
    float eval(float t)
    {
        float t2 = t*t;
        float t3 = t2 * t;
        return c0 + c1*t + c2*t2 + c3*t3;
    }
};



struct Vec2D
{
    Vec2D(float _x, float _y) : x(_x), y(_y) {}
    float x, y;
};



using std::vector;

class CoordinatesHandler {
    char * _filename;
    int _channelCount;
    float _frameTime; // in ms: so 25fps == 40ms
    bool _isLoaded;
    
    // coords
    long _numberOfCoordinates;
    float _endTime;
    vector<vector<float> > _myCoords;
    
    double interpolateSingleCR(double* p, double* time, double t);
    void _InitCubicPoly(float x0, float x1, float t0, float t1, CubicPoly &p);
    void _InitCatmullRom(float x0, float x1, float x2, float x3, CubicPoly &p);
    void _InitNonuniformCatmullRom(float x0, float x1, float x2, float x3, float dt0, float dt1, float dt2, CubicPoly &p);
    float _VecDistSquared(const Vec2D& p, const Vec2D& q);
    void _InitCentripetalCR(const Vec2D& p0, const Vec2D& p1, const Vec2D& p2, const Vec2D& p3, CubicPoly &px, CubicPoly &py);
    
    
public:
    CoordinatesHandler(char * filename, int channels);
    float getEndTime();
    float getCoordinateAtTime(int theChannel, long theTime);
    bool load();
    bool isLoaded();
    float getCRInterpolated(float a, float b, float c, float d, float fraction);

    
};

#endif /* defined(__RoboServerA__CoordinatesHandler__) */
