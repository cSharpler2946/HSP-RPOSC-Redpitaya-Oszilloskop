#include "SContainer.hpp"
#include "SRDDecoderDefintion.h"

class SRDDecoderList: SContainer{
    public:
        void createDecoderList();
        vector<SRDDecoderDefintion> decoderList;
};