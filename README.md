# Contents

1. graph_results.py - reads a results csv file and produces histogram figures.
2. perf_test.py - runs the program with various parameters and logs the results to a csv file.
3. test_gen.py - uses the example outputs to generate C test files.
4. `/CITS3402P1` - the main solution and project directory.
5. `/CITS3402P1-Tests` - the project that contains unit tests for the main program.
6. `executable_x64.zip` - a zip file containing a release build of the program for 64bit windows.
7. `report.pdf` - the project report
8. `unit-tests-log.txt` - a log of the output from running the test project.
9. `v1-stats` - the raw test results and graphs derived from them for the 'First Approach' variations of the operators.
9. `v2-stats` - the raw test results and graphs derived from them for the 'Second Approach' variations of the operators.

# Compilation

Open `/CITS3402P1/CITS3402P1.sln` in Visual Studio 2019 (ensure you have installed the C/C++ Development tool workload), right click the `CITS3402P1-Tests` project and choose 'unload' and then build the solution. The executable can then be found in the `x64` directory.

A binary for Windows 64-bit is provided in the `executable_x64.zip` file if required.

# Running The Tests

You will need to build and install a 64-bit version of the check library from source (this can be annoying to do) so you can instead run the test executable found in the `executable_x64.zip` HOWEVER YOU MUST RUN IT FROM THE CORRECT DIRECTORY, it expects files to exist in locations such as `"./inputs/float64.in"` relative to the current work directory. Therefore you should invoke it from within the `CITS3402P1-Tests` folder.
