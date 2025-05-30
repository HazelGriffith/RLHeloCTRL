#define COMPS_LENGTH 9

#include <math.h> 
#include <iostream>
#include <string>

#include "CompsInfo.hpp"

/***************************************************/
/************* Output stream ************************/
/***************************************************/

ostream& operator << (ostream &out, const CompsInfo &e){
	std::string compNames[COMPS_LENGTH] = {"TakeOff", "Sticks", "Knobs", "WP", "NMPC", "DAA", "PilotTakeover", "LandingNoHover", "LandingHover"};
    for (int i = 0; i < COMPS_LENGTH; i++){
        out << compNames[i];
        int compState = e.comps.at(i);
        switch(compState){
				case 0:
					out << " is off,";
					break;
				case -1:
					out << " is broken,";
					break;
				case 1:
					out << " is on,";
					break;
				default:
					out << " is not in a valid state,";
					break;
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