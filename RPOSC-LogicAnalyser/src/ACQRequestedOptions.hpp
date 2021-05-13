#pragma once

using namespace std;
#include <string>
#include "PContainer.hpp"
#include<vector>
#include "AcquirerConstants.hpp"
#include "../lib/nlohmann/jsonWrapper.hpp"
#include "PContainer.hpp"
#include "AllOptionsValid.hpp"


class ACQRequestedOptions : public PContainer
{
    public:
        ACQRequestedOptions(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update);
        void Update();
};