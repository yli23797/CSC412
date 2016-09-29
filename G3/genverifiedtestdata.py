"""Creates the directory "/verifiedtestdata" and fills it with matrix files.
5A * 5B = 5O,
25A * 25B = 25O, etc...

The number is the square size of the matrices.
"""

import numpy
import os

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

if __name__=="__main__":
    try:
        os.mkdir("verifiedtestdata")
    except FileExistsError:
        pass
    for t in range(5, 100, 5):
        A = numpy.random.randint(-5, 5, (t, t))
        B = numpy.random.randint(-5, 5, (t, t))
        Result = A.dot(B)

        write_matrix(os.path.join("verifiedtestdata", "{0}A".format(t)), A)
        write_matrix(os.path.join("verifiedtestdata", "{0}B".format(t)), B)
        write_matrix(os.path.join("verifiedtestdata", "{0}O".format(t)), Result)

