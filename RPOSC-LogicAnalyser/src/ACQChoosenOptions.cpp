using namespace std;

#include "ACQChoosenOptions.hpp"
#include <loguru.hpp>

//double sampleRate; //index to set sampleCount in rp.h
//int decimation; //index to set decimation in rp.h
//uint32_t sampleCount; // index to set buffer size to read the data into
//double sampleTime; // time to sample (used to calculate if sampleCount and decimation/rate are fitting)
//int pinState;   // needed to set the gain together with the channel
//std::vector<std::string> probeAttenuation;


ACQChoosenOptions::ACQChoosenOptions(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update, AllOptionsValid *_allOptionsValid) : PContainer(name, am, defaultVal, fpga_update)
{
  allOptionsValid = _allOptionsValid; //TODO: Set the options valid, when they are (currently hotfixed with default value = true in AllOptionsValid.hpp)
}

void ACQChoosenOptions::OnNewInternal()
{
  LOG_F(INFO, "update parameters for acquisition!");
  nlohmann::json tmp = nlohmann::json::parse(VALUE->Value());

  if(ParseParameters(tmp))
  {
    if(ValidateOptions())
    {
      LOG_F(INFO, "Options are validated successfully!");
      allOptionsValid->setAcquirerValidity(true);
    }
    else{
      LOG_F(INFO, "Options are invalid!");
      allOptionsValid->setAcquirerValidity(false);
      return;
    }
    LOG_F(INFO, "Successfully updated parameter values for Aquirer!");
    LOG_F(INFO, "SampleRate: %f", sampleRate);
    LOG_F(INFO, "SampleCount: %d", sampleCount);
    LOG_F(INFO, "SampleTime: %f", sampleTime);
    LOG_F(INFO, "gain for Channel 1: %d", gainPerChannel[0]);
    LOG_F(INFO, "gain for Channel 2: %d", gainPerChannel[1]);
    LOG_F(INFO, "attenuation for Channel 1: %s", probeAttenuation[0].c_str());
    LOG_F(INFO, "attenuation for Channel 2: %s", probeAttenuation[1].c_str());
    LOG_F(INFO, "decimation is calculated and set to: %d", decimation);
  }
  else
  {
    LOG_F(INFO, "Something went wrong by parsing the options!!");
    allOptionsValid->setAcquirerValidity(false);
  }
}

// Parse the Json into the Variables
bool ACQChoosenOptions::ParseParameters(nlohmann::json jsonString)
{
  gainPerChannel.clear();
  probeAttenuation.clear();
  sampleRate = jsonString["samplerate_Hz"];
  sampleCount = jsonString["samplecount"];
  sampleTime = jsonString["sampletime_us"];
  string tmp = (string)(jsonString["gainPerChannel"]["Channel 1"]); //TODO: Make dependent on constants
  uint8_t gain0 = TranslatePinState(tmp.c_str());
  gainPerChannel.push_back(gain0);
  tmp = (string)(jsonString["gainPerChannel"]["Channel 2"]);
  uint8_t gain1 = TranslatePinState(tmp.c_str());
  gainPerChannel.push_back(gain1);

  tmp = (string)(jsonString["probeAttenuationPerChannel"]["Channel 1"]);
  probeAttenuation.push_back(tmp.c_str());
  tmp = (string)(jsonString["probeAttenuationPerChannel"]["Channel 2"]);
  probeAttenuation.push_back(tmp.c_str());
  
  decimation = CalculateDecimation(sampleRate);

  return true;
}

// checks the variables for validity (if one of the variables is unvalid false is returned)
bool ACQChoosenOptions::ValidateOptions()
{
  auto foundRate = find(AcquirerConstants::supportedSampleRates.begin(), AcquirerConstants::supportedSampleRates.end(), sampleRate);
  if(foundRate == AcquirerConstants::supportedSampleRates.end())
  {
    LOG_F(INFO, "Sample Rate is not supported!!");
    return false;
  }
  if(sampleCount > AcquirerConstants::maxSamples)
  {
    LOG_F(INFO, "Sample count is bigger than buffer!!");
    return false;
  }
  auto foundGain0 = find(AcquirerConstants::supportedPinStateInt.begin(), AcquirerConstants::supportedPinStateInt.end(), gainPerChannel[0]);
  auto foundGain1 = find(AcquirerConstants::supportedPinStateInt.begin(), AcquirerConstants::supportedPinStateInt.end(), gainPerChannel[1]);
  if(foundGain0 == AcquirerConstants::supportedPinStateInt.end() || foundGain1 == AcquirerConstants::supportedPinStateInt.end())
  {
    LOG_F(INFO, "only gain high and low are supported!!");
    return false;
  }
  auto foundAttenuation0 = find(AcquirerConstants::supportedAttenuation.begin(), AcquirerConstants::supportedAttenuation.end(), probeAttenuation[0]);
  auto foundAttenuation1 = find(AcquirerConstants::supportedAttenuation.begin(), AcquirerConstants::supportedAttenuation.end(), probeAttenuation[1]);
  if(foundAttenuation0 == AcquirerConstants::supportedAttenuation.end() || foundAttenuation1 == AcquirerConstants::supportedAttenuation.end())
  {
    LOG_F(INFO, "Only attenuation 1 and 10 are supported!!");
    return false;
  }
  auto foundDecimation = find(AcquirerConstants::supportedDecimations.begin(), AcquirerConstants::supportedDecimations.end(), decimation);
  if(foundDecimation == AcquirerConstants::supportedDecimations.end())
  {
    LOG_F(INFO, "The decimation is not supported!!");
    return false;
  }
  // calculate the SampleTime for the given SampleRate(if not valid already aborted above) and the sampleCount(max 16384 if bigger, already aborted above)
  // sampleRate * sampleTime <= maxSamples
  if(sampleRate*sampleTime <= AcquirerConstants::maxSamples)
  {
    return false;
  }
  return true;
}

// Translate the userfriendly string into the fitting index
uint ACQChoosenOptions::CalculateDecimation(double sampleRate)
{
  int index = -1;
  auto found = find(AcquirerConstants::supportedSampleRates.begin(), AcquirerConstants::supportedSampleRates.end(), sampleRate);

  // again check in the vector for the string and use the index later for initialisation
  if(found != AcquirerConstants::supportedSampleRates.end())
  {
    index = found - AcquirerConstants::supportedSampleRates.begin();
  }
  else{
    index = -1;
  }
  return AcquirerConstants::supportedDecimations[index];
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
