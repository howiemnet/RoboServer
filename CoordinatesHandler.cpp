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

}

bool CoordinatesHandler::load() {
    _isLoaded = false;
    //using namespace std;
    int myCount = 0;
    std::string line;
    std::ifstream myFile (_filename);
    if (myFile.is_open())
    {
        while ( getline (myFile,line) )
        {
            myCount++;
        }
        printf("Lines read: %d", myCount);
        
        //
        // now we know how many coords we need to load
        //
        
        _numberOfCoordinates = myCount;
        //_myCoords.resize(myCount);
        //for (int i = 0; i < myCount; ++i)
        //    _myCoords[i].resize(_channelCount+1);
        
        // Put some values in
        //_myCoords[1][2] = 6.0;
        
        //
        // so go back to start
        //
        
        myFile.clear();
        myFile.seekg(0, std::ios::beg);
        myCount = 0;
        
        //
        // and parse each line
        //
        
        while ( getline (myFile,line) )
        {
            
            
            
            std::istringstream split(line);
            float value;
            
            char sep;
            _myCoords.push_back(std::vector<float>());
            while (split >> value) //
            {
                /*
                 if(!first)
                 {
                 // Each new value read on line 1 should create a new inner vector
                 vec.push_back(std::vector<float>());
                 }
                 */
                
                _myCoords[myCount].push_back(value);
                
                /* ++col; */
                
                // read past the separator
                split>>sep;
            }
            
            // Finished reading line 1 and creating as many inner
            // vectors as required
            /* first = true; */
            ++myCount;
            
            
        }
        
        
        
        myFile.close();
        
        // test:
        printf("line %f is %f",  _myCoords[30][0],  _myCoords[30][1]);
        return true;
    } else {
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