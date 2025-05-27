#ifndef __HELO_HPP__
#define __HELO_HPP__
#define COMPS_LENGTH 9
#define PROB_OF_FAULT 50

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
    comps[6] = PilotTakeover
    comps[7] = Landing
    0-6 values: -1 = broken, 0 = off, 1 = on
    7 values: -1 = broken, 0 = off, 1 = no-hover, 2 = hover 
    */
	struct HeloState {

		double sigma;

		// Declare model-specific variables
		vector<int> comps;
		
		// Set the default values for the state constructor for this specific model
		HeloState(): sigma(0), comps({0,0,0,0,0,0,0,0,0}) {};
	};

	std::ostream& operator<<(std::ostream &out, const HeloState& state) {
		string compNames[COMPS_LENGTH] = {"TakeOff", "Sticks", "Knobs", "WP", "NMPC", "DAA", "PilotTakeover", "Landing/NoHover", "Landing/Hover"};
		for (int i = 0; i < COMPS_LENGTH; i++){
			out << compNames[i];
			int compState = state.comps.at(i);
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

	// Atomic model of Helo
	class Helo: public Atomic<HeloState> {
		private:

		public:

			// Declare ports for the model

			// Input ports
			Port<CompsInfo> in;

			// Output ports
			Port<CompsInfo> out;

			// Declare variables for the model's behaviour

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
				
				state.sigma = 25;
				
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
                uniform_int_distribution<> randComp(0,7);
                int faultChance = probDist(generator);

                if (faultChance <= PROB_OF_FAULT){
                    int brokenComp = randComp(generator);
                    if (state.comps.at(brokenComp) == -1){
                        state.comps.at(brokenComp) = 0;
                    } else {
                        state.comps.at(brokenComp) = -1;
                    }
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
						for (int i = 0; i < COMPS_LENGTH; i++){
                            //If the new comp state is different from the previous
                            if (state.comps.at(i) != newCompsInfo[i]){
                                //If the component is not broken and can be changed
                                if (state.comps.at(i) != -1){
                                    state.comps.at(i) = newCompsInfo[i];
                                }
                            }
                        }
					}
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
				out->addMessage(CompsInfo(state.comps));
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