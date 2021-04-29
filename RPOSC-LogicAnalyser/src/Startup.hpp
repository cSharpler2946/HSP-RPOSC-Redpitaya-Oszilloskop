#pragma once

#include "PContainer.hpp"
#include "SRDDecoderList.hpp"

class Startup: public PContainer {
    public:
        Startup(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update, SRDDecoderList * _decoderList);
        void OnNewInternal();
        SRDDecoderList *decoderList;
};