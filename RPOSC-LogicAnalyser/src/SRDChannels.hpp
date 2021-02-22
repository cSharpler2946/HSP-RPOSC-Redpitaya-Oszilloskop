#pragma once

#include "SContainer.hpp"
#include <libsigrokdecode/libsigrokdecode.h>

class SRDChannels: public SContainer {
    public:
        SRDChannels(std::string _name, int _size, std::string _def_value, srd_decoder_inst *_decoderInst);
        srd_decoder_inst *decoderInst;
        void Update();
};
