#!/usr/bin/env python

import commands
import numpy
import sys

def get_count(cmd):
    print cmd
    rc, out=commands.getstatusoutput(cmd)    
    counter=int(out.splitlines()[-1].split()[-1])
    print "counter was %d" % (counter)
    return counter


def main():

    counts=[]
    for i in range (0,5):
        counts.append( get_count("./second_test 2") )

    dev  = numpy.std(counts)
    mean = numpy.mean(counts)
    n = ( (1.96 * dev **2 ) / mean )

    print "            n : %f" % ( n ) 
    print "         mean : %f" % ( mean )
    print "std deviation : %f" % ( dev )

    while ( n > len(counts) ) :
        counts.append( get_count("./second_test 2") )

        dev  = numpy.std(counts)
        mean = numpy.mean(counts)
        n = ( (1.96 * dev **2 ) / mean )

        print "            n : %f" % ( n ) 
        print "          len : %d" % ( len(counts) )
        print "         mean : %f" % ( mean )
        print "std deviation : %f" % ( dev )





if __name__ == "__main__":
    main()
