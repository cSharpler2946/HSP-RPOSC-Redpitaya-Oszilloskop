#pragma once

using namespace std;
#include <string>
#include "PContainer.hpp"
#include<string>
#include<vector>
#include<algorithm>
#include "AcquirerConstants.hpp"
#include "../lib/nlohmann/jsonWrapper.hpp"
#include "PContainer.hpp"
#include "AllOptionsValid.hpp"

class ACQChoosenOptions : public PContainer{
  public:
    double sampleRate; //index to set sampleCount in rp.h
    uint32_t sampleCount; // index to set buffer size to read the data into
    double sampleTime; // time to sample (used to calculate if sampleCount and decimation/rate are fitting)
    uint decimation; // needed to set up acquirer. Calculated with sampleRate
    vector<int> gainPerChannel;
    vector<int> probeAttenuation;
    AllOptionsValid *allOptionsValid;
    ACQChoosenOptions(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update, AllOptionsValid *_allOptionsValid);
    uint32_t TranslateSampleCount(int count);
    void OnNewInternal();
    bool ParseParameters(nlohmann::json jsonString);
    bool ValidateOptions();
    uint CalculateDecimation(double sampleRate);
    uint8_t TranslatePinState(string pinState);
    uint TranslateSampleTime(string sampleTime);
    
    
    
};
