#pragma once

#include "PContainer.hpp"
#include <libsigrokdecode/libsigrokdecode.h>
#include "SRDRequestedOptions.hpp"

class ChosenDecoder: public PContainer {
    public:
        ChosenDecoder(std::string name, CBaseParameter::AccessMode am, string defaultVal, int fpga_update, SRDRequestedOptions *_reqOptions, srd_decoder_inst *_decoderInstance);
        void loadChosenDecoder();
        SRDRequestedOptions* reqOptions;
        srd_decoder_inst *decoderInstance;
};