#!/bin/bash

root_dir=$(dirname $0)
echo "Root directory:" $root_dir

echo "Removing old build dir: " $root_dir/deploy
rm -rf $root_dir/deploy

mkdir $root_dir/deploy
mkdir $root_dir/deploy/RPOSC-LogicAnalyser

# CPP App
bash $root_dir/RPOSC-LogicAnalyser/cleanAndBuild.sh
cp $root_dir/RPOSC-LogicAnalyser/build/controllerhf.so $root_dir/deploy/RPOSC-LogicAnalyser

# Website
bash $root_dir/logic-analyzer-webui-dev/build.sh
cp -r $root_dir/logic-analyzer-webui-dev/dist/* $root_dir/deploy/RPOSC-LogicAnalyser

# copy deployFiles
cp -r $root_dir/deployFiles/* $root_dir/deploy/RPOSC-LogicAnalyser
