if [ -d "build" ]; then rm -Rf build; fi
if [ -d "bin" ]; then rm -Rf bin; fi
rm simulation_results/*.csv
mkdir -p build
cd build || exit
rm -rf *
cmake ..
make
cd ..
echo Compilation done. Executable in the bin folder
