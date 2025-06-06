# Simple Helicopter Model

## Description
This project uses the Cadmium v2 library to implement a DEVS model of a helicopter supervisor program. Currently, it uses an array of integers to determine the status of 9 components that the supervisor monitors. 
In the model's internal transition function is has a chance of generating a fault with one of the components and forces it to enter a "broken" state. For a component to be turned "on" or "off" the model must receive a message telling it to do so.
However, if the component being changed is currently "broken", then it will not change. Two other faults are possible, one where multiple components are active simultaneously, and another fault where an unexpected or illegal transition occurs. 

## Installation
Follow the Cadmium V2 installation manual to setup the environment, then clone the repository and run "./build.sh" to create the "main.out" executable in the bin folder. 
If you run the executable with "./bin/main.out" it will run the sample simulation with the input script provided. Faults can be randomly generated in 25 time step intervals
if you run the executable with the argument "T". Using "F" will also disable them, but they are disabled by default when no arguments are provided. 

A tool to visualize the simulations run with the Simple Helo model was made in python as a spyder project. If you run the python script with the libraries pySimpleGui and pandas installed 
a window like the one pictured below will appear. The slider on the bottom left cycles through every state the simple helo model was in during the latest simulation. The time of the current state is shown below the slider. It reads the results from
the results.csv file stored in the simulation_results folder.

![image](https://github.com/user-attachments/assets/4172f3f7-968c-4de9-bce0-4c3cfefaeaac)

## Plans for the Future
- Add a type of fault where an illegal transition occurs [X]
- Add a type of fault where multiple components are acrive [X]
- Add a visual tool for seeing simulation runs [X]
- Add the ability to enable/disable random faults [X]
- Write scripts to present correct behaviour
