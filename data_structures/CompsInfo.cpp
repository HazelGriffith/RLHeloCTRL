#define COMPS_LENGTH 9

#include <math.h> 
#include <iostream>
#include <string>

#include "CompsInfo.hpp"

/***************************************************/
/************* Output stream ************************/
/***************************************************/

ostream& operator << (ostream &out, const CompsInfo &e){
	std::string compNames[COMPS_LENGTH] = {"TakeOff", "Sticks", "Knobs", "WP", "NMPC", "DAA", "PilotTakeover", "Landing/NoHover", "Landing/Hover"};
    for (int i = 0; i < COMPS_LENGTH; i++){
        out << compNames[i];
        int compState = e.comps.at(i);
        if (compState == 0){
            out << " is off,";
        } else if (compState == -1){
            out << " is broken,";
        } else if (compState == 1){
            out << " is on,";
        } else {
            out << " is not in a valid state,";
        }
    }
	return out;
}

/***************************************************/
/************* Input stream ************************/
/***************************************************/

istream& operator >> (istream &in, CompsInfo &e){
    std::string tempStr;
	for (int i = 0; i < COMPS_LENGTH; i++){
        in >> tempStr;
        e.comps.push_back(std::stoi(tempStr)); 
    }
	
	return in;
}