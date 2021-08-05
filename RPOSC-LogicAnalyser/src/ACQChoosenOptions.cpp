using namespace std;

#include "ACQChoosenOptions.hpp"
#include <loguru.hpp>

// TODO: TranslateFunction um strings auf richtigen Typen zu ändern.
// Macht es sinn den Typen aus rp.h zu inkludieren
//double sampleRate; //index to set sampleCount in rp.h
//int decimation; //index to set decimation in rp.h
//uint32_t sampleCount; // index to set buffer size to read the data into
//double sampleTime; // time to sample (used to calculate if sampleCount and decimation/rate are fitting)
//int pinState;   // needed to set the gain together with the channel
//std::vector<std::string> probeAttenuation;


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
    LOG_F(INFO, "SampleRate: %f", sampleRate);
    LOG_F(INFO, "SampleCount: %d", sampleCount);
    LOG_F(INFO, "SampleTime: %f", sampleTime);
    LOG_F(INFO, "gain for Channel 1: %d", gainPerChannel[0]);
    LOG_F(INFO, "gain for Channel 2: %d", gainPerChannel[1]);
    LOG_F(INFO, "attenuation for Channel 1: %s", probeAttenuation[0].c_str());
    LOG_F(INFO, "attenuation for Channel 2: %s", probeAttenuation[1].c_str());
  }
  decimation = CalculateDecimation(sampleRate);
  LOG_F(INFO, "decimation is calculated and set to: %d", decimation);
}

bool ACQChoosenOptions::ResetParameters(nlohmann::json jsonString)
{
  gainPerChannel.clear();
  probeAttenuation.clear();
  sampleRate = jsonString["samplerate_Hz"];
  sampleCount = jsonString["samplecount"];
  sampleTime = jsonString["sampletime_us"];
  string tmp = (string)(jsonString["gainPerChannel"]["IN1"]);
  uint8_t gain0 = TranslatePinState(tmp.c_str());
  gainPerChannel.push_back(gain0);
  tmp = (string)(jsonString["gainPerChannel"]["IN2"]);
  uint8_t gain1 = TranslatePinState(tmp.c_str());
  gainPerChannel.push_back(gain1);

  tmp = (string)(jsonString["probeAttenuationPerChannel"]["IN1"]);
  probeAttenuation.push_back(tmp.c_str());
  tmp = (string)(jsonString["probeAttenuationPerChannel"]["IN2"]);
  probeAttenuation.push_back(tmp.c_str());
  
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
