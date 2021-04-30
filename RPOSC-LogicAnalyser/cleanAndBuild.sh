#!/bin/bash

root_dir=$(dirname $0)
echo "Root directory:" $root_dir

echo "Removing old build dir: " $root_dir/build
rm -rf $root_dir/build

echo "Calling build.sh script"
bash $root_dir/build.sh