#!/bin/bash

root_dir=$(dirname $0)
echo "Root directory:" $root_dir

echo "Removing old build dir: " $root_dir/dist
rm -rf $root_dir/dist

echo "Running npm install"
cd $root_dir
npm install

echo "Building website"
npm run build
