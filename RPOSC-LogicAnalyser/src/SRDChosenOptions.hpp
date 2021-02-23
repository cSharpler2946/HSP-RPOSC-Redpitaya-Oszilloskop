#pragma once
#include "SContainer.hpp"
#include <libsigrokdecode/libsigrokdecode.h>
#include "AllOptionsValid.hpp"
#include <map>
#include <string>

class SRDChosenOptions: public SContainer {
    public:
        SRDChosenOptions(std::string _name, int _size, std::string _def_value, srd_decoder_inst *_decoderInst, AllOptionsValid *_allOptionsValid);
        srd_error_code setOption(srd_decoder_inst * inst, std::map<std::string,std::string> options);
        void OnNewInternal();
        bool optionsValid = false;
        srd_decoder_inst *decoderInst;
        AllOptionsValid *allOptionsValid;
};
