#pragma once

#include "SPContainer.hpp"
#include <CustomParameters.h>
#include <string>

class PContainer: public SPContainer {
    public:
        PContainer(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update);
        void OnNew();

    protected:
        CStringParameter* VALUE;
};