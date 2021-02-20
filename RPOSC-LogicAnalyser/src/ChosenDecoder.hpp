#pragma once
#include "PContainer.hpp"
#include <libsigrokdecode/libsigrokdecode.h>
#include "SRDRequestedOptions.hpp"

class ChosenDecoder: PContainer {
    public:
        ChosenDecoder(std::string name, CBaseParameter::AccessMode am, string defaultVal, int fpga_update, SRDReqestedOptions& _reqOptions, srd_decoder_inst *_decoderInstance);
        void loadChosenDecoder();
        SRDReqOptions& reqOptions;
        srd_decoder_inst *decoderInstance;
}