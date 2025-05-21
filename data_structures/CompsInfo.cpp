#define COMPS_LENGTH 8

#include <math.h> 
#include <iostream>
#include <string>

#include "CompsInfo.hpp"

/***************************************************/
/************* Output stream ************************/
/***************************************************/

ostream& operator << (ostream &out, const CompsInfo &e){
	std::string compNames[COMPS_LENGTH] = {"TakeOff", "Sticks", "Knobs", "WP", "NMPC", "DAA", "PilotTakeover", "Landing"};
    for (int i = 0; i < COMPS_LENGTH; i++){
        out << compNames[i];
        if (e.comps.at(i) == 0){
            out << " is off,";
        } else if (e.comps.at(i) == -1){
             out << " is broken,";
        } else {
            if (i != 7){
                if (e.comps.at(i) == 1){
                    out << " is on,";
                } else {
                    out << " is not in a valid e,";
                }
            } else {
                if (e.comps.at(i) == 1){
                    out << " is landing with no-hover,";
                } else if (e.comps.at(i) == 2){
                    out << " is landing with hover,";
                } else {
                    out << " is not in a valid e,";
                }
            }
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