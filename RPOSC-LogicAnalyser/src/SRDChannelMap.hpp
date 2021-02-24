#pragma once

#include "PContainer.hpp"
#include "AllOptionsValid.hpp"
#include <libsigrokdecode/libsigrokdecode.h>
#include "Acquirer.hpp"
#include <map>

class SRDChannelMap: public PContainer {
    public:
        SRDChannelMap(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update, AllOptionsValid *_allOptionsValid, srd_decoder_inst *_decoderInst, Acquirer *_currentAcquirer);
        void OnNewInternal();
        void resetChannelMap();
        std::map<std::string, std::string> channelMap;
        AllOptionsValid *allOptionsValid;
        srd_decoder_inst *decoderInst;
        Acquirer *currentAcquirer;
};