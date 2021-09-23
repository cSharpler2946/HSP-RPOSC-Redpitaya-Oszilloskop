#!/bin/bash

if [ ! -d "${root_dir}/externalRedpitaya" ] && [ ! -d "${root_dir}/gcc-arm-10.2-2020.11-x86_64-arm-none-linux-gnueabihf" ]; then
    echo "Missing dependencies: Due to file size constraints, not all necessary dependencies can be included."
    echo "Please clone the following repository and use that as your starting point for the project: https://github.com/cSharpler2946/HSP-RPOSC-Redpitaya-Oszilloskop."
    exit 1
fi

root_dir=$(dirname $0)
echo "Root directory:" $root_dir

echo "Building application"
mkdir $root_dir/build
cd $root_dir/build

echo "Calling cmake"
cmake ..

echo "Calling make"
make
