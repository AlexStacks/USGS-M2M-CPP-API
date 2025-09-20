#!/bin/bash

function buildProject(){
    echo "Building Project"
    mkdir build
    cd build
    cmake ..
    make
    cd ..
    echo "Build script leaving."
}
function cleanProject(){
    echo "Cleaning up project"
    rm -rf build
}
function runProject(){
    echo "Running Project"
    cd build    
    ./stackMaps
    cd ..
    echo "Project finished Running"
}
if [ $OPTIND -eq 1 ]; then 
    cleanProject
    buildProject
    exit 1
fi

while getopts "bcr" opt; do
    case $opt in
        b)
            buildProject
            ;;
        c)
            cleanProject
            ;;
        r)
            cleanProject
            buildProject
            runProject
            ;;
        \?)
            echo "Invalid Option: -$OPTARG" >&2 + "Available options are -b (build), -c (clean), and -r (clean, build, and run)"
            exit 1
            ;;
    esac
done



