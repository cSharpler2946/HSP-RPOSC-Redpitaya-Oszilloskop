#!/bin/bash

root_dir=$(dirname $0)
echo "Root directory:" $root_dir

if [ ! -d "${root_dir}/externalRedpitaya" ] && [ ! -d "${root_dir}/gcc-arm-10.2-2020.11-x86_64-arm-none-linux-gnueabihf" ]; then
    echo "Missing dependencies: Due to file size constraints, not all necessary dependencies can be included."
    echo "Please clone the following repository and use that as your starting point for the project: https://github.com/cSharpler2946/HSP-RPOSC-Redpitaya-Oszilloskop."
    exit 1
fi

echo "Removing old build dir: " $root_dir/build
rm -rf $root_dir/build

echo "Calling build.sh script"
bash $root_dir/build.sh
