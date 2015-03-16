//
//  CoordinatesLoader.cpp
//  RoboServerA
//
//  Created by h on 11/03/2015.
//
//

#include "CoordinatesHandler.hpp"
#include <iostream>
#include <istream>
#include <sstream>


#include <fstream>
#include <string>
#include <math.h>

CoordinatesHandler::CoordinatesHandler(char * theFilename, int howManyChannels) {
    _filename = theFilename;
    _channelCount = howManyChannels;
    _numberOfCoordinates = 0;

}

bool CoordinatesHandler::load() {
    _isLoaded = false;
    //using namespace std;
    int myCount = 0;
    std::string line;
    std::ifstream myFile (_filename);
    if (myFile.is_open())
    {
        myCount = 0;
        while ( getline (myFile,line) )
        {
            std::istringstream split(line);
            float value;
            char sep;
            _myCoords.push_back(std::vector<float>());
            while (split >> value) //
            {
                _myCoords[myCount].push_back(value);
                split>>sep;
            }
            ++myCount;
        }
        _numberOfCoordinates = myCount;
        _endTime = _myCoords[myCount-1][0];
        printf("End time is %f\n", _endTime);
        myFile.close();
        return true;
    }
    else
    {
        printf("Couldn't open file\n");
        return false;
    }
}

float CoordinatesHandler::getCoordinateAtTime(int theChannel, long theTime) {
    // returns smoothed coordinate at any given time
    int myIndex = (int) (theTime / 40);
    if (myIndex < 0) {
        myIndex = 0;
    }
    if (myIndex >= _numberOfCoordinates) {
        myIndex = (int) _numberOfCoordinates -1;
    }
    // got nearest coordinate in time; do smoothing / interpolation:
    //
    
    // nearest value:
    float nearestCoord = _myCoords[myIndex][theChannel];
    
    // fraction of time we need to interpolate:
    float partialTimeSlice = (theTime - (myIndex * 40))/40.0f;
    
    // check what sort of interpolation is valid (CR is only valid if we have 4 points)
    if ((myIndex > 0) && (myIndex < (_numberOfCoordinates-3)))
    {
        // we can do CR:
        return (getCRInterpolated(_myCoords[myIndex-1][theChannel], _myCoords[myIndex][theChannel], _myCoords[myIndex+1][theChannel], _myCoords[myIndex+2][theChannel], partialTimeSlice));
    }
    else
    {
        // can't do CR, try linear:
        if (myIndex < (_numberOfCoordinates-2)) {
            float myDelta = _myCoords[myIndex+1][theChannel] - nearestCoord;
            nearestCoord += (myDelta * (partialTimeSlice / 40));
        }
        return nearestCoord;
    }
    


}

float CoordinatesHandler::getEndTime() {
    return _endTime;
}

bool CoordinatesHandler::isLoaded() {
    return _isLoaded;
}


double CoordinatesHandler::interpolateSingleCR(double* p, double* time, double t) {
    double L01 = p[0] * (time[1] - t) / (time[1] - time[0]) + p[1] * (t - time[0]) / (time[1] - time[0]);
    double L12 = p[1] * (time[2] - t) / (time[2] - time[1]) + p[2] * (t - time[1]) / (time[2] - time[1]);
    double L23 = p[2] * (time[3] - t) / (time[3] - time[2]) + p[3] * (t - time[2]) / (time[3] - time[2]);
    double L012 = L01 * (time[2] - t) / (time[2] - time[0]) + L12 * (t - time[0]) / (time[2] - time[0]);
    double L123 = L12 * (time[3] - t) / (time[3] - time[1]) + L23 * (t - time[1]) / (time[3] - time[1]);
    double C12 = L012 * (time[2] - t) / (time[2] - time[1]) + L123 * (t - time[1]) / (time[2] - time[1]);
    return C12;
}






void CoordinatesHandler::_InitCubicPoly(float x0, float x1, float t0, float t1, CubicPoly &p)
{
    p.c0 = x0;
    p.c1 = t0;
    p.c2 = -3*x0 + 3*x1 - 2*t0 - t1;
    p.c3 = 2*x0 - 2*x1 + t0 + t1;
}

// standard Catmull-Rom spline: interpolate between x1 and x2 with previous/following points x1/x4
// (we don't need this here, but it's for illustration)
void CoordinatesHandler::_InitCatmullRom(float x0, float x1, float x2, float x3, CubicPoly &p)
{
    // Catmull-Rom with tension 0.5
    _InitCubicPoly(x1, x2, 0.5f*(x2-x0), 0.5f*(x3-x1), p);
}

// compute coefficients for a nonuniform Catmull-Rom spline
void CoordinatesHandler::_InitNonuniformCatmullRom(float x0, float x1, float x2, float x3, float dt0, float dt1, float dt2, CubicPoly &p)
{
    // compute tangents when parameterized in [t1,t2]
    float t1 = (x1 - x0) / dt0 - (x2 - x0) / (dt0 + dt1) + (x2 - x1) / dt1;
    float t2 = (x2 - x1) / dt1 - (x3 - x1) / (dt1 + dt2) + (x3 - x2) / dt2;
    
    // rescale tangents for parametrization in [0,1]
    t1 *= dt1;
    t2 *= dt1;
    
    _InitCubicPoly(x1, x2, t1, t2, p);
}

float CoordinatesHandler::_VecDistSquared(const Vec2D& p, const Vec2D& q)
{
    float dx = q.x - p.x;
    float dy = q.y - p.y;
    return dx*dx + dy*dy;
}

void CoordinatesHandler::_InitCentripetalCR(const Vec2D& p0, const Vec2D& p1, const Vec2D& p2, const Vec2D& p3,
                       CubicPoly &px, CubicPoly &py)
{
    float dt0 = powf(_VecDistSquared(p0, p1), 0.25f);
    float dt1 = powf(_VecDistSquared(p1, p2), 0.25f);
    float dt2 = powf(_VecDistSquared(p2, p3), 0.25f);
    
    // safety check for repeated points
    if (dt1 < 1e-4f)    dt1 = 1.0f;
    if (dt0 < 1e-4f)    dt0 = dt1;
    if (dt2 < 1e-4f)    dt2 = dt1;
    
    _InitNonuniformCatmullRom(p0.x, p1.x, p2.x, p3.x, dt0, dt1, dt2, px);
    _InitNonuniformCatmullRom(p0.y, p1.y, p2.y, p3.y, dt0, dt1, dt2, py);
}


float CoordinatesHandler::getCRInterpolated(float a, float b, float c, float d, float fraction)
{
    Vec2D p0(0,a), p1(1,b), p2(2,c), p3(3,d);
    CubicPoly px, py;
    _InitCentripetalCR(p0, p1, p2, p3, px, py);
    return py.eval(fraction);
}


