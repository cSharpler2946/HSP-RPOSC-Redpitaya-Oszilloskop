#pragma once

#include "PContainer.hpp"
#include <libsigrokdecode/libsigrokdecode.h>
#include "SRDRequestedOptions.hpp"
#include "AllOptionsValid.hpp"
#include "SRDChannels.hpp"

class ChosenDecoder: public PContainer {
    public:
        ChosenDecoder(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update, SRDRequestedOptions *_reqOptions, SRDChannels *_channels, srd_session *_srdSession, srd_decoder_inst **_decoderInstance, AllOptionsValid *_allOptionsValid);
        void loadChosenDecoder();
        void OnNewInternal();
        SRDRequestedOptions* reqOptions;
        srd_decoder_inst **decoderInstance;
        srd_session *srdSession;
        AllOptionsValid *allOptionsValid;
        SRDChannels *channels;
};
