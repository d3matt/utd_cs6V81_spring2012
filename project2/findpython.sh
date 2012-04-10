#!/bin/bash

PYTHON27=$(which python2.7 2>/dev/null)
PYTHON26=$(which python2.6 2>/dev/null)
PYTHON=$(which python)

if [ -n "$PYTHON27" ]; then
    echo $PYTHON27
elif [ -n "$PYTHON26" ] ; then
    echo $PYTHON26
else
    echo $PYTHON
fi
