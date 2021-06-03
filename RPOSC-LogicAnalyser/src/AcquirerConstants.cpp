#include "AcquirerConstants.hpp"

//constant values which are given by the rp.h
   std::vector<double> AcquirerConstants::supportedSampleRates = {
     125000000,
     15625000,
     1953125,
     122070.3125,
     15258.7890625,
     1907.3486328125};
   std::vector<string> AcquirerConstants::availableChannels = {"IN1", "IN2"};
   std::vector<int> AcquirerConstants::supportedDecimations = {
     1, 8, 64, 1024, 8192, 65536
   };
   std::vector<std::string> AcquirerConstants::supportedPinState = {"LOW", "HIGH"};
   int AcquirerConstants::maxSamples = 16384;
