"""Usage: python3 autotest.py yourexe

Requires the following libraries (use pip to install):

numpy
hypothesis
"""
import numpy
import argparse
import os

import subprocess

#This just handles the command line arguments, don't worry about it.
parser=argparse.ArgumentParser(description="Automatically test matrix multiplication programs.")
parser.add_argument('target', type=str, help="The path to executable to be tested.")
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


#Just some arbitrary matrices for now, fanciness comes later.
matrixa = numpy.array([ [1,2,3,4], [5,6,7,8], [9,10,11,12] ] )
matrixb = numpy.array([[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]])

#Try... finally block so we don't leave random matrix files lying around
try:
    write_matrix("testmatrixa", matrixa)
    write_matrix("testmatrixb", matrixb)
    output = subprocess.run([args.target, "testmatrixa", "testmatrixb"])
    #Everyone seems to have standardized on "testResult\result" being the output file, so that's what we read.

    result = read_matrix(os.path.join("testResult", "result"))

    #Now multiply the two with Numpy...
    correctresult = numpy.dot(matrixa, matrixb)

    #And raise AssertionError if the known-good result does not match the result of the program.
    assert result == correctresult

finally:
    os.remove("testmatrixa")
    os.remove("testmatrixb")