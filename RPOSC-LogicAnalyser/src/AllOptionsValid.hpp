#pragma once

#include "PContainer.hpp"

class AllOptionsValid: public PContainer {
    public:
        AllOptionsValid(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update);
        void Update();
        void setChannelMapValidity(bool v);
        void setAcquirerValidity(bool v);
        void setDecoderValidity(bool v);
        bool areAllOptionsValid();

    private:
        bool channelMapValid = false;
        bool acquirerValid = false;
        bool decoderValid = false;
}