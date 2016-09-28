#!/usr/bin/env python
"""Usage: python3 autotest.py yourexe

Requires the following libraries (use pip to install):

numpy
hypothesis

HOW TO GET THIS WORKING ON DSUNIX:

Enter these into your shell:
cd ~/
wget https://bootstrap.pypa.io/ez_setup.py -O - | python - --user
cd ./local/bin
./easy_install --user pip
./pip install --user numpy hypothesis hypothesis[numpy]

Once you've done all that, autotest.py should run like any other script/executable.
Compile your program, enter "./autotest.py ./myprogram.out", and it should tell you if there's anything wrong with your
program.


"""
import glob

import numpy
import argparse
import os

import subprocess

import hypothesis
import hypothesis.extra.numpy

import time

#the wrong way to do Python 3 to 2 compatibility
try:
    raw_input = input
except:
    pass

#This just handles the command line arguments, don't worry about it.
parser=argparse.ArgumentParser(description="Automatically test matrix multiplication programs.")
parser.add_argument('target', type=str, help="The path to the executable to be tested.")
args=parser.parse_args()


#And these are just helper functions that read and write the matrix files our programs handle
def write_matrix(filename, matrix):
    """Write a Numpy matrix to disk in the text format we're all standardizing on."""
    with open(filename, "wt") as thefile:
        #First we write the dimensions...
        #BTW, This'll explode if you put something other than a two-dimensional array into it. So, uh, don't.
        thefile.write("{0} {1}\n".format(*matrix.shape))

        #Then start writing the data itself.
        for theline in matrix:
            #Turn each row into a list of strings where each string is a number on that row, then join them with spaces
            #and write that to the file, followed by a newline.
            thefile.write( ' '.join( [str(x) for x in theline] ) + "\n")

def read_matrix(filename):
    """Read a file in our standard format into a Numpy matrix and return said matrix."""
    with open(filename, "rt") as thefile:
        #We don't care about the first line, so just skip it.
        lines=thefile.readlines()[1:]

        rows=[]
        for theline in lines:
            #Split by whitespace and convert the text, which should be numbers, to integers
            therow = [int(x) for x in theline.split()]
            rows.append(therow)

        #Then make a Numpy array out of that.
        return numpy.array(rows)

#And, last but not least
def run_test(target, matrixa, matrixb, verbose=False):
    """Test the given matrix multiplier with the given matrices against Numpy and raise AssertionError if the program
    gives an incorrect result. If verbose is true, prints the correct answer and the answer the program gave."""
    # Try... finally block so we don't leave random matrix files lying around
    try:
        write_matrix("testmatrixa", matrixa)
        write_matrix("testmatrixb", matrixb)

        try:
            output = subprocess.check_output([target, "testmatrixa", "testmatrixb"])
        except subprocess.CalledProcessError as e:
            print("The subject program gave an error: \n\n")
            if e.output:
                print(e.output)
            else:
                print("[it didn't print anything either...]")
            return
        else:
            if output:
                print("Subject program printed: \n\n")
                print(output)

        # The output file should be the only file in /testResult
        resultpath = next(glob.iglob(os.path.join("testResult", "*")))
        if verbose: print("Reading results from {0}...".format(resultpath))
        result = read_matrix(resultpath)

        # Now multiply the two with Numpy...
        correctresult = numpy.dot(matrixa, matrixb)

        if verbose:
            print("Your program outputted: ")
            print(result)
            print("The correct result: ")
            print(correctresult)

        # And raise AssertionError if the known-good result does not match the result of the program.
        assert numpy.array_equal(result, correctresult)

    finally:
        os.remove("testmatrixa")
        os.remove("testmatrixb")
        pass

if glob.glob(os.path.join("testResult", "*")):
    print("\nI'm about to delete everything in the /testResult directory so that I know the only file in "
          "there is the output of your program. "
          "\nPress Ctrl-C now if this isn't okay...")
    time.sleep(3)

for thepath in glob.iglob(os.path.join("testResult", "*")):
    os.remove(thepath)

print("\n\nRunning test...")
try:
    @hypothesis.given( hypothesis.extra.numpy.arrays(int, (3, 3) ), hypothesis.extra.numpy.arrays(int, (3, 3) ) )
    def test(matrix1, matrix2):
        hypothesis.assume(matrix1.shape == matrix2.shape)
        run_test(args.target, matrix1, matrix2)
    test()


except AssertionError:
    print("Your program is not correct! The 'falsifying example' above is two matrices that your program does not "
          "multiply correctly. There's nothing I can do about the formatting being ugly unfortunately. :(")
except OSError:
    print("Something went wrong: make sure you put ./ in front of your program just like if you were trying to run it"
          " directly.")
    raise
else:
    print("If you didn't get any other error messages (such as 'your program crashed'...) then as far as I can tell"
          "your program works perfectly!")