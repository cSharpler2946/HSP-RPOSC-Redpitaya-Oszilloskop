#include "AllOptionsValid.hpp"
#include "../lib/nlohmann/jsonWrapper.hpp"

AllOptionsValid::AllOptionsValid(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update):
PContainer(name, am, defaultVal, fpga_update) {

}

void AllOptionsValid::Update() {
    nlohmann::json tmp;
    tmp["acquirerValid"]=acquirerValid;
    tmp["channelMapValid"]=channelMapValid;
    tmp["decoderValid"]=decoderValid;

    VALUE->Set(tmp.dump());
}

void AllOptionsValid::setAcquirerValidity(bool v) {
    acquirerValid = v;
    Update();
}

void AllOptionsValid::setChannelMapValidity(bool v) {
    channelMapValid = v;
    Update();
}

void AllOptionsValid::setDecoderValidity(bool v) {
    decoderValid = v;
    Update();
}

bool AllOptionsValid::areAllOptionsValid() {
    return acquirerValid && channelMapValid && decoderValid;
}