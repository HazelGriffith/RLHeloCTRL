# Simple Helicopter Model

## Description
This project uses the Cadmium v2 library to implement a DEVS model of a helicopter supervisor program. Currently, it uses an array of integers to determine the status of 9 components that the supervisor monitors. 
In the model's internal transition function is has a chance of generating a fault with one of the components and forces it to enter a "broken" state. For a component to be turned "on" or "off" the model must receive a message telling it to do so.
However, if the component being changed is currently "broken", then it will not change.

## Installation
Follow the Cadmium V2 installation manual to setup the environment, then clone the repository and run "./build.sh" to create the "main.out" executable in the bin folder. 
If you run the executable with "./main.out" it will run the sample simulation with the input script provided.

## Plans for the Future
- Add a type of fault where an illegal transition occurs
