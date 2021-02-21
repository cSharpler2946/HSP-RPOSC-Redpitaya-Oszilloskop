#include "AllOptionsValid.hpp"
#include "../lib/nlohmann/json.hpp"

AllOptionsValid::AllOptionsValid(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update):
PContainer(name, am, defaultVal) {

}

AllOptionsValid::Update() {
    json tmp;
    tmp["acquirerValid"]=acquirerValid;
    tmp["channelMapValid"]=channelMapValid;
    tmp["decoderValid"]=decoderValid;

    VALUE->Set(tmp.dump());
}

AllOptionsValid::setAcquirerValidity(bool v) {
    acquirerValid = v;
    Update();
}

AllOptionsValid::setChannelMapValidity(bool v) {
    channelMapValid = v;
    Update();
}

AllOptionsValid::setDecoderValidity(bool v) {
    decoderValid = v;
    Update();
}