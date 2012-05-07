#!/bin/bash

for dir in results* ; do
    pushd $dir &> /dev/null
    ../plot.py
    popd &> /dev/null
done

