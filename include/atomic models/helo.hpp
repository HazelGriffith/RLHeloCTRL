#ifndef __HELO_HPP__
#define __HELO_HPP__
#define COMPS_LENGTH 9
#define PROB_OF_BREAK 10
#define PROB_OF_MULTICOMPS 33
#define PROB_OF_BAD_TRANSITION 25
#define FAULT_FREQ 25


#include "cadmium/modeling/devs/atomic.hpp"
#include "../CompsInfo.hpp"
#include <iostream>
#include <vector>
#include <cassert>
#include <random>
#include <chrono>

using namespace std;

namespace cadmium::SimpleHelo {
	/* State of the helicopter, where each int in the vector array indicates the status of a componenent
    comps[0] = TakeOff
    comps[1] = Sticks
    comps[2] = Knobs
    comps[3] = WP
    comps[4] = NMPC
    comps[5] = DAA
    comps[6] = LandingNoHover
    comps[7] = LandingHover
	comps[8] = PilotTakeover
	values: -1 = broken, 0 = off, 1 = on
    */
	struct HeloState {

		double sigma;

		// int vector tracks the status of each component
		vector<int> currComps;

		// int vector keeps last state of components
		vector<int> prevComps;

		// These booleans indicate the presence of faults

		// True if multiple components are active simultaneously
		bool multiComps;

		// True if a component is considered broken
		bool brokenComp;

		// True if a bad transition has occurred and not been fixed
		bool badTransition;

		// True if any fault has occurred
		bool fault;

		
		// Set the default values for the state constructor for this specific model
		HeloState(): sigma(0), currComps({1,0,0,0,0,0,0,0,0}), prevComps({1,0,0,0,0,0,0,0,0}), multiComps(false), brokenComp(false), badTransition(false), fault(false) {};
	};

	std::ostream& operator<<(std::ostream &out, const HeloState& state) {
		string compNames[COMPS_LENGTH] = {"TakeOff", "Sticks", "Knobs", "WP", "NMPC", "DAA", "LandingNoHover", "LandingHover", "PilotTakeover"};
		for (int i = 0; i < COMPS_LENGTH; i++){
			out << compNames[i];
			int compState = state.currComps.at(i);
			switch(compState){
				case 0:
					out << " is off ";
					break;
				case -1:
					out << " is broken ";
					break;
				case 1:
					out << " is on ";
					break;
				default:
					out << " is not in a valid state ";
					break;
			}
		}
		if (state.multiComps){
			out << " multiple components are on ";
		}
		if (state.brokenComp){
			out << " a component is broken ";
		}
		if (state.badTransition){
			out << " a bad transition has occurred ";
		}
		out << "<";
		for (int i = 0; i < COMPS_LENGTH; i++){
			out << state.currComps.at(i);
			if (i != COMPS_LENGTH-1){
				out << ";";
			}
		}
		out << ">";
		return out;
	}

	// Atomic model of Helo
	class Helo: public Atomic<HeloState> {
		private:

		public:

			// Declare ports for the model

			// Input ports
			Port<CompsInfo> in;

			// Output ports
			Port<CompsInfo> out;

			/**
			 * Constructor function for this atomic model, and its respective state object.
			 *
			 * A Helo object is created with a HeloState struct
			 *
			 * @param id ID of the new Helo model object, will be used to identify results on the output file
			 */
			Helo(const string& id): Atomic<HeloState>(id, HeloState()) {

				// Initialize ports for the model

				// Input Ports
				in  = addInPort<CompsInfo>("in");

				// Output Ports
				out = addOutPort<CompsInfo>("out");
				
				state.sigma = FAULT_FREQ;
				
			}

