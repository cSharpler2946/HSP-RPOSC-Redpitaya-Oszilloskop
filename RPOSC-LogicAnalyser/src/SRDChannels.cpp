#include "SRDChannels.hpp"

SRDChannels::SRDChannels(std::string _name, int _size, std::string _def_value, srd_decoder_inst *_decoderInst):
SContainer(_name, _size, _def_value) {
    decoderInst = _decoderInst;
}

void SRDChannels::Update() {  //set to void because some type is needed to compile :D
    /*
    * Read channels from decoderInst
    * Create json
    * set VALUE
    */
}
