#pragma once

#include <vector>
#include "AcquirerConstants.hpp"
#include <string>
#include "PContainer.hpp"
#include "../lib/nlohmann/jsonWrapper.hpp"
#include "loguru.hpp"

class ACQRequestedOptions: public PContainer{
    public: 
        ACQRequestedOptions(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update);
        void Update();
};