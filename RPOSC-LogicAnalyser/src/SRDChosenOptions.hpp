#pragma once
#include "SContainer.hpp"
#include <libsigrokdecode/libsigrokdecode.h>

class SRDChosenOptions: public SContainer {
    public:
        SRDChosenOptions(std::string _name, int _size, std::string _def_value, srd_decoder_inst *_decoderInst);
        srd_error_code setOptions(srd_decoder_inst * inst, map<string,string> options);
        bool optionsValid = false;
        srd_decoder_inst *decoderInst;
};