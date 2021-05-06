#include "AcquirerConstants.hpp"

//constant values which are given by the rp.h
   std::vector<string> AcquirerConstants::supportedSampleRates = {
     "125Msps",
     "15.625Msps",
     "1.953Msps",
     "122.070Ksps",
     "15.258Ksps",
     "1.907Ksps"};
   std::vector<string> AcquirerConstants::availableChannels = {"A", "B"};
   std::vector<std::string> AcquirerConstants::supportedDecimations = {
     "1", "8", "64", "1024", "8192", "65536"
   };
   std::vector<std::string> AcquirerConstants::supportedPinState = {"LOW", "HIGH"};
  std::vector<std::string> AcquirerConstants::maxSamples = {"16384"};