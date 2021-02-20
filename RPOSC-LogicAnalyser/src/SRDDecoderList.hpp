#pragma once

#include "SContainer.hpp"
#include "SRDDecoderDefintion.h"

class SRDDecoderList: public SContainer{
    public:
        SRDDecoderList(std::string _name, int _size, std::string _def_value);
        void createDecoderList();
        vector<SRDDecoderDefintion> decoderList;
};