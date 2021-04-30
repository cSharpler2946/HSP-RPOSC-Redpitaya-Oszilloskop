#!/bin/bash

root_dir=$(dirname $0)
echo "Root directory:" $root_dir

echo "Building application"
mkdir $root_dir/build
cd $root_dir/build

echo "Calling cmake"
cmake ..

echo "Calling make"
make