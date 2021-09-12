#pragma once

#include "ACQChoosenOptions.hpp"
#include <vector>


class Acquirer{
    public:
        virtual bool startAcq(){};
        virtual vector<float> getData(int channel){};
};