#include "ACQRequestedOptions.hpp"

ACQRequestedOptions::ACQRequestedOptions(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update):
PContainer(name, am, defaultVal, fpga_update){}

void ACQRequestedOptions::Update()
{
    LOG_F(INFO, "Initialized the aquirer options");
    nlohmann::json tmp;
    tmp["sampleRates"]=AcquirerConstants::supportedSampleRates;
    tmp["channels"]=AcquirerConstants::availableChannels;
    tmp["decimations"]=AcquirerConstants::supportedDecimations;
    tmp["state"]=AcquirerConstants::supportedPinState;
    tmp["sampleLimit"]=AcquirerConstants::maxSamples;
    VALUE->Set(tmp.dump());
}