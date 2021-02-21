#pragma once

using namespace std;
#include <vector>
#include <string>

class ACQChoosenOptions{
  public:
    int sampleRate; //index to set sampleCount in rp.h
    int decimation; //index to set decimation in rp.h
    int sampleCount; // index to set buffer size to read the data into
    string sampleTime; // time to sample (used to calculate if sampleCount and decimation/rate are fitting)
    int pinState;   // needed to set the gain together with the channel
    string probeAttenuation;
    ACQChoosenOptions();
    int TranslateDecimation(string decimation);
    int TranslateSampleRate(string sampleRate);
    int TranslatePinState(string pinState);
    int TranslateSampleCount(string count);
};
