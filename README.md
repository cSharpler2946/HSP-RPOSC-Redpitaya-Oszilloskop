# HSP-RPOSC-Redpitaya-Oszilloskop

## Build deployable version
- run `./buildDeploy.sh`
- Requirements: 
    - make
    - cmake
    - npm
- The folder ./deploy/RPOSC-LogicAnalyser can now be copied to the target:  
  `scp -r RPOSC-LogicAnalyser root@xxx.xxx.xxx.xxx:/opt/redpitaya/www/apps/`
  
- **To run the application on the redpitaya, packages from ./target_packages need to be installed!**
