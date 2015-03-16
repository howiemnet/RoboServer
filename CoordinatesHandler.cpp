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
        myFile.close();
        return true;
    }
    else
    {
        printf("Couldn't open file\n");
        return false;
    }
}

float CoordinatesHandler::getCoordinateAtTime(float theTime) {
    // returns smoothed coordinate at any given time
    if (_isLoaded) {
        // get value
        return 1.0;
    } else {
        return 0.0;
    }
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

/*
public static List<Coord> CoordinatesHandler::interpolate(List<Coord> points, int index, int pointsPerSegment, CatmullRomType curveType) {
    List<Coord> result = new ArrayList<>();
    double[] x = new double[4];
    double[] y = new double[4];
    double[] time = new double[4];
    for (int i = 0; i < 4; i++) {
        x[i] = points.get(index + i).X;
        y[i] = points.get(index + i).Y;
        time[i] = i;
    }
    
    double tstart = 1;
    double tend = 2;
    if (!curveType.equals(CatmullRomType.Uniform)) {
        double total = 0;
        for (int i = 1; i < 4; i++) {
            double dx = x[i] - x[i - 1];
            double dy = y[i] - y[i - 1];
            if (curveType.equals(CatmullRomType.Centripetal)) {
                total += Math.pow(dx * dx + dy * dy, .25);
            } else {
                total += Math.pow(dx * dx + dy * dy, .5);
            }
            time[i] = total;
        }
        tstart = time[1];
        tend = time[2];
    }
    double z1 = 0.0;
    double z2 = 0.0;
    if (!Double.isNaN(points.get(index + 1).Z)) {
        z1 = points.get(index + 1).Z;
    }
    if (!Double.isNaN(points.get(index + 2).Z)) {
        z2 = points.get(index + 2).Z;
    }
    double dz = z2 - z1;
    int segments = pointsPerSegment - 1;
    result.add(points.get(index + 1));
    for (int i = 1; i < segments; i++) {
        double xi = interpolate(x, time, tstart + (i * (tend - tstart)) / segments);
        double yi = interpolate(y, time, tstart + (i * (tend - tstart)) / segments);
        double zi = z1 + (dz * i) / segments;
        result.add(new Coord(xi, yi, zi));
    }
    result.add(points.get(index + 2));
    return result;
}

*/




