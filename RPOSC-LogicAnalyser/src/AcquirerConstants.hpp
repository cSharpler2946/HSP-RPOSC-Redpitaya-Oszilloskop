
#pragma once

using namespace std;
#include<vector>
#include<string>

class AcquirerConstants
{
    public:
    static vector<double> supportedSampleRates;
    static vector<string> availableChannels;
    static vector<int> supportedDecimations;
    static vector<string> supportedPinState;
    static vector<int> maxSamples;
};