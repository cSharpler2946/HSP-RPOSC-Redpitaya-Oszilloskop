#pragma once

#include "SContainer.hpp"
#include <libsigrokdecode/libsigrokdecode.h>
#include "../lib/nlohmann/jsonWrapper.hpp"
#include "AllOptionsValid.hpp"
#include <string>

class SRDChosenOptions: public PContainer {
public:
    SRDChosenOptions(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update, srd_decoder_inst **_decoderInst, AllOptionsValid *_allOptionsValid);
    srd_error_code setOptions(std::map<std::string,std::string> options);
    void OnNewInternal() override;
    srd_decoder_inst **decoderInst;
    AllOptionsValid *allOptionsValid;
};
