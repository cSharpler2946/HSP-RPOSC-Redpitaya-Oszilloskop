#pragma once

#include "SContainer.hpp"
#include <libsigrokdecode/libsigrokdecode.h>

class SRDRequestedOptions: public SContainer {
    public:
        SRDRequestedOptions(std::string _name, int _size, std::string _def_value, srd_decoder_inst *_decoderInst);
        bool decoderChanged = false;
        srd_decoder_inst * decoderInst;
        void Update();    //added because ...
};
