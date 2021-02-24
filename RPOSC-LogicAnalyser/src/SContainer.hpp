#pragma once

#include "SPContainer.hpp"
#include <string>
#include "CStringSignal.h"

class SContainer: public SPContainer {
    public:
        SContainer(std::string _name, int _size, std::string _def_value);
        void OnNew();
    protected:
        CStringSignal *VALUE;
};