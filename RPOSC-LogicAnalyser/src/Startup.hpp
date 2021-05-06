#pragma once

#include "PContainer.hpp"
#include "SRDDecoderList.hpp"
#include "ACQRequestedOptions.hpp"

class Startup: public PContainer {
    public:
        Startup(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update, SRDDecoderList * _decoderList, ACQRequestedOptions * _requestedOptions);
        void OnNewInternal();
        SRDDecoderList *decoderList;
        ACQRequestedOptions *requestedOptions;
};