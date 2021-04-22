using namespace std;
#include<string>
#include<vector>
#include<algorithm>
#include "ACQChoosenOptions.hpp"
#include "AcquirerConstants.hpp"

// TODO: TranslateFunction um strings auf richtigen Typen zu ändern.
// Macht es sinn den Typen aus rp.h zu inkludieren
int sampleRate; //index to set sampleCount in rp.h
int decimation; //index to set decimation in rp.h
uint32_t sampleCount; // index to set buffer size to read the data into
string sampleTime; // time to sample (used to calculate if sampleCount and decimation/rate are fitting)
int pinState;   // needed to set the gain together with the channel
string probeAttenuation;


ACQChoosenOptions::ACQChoosenOptions(){}

// Translate the userfriendly string into the fitting index
int ACQChoosenOptions::TranslateSampleRate(string sampleRate){
  int index = -1;
  auto found = find(AcquirerConstants::supportedSampleRates.begin(), AcquirerConstants::supportedSampleRates.end(), sampleRate);

  // if element found, we need the index because setting the sampleRate is done by an integer number
  if(found != AcquirerConstants::supportedSampleRates.end())
  {
    index = found - AcquirerConstants::supportedSampleRates.begin();
  }
  // if element is not found return -1 as an error
  else{
    index = -1;
  }
  return index;
}

// Translate the userfriendly string into the fitting index
int ACQChoosenOptions::TranslateDecimation(string decimation)
{
  int index = -1;
  auto found = find(AcquirerConstants::supportedSampleRates.begin(), AcquirerConstants::supportedSampleRates.end(), decimation);

  // again check in the vector for the string and use the index later for initialisation
  if(found != AcquirerConstants::supportedSampleRates.end())
  {
    index = found - AcquirerConstants::supportedSampleRates.begin();
  }
  else{
    index = -1;
  }
  return index;
}

// Translate the userfriendly string into the fitting index
int ACQChoosenOptions::TranslatePinState(string pinState)
{
  int index = -1;
  auto found = find(AcquirerConstants::supportedSampleRates.begin(), AcquirerConstants::supportedSampleRates.end(), pinState);
  if(found != AcquirerConstants::supportedSampleRates.end())
  {
    index = found - AcquirerConstants::supportedSampleRates.begin();
  }
  else{
    index = -1;
  }

  return index;
}

// convert counter string to int and check if its in the range of the possible buffersize
uint32_t ACQChoosenOptions::TranslateSampleCount(string count)
{
  uint32_t index = -1;
  // convert the string into an integer and check if it's in the range
  index = std::stoi(count);
  if(index > 0 && index < 16 *1024)
  {
    return index;
  }
  else{
    return -1;
  }
}
