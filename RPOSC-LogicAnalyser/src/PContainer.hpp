#pragma once

#include "SPContainer.hpp"
#include <CustomParameters.h>

class PContainer: public SPContainer {
    public:
        PContainer(std::string name, CBaseParameter::AccessMode am, string defaultVal, int fpga_update);
        void OnNew();

    protected:
        CStringParameter* VALUE;
};