			/**
			 * The transition function is invoked each time the value of
			 * state.sigma reaches 0.
			 *
			 * In each internal transition the model generates a random number to determine if a fault has occurred with any component
			 * If a fault has occurred, then a random component is selected to be in the "broken" state
			 * If the selected component is already "broken" then it is set to "off"
			 *
			 * @param state reference to the current state of the model.
			 */
			void internalTransition(HeloState& state) const override {
				unsigned seed1 = chrono::system_clock::now().time_since_epoch().count();
				minstd_rand0 generator(seed1);
                uniform_int_distribution<> probDist(1,100);
                uniform_int_distribution<> compDist(0,COMPS_LENGTH-1);
                int faultChance = probDist(generator);
				int randomComp;

				// generate a fault if one is not already happening
				if ((faultChance <= PROB_OF_BREAK)&&(state.fault == false)){
					// generate a broken component fault
					state.prevComps = state.currComps;
					randomComp = compDist(generator);
					state.currComps.at(randomComp) = -1;
					state.fault = true;
				}

				faultChance = probDist(generator);
				if ((faultChance <= PROB_OF_MULTICOMPS)&&(state.fault == false)){
					// generate a multiple components are on fault
					state.prevComps = state.currComps;
					randomComp = compDist(generator);
					while (state.currComps.at(randomComp) != 0){
						randomComp = compDist(generator);
					}
					state.currComps.at(randomComp) = 1;
					state.fault = true;
				}

				faultChance = probDist(generator);
				if ((faultChance <= PROB_OF_BAD_TRANSITION)&&(state.fault == false)){
					// generate a bad transition fault
					state.prevComps = state.currComps;
					randomComp = compDist(generator);
					int activeCompInd = -1;
					for (int i = 0; i < COMPS_LENGTH; i++){
						if (state.currComps.at(i) == 1){
							activeCompInd = i;
							break;
						}
					}
					randomComp = compDist(generator);
					while ((state.currComps.at(randomComp) != 0)||(randomComp == activeCompInd)){
						randomComp = compDist(generator);
					}
					state.currComps.at(randomComp) = 1;
					state.currComps.at(activeCompInd) = 0;
					state.fault = true;
				}

				// check if a fault is in progress or resolved
				int numbOfOnComps = 0;
				state.brokenComp = false;
				for (int i = 0; i < COMPS_LENGTH; i++){
					switch(state.currComps.at(i)){
						case -1:
							state.brokenComp = true;
							break;
						case 1:
							numbOfOnComps++;
							break;
						default:
							break;
					}
				}
				if (numbOfOnComps > 1){
					state.multiComps = true;
				} else if (numbOfOnComps < 1){
					// we assume a state where everything is off or broken is the result of a bad transition
					state.badTransition = true;
				} else {
					// exactly one component is on
					state.multiComps = false;
				}

				if ((!state.brokenComp)&&(!state.multiComps)&&(!state.badTransition)){
					// no faults are occurring right now
					state.fault = false;
				}

				state.sigma = 25;
			}

			/**
			 * The external transition function is invoked each time external data
			 * is sent to an input port for this model.
			 *
			 * The state of the helicopter only advances to the state it is told to advance to, unless a component is broken
			 * 
			 * @param state reference to the current model state.
			 * @param e time elapsed since the last state transition function was triggered.
			 */
			void externalTransition(HeloState& state, double e) const override {

				// First check if there are un-handled inputs for the "in" port
				if(!in->empty()){

					// The variable x is created to handle the external input values in sequence.
					// The getBag() function is used to get the next input value.
					for( const auto x : in->getBag()){
                        vector<int> newCompsInfo = x.comps;
						state.prevComps = state.currComps;
						for (int i = 0; i < COMPS_LENGTH; i++){
                            //If the new comp state is different from the previous
                            if (state.currComps.at(i) != newCompsInfo[i]){
                                //If the component is not broken and can be changed
                                if (state.currComps.at(i) != -1){
                                    state.currComps.at(i) = newCompsInfo[i];
                                }
                            }
                        }
					}
					state.sigma = 0;
				}
			}

			/**
			 * This function outputs any desired state values to their associated ports.
			 * 
			 * The state of every component is output
			 *
			 * @param state reference to the current model state.
			 */
			void output(const HeloState& state) const override {
				out->addMessage(CompsInfo(state.currComps));
			}

			/**
			 * It returns the value of state.sigma for this model.
			 * 
			 * Currently, sigma is only used to determine the frequency of faults
			 *
			 * @param state reference to the current model state.
			 * @return the sigma value.
			 */
			[[nodiscard]] double timeAdvance(const HeloState& state) const override {
				return state.sigma;
			}
	};
	#endif // __Helo_HPP__
}