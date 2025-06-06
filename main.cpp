#include "cadmium/simulation/root_coordinator.hpp"
#include <limits>
#include "cadmium/simulation/logger/csv.hpp"
#include <iostream>
#include <string>

#include "include/coupled models/top_model.hpp"

using namespace cadmium::SimpleHelo;

int main(int argc,char* argv[]){
	bool randFaults = false;
    if (argc > 2){
        assert((false, "Too many arguments"));
    } else if (argc == 2){
        std::string arg1(argv[1]);

        if ((arg1.compare("T") == 0)||(arg1.compare("t") == 0)){
            randFaults = true;
        } else if ((arg1.compare("F") == 0)||(arg1.compare("f") == 0)){
            randFaults = false;
        } else {
            assert((false, "The argument provided must be T or F"));
        }
    }

    // Declare and initialize the top model
    auto model = std::make_shared<Top_model>("top_model", randFaults);
    auto rootCoordinator = cadmium::RootCoordinator(model);
	
    //set the name of the output file
	rootCoordinator.setLogger<cadmium::CSVLogger>("simulation_results/results.csv",",");

    rootCoordinator.start();

    rootCoordinator.simulate(250.0);

    rootCoordinator.stop();
    return 0;
}
