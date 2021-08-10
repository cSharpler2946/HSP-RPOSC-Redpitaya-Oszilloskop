#include "AcquirerConstants.hpp"

//constant values which are given by the rp.h


// ATTENTIONE!!!!
// It's very important that the indexes of the decimations match the indexes of the sampleRates
// the calculation of the decimation is simply done by maping with the indexes

   std::vector<double> AcquirerConstants::supportedSampleRates = {
     125000000,
     15625000,
     1953125,
     122070.3125,
     15258.7890625,
     1907.3486328125};
   //std::vector<string> AcquirerConstants::availableChannels = {"IN1", "IN2"};
   std::vector<string> AcquirerConstants::availableChannels = {"Channel 1", "Channel 2"}; //Just for debugging as frontend currently does not get channel names from backend
   std::vector<int> AcquirerConstants::supportedDecimations = {
     1, 8, 64, 1024, 8192, 65536
   };
   std::vector<std::string> AcquirerConstants::supportedPinState = {"LOW", "HIGH"};
   std::vector<int> AcquirerConstants::supportedPinStateInt = {0,1};
   int AcquirerConstants::maxSamples = 16384;
   std::vector<string> AcquirerConstants::supportedAttenuation = {"1x", "10x"};
