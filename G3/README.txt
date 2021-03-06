CSC410 - G3

Diana Lewis
Alex Houser
Marty Moradian
Yanlin Li
Stephen Chilcote


Keep in mind that all files use 32-bit integers, so sufficiently large dot
products will overflow (or underflow).

 Sequential
------------
The program generates three matrices, two populated with random numbers and
one with 0s. It then loops through the amtrices and calculates their product.
Once calculated, the results are put in the zero-initialized matrix.

To change the grid size, you will need to change the value of N, M, & P.

Compile the file and type ./a.out to run the program (if not passing a -o
flag or renaming).

 pthread
-----------
The program takes two matrix files (provided) as input, and will write out
their product to testResult/<rows>_<columns>_pthread

Compile the file with -pthread, and run with two matrices.  Also, make a
testResult directory if you haven't already.

 OpenMP
-----------
The program can either read two matrix files and write out their product to
testResult/<rows>_<columns>_openmp, or it can run in interactive mode.  In
interactive mode, you must provide the number of rows and columns for each
matrix, and an integer to modulo rand() by.

Compile the file with -fopenmp, and either run with two matrices as you did
for pthread, or with the -i or --interactive flag.

 autotest.py
-------------
Requires Python (3 or 2.7) and the numpy and hypothesis packages.
Install with "pip numpy hypothesis hypothesis[numpy]" if you have pip.

HOW TO GET THIS WORKING ON DSUNIX:

Enter these into your shell:
cd ~/
wget https://bootstrap.pypa.io/ez_setup.py -O - | python - --user
cd ./local/bin
./easy_install --user pip
./pip install --user numpy hypothesis hypothesis[numpy]

Run python autotest.py ./<program>

Use --verbose flag to see test cases run, and --longlong flag to use 63-bit test cases (will cause integer overflow
errors on all working code we have though.)

