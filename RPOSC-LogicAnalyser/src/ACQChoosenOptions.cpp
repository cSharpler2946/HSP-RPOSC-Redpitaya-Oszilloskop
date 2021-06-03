using namespace std;

#include "ACQChoosenOptions.hpp"
#include <loguru.hpp>

// TODO: TranslateFunction um strings auf richtigen Typen zu ändern.
// Macht es sinn den Typen aus rp.h zu inkludieren
int sampleRate; //index to set sampleCount in rp.h
int decimation; //index to set decimation in rp.h
uint32_t sampleCount; // index to set buffer size to read the data into
string sampleTime; // time to sample (used to calculate if sampleCount and decimation/rate are fitting)
int pinState;   // needed to set the gain together with the channel
string probeAttenuation;


ACQChoosenOptions::ACQChoosenOptions(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update, AllOptionsValid *_allOptionsValid) : PContainer(name, am, defaultVal, fpga_update)
{
  allOptionsValid = _allOptionsValid;
}

void ACQChoosenOptions::OnNewInternal()
{
  LOG_F(INFO, "update parameters for acquisition!");

  nlohmann::json tmp = nlohmann::json::parse(VALUE->Value());
  if(ResetParameters(tmp))
  {
    LOG_F(INFO, "Successfully updated parameter values for Aquirer!");
    LOG_F(INFO, "SampleRate: ", sampleRate);
  }
}

bool ACQChoosenOptions::ResetParameters(nlohmann::json jsonString)
{
  double tmp = jsonString["samplerate_Hz"];
  LOG_F(INFO, "samplerate: %f", tmp);
  //std:string test = jsonString["samplerate_Hz"];
  //LOG_F(INFO, "%s", test);
  //sampleRate = TranslateSampleRate(jsonString["samplerate_Hz"]);
  //LOG_F(INFO, "hallo"jsonString["samplerate_Hz"]);
  //sampleCount = TranslateSampleCount(jsonString["samplecount"]);
  //sampleTime = TranslateSampleTime(jsonString["sampletime_us"]);
  //pinState = TranslatePinState(jsonString["gain"]);
  //probeAttenuation = jsonString["probeAttenuation"];
  //decimation = jsonString["decimation"];
  return true;
}

// Translate the userfriendly string into the fitting index
uint ACQChoosenOptions::TranslateSampleRate(double sampleRate){
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
uint ACQChoosenOptions::TranslateDecimation(int decimation)
{
  int index = -1;
  auto found = find(AcquirerConstants::supportedDecimations.begin(), AcquirerConstants::supportedDecimations.end(), decimation);

  // again check in the vector for the string and use the index later for initialisation
  if(found != AcquirerConstants::supportedDecimations.end())
  {
    index = found - AcquirerConstants::supportedDecimations.begin();
  }
  else{
    index = -1;
  }
  return index;
}

// Translate the userfriendly string into the fitting index
uint8_t ACQChoosenOptions::TranslatePinState(string pinState)
{
  int index = -1;
  auto found = find(AcquirerConstants::supportedPinState.begin(), AcquirerConstants::supportedPinState.end(), pinState);
  if(found != AcquirerConstants::supportedPinState.end())
  {
    index = found - AcquirerConstants::supportedPinState.begin();
  }
  else{
    index = -1;
  }

  return index;
}

// convert counter string to int and check if its in the range of the possible buffersize
uint32_t ACQChoosenOptions::TranslateSampleCount(int count)
{
  uint32_t index = -1;
  // convert the string into an integer and check if it's in the range
  index = count;
  if(index > 0 && index < 16 *1024)
  {
    return index;
  }
  else{
    return -1;
  }
}

// convert time string to int and check if its in the range.
uint ACQChoosenOptions::TranslateSampleTime(string time)
{
  uint32_t index = -1;
  // convert the string into an integer and check if it's in the range
  index = std::stoi(time);
  // TODO: calculation how long we can sample with the defined sampleRate and the defined buffersize
    return index;
}
