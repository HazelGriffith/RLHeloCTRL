#include "cadmium/simulation/root_coordinator.hpp"
#include <limits>
#include "cadmium/simulation/logger/csv.hpp"
#include <iostream>
#include <string>


// We must include our "top model" which is a coupled model used to
// that hold other models inside of it
#include "include/coupled models/top_model.hpp"

using namespace cadmium::RLHeloCTRL;

int main(int argc,char* argv[]){
	
    // Declare and initialize the top model
    auto model = std::make_shared<Top_model>("top_model");
    auto rootCoordinator = cadmium::RootCoordinator(model);
	
    // For simulation purposes, set the name of the output file
	rootCoordinator.setLogger<cadmium::CSVLogger>("simulation_results/results.csv",",");

    rootCoordinator.start();

    // For simulations, we can set the number of seconds we want to simulate
    rootCoordinator.simulate(250.0);

    rootCoordinator.stop();
    return 0;
}
