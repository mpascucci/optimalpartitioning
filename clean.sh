#!/bin/bash

# CLEAN THE BUILD FILES

# delete python generated folder
find . -name "*.egg-info" -exec rm -r '{}' \; 

# delete *.so files
find . -name "*.so" -exec rm -r '{}' \; 

# delete build folder
if test -d "build"
then
    rm -r build
fi

