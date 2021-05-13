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
    uint sampleRate; //index to set sampleCount in rp.h
    uint decimation; //index to set decimation in rp.h
    uint32_t sampleCount; // index to set buffer size to read the data into
    uint sampleTime; // time to sample (used to calculate if sampleCount and decimation/rate are fitting)
    uint8_t pinState;   // needed to set the gain together with the channel
    string probeAttenuation;
    AllOptionsValid *allOptionsValid;
    ACQChoosenOptions(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update, AllOptionsValid *_allOptionsValid);
    uint TranslateDecimation(string decimation);
    uint TranslateSampleRate(double sampleRate);
    uint8_t TranslatePinState(string pinState);
    uint TranslateSampleTime(string sampleTime);
    uint32_t TranslateSampleCount(int count);
    void OnNewInternal();
    bool ResetParameters(nlohmann::json jsonString);
};
