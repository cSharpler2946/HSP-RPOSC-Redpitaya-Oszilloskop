#pragma once

#include "ACQChoosenOptions.hpp"
#include <vector>


class Acquirer{
    public:
        virtual bool startAcq(){};
        virtual vector<double> getData(int channel){};
};