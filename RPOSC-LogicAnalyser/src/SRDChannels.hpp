#pragma once

#include "SContainer.hpp"
#include <libsigrokdecode/libsigrokdecode.h>

class SRDChannels: public SContainer {
    public:
        SRDChannels(std::string _name, int _size, std::string _def_value, srd_decoder_inst *_decoderInst);
        srd_decoder_inst *decoderInst;
        //TODO: Decide from where Update is called (probably ChosenDecoder)
        void Update();  // added function because it is used in cpp....
};
