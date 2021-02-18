#include "SRDDecoderList.hpp"

std::string SRDDecoderList::toJsonString() {
    //TODO: Remove these functions
}

void SRDDecoderList::fromJsonString() {
    //TODO: Remove these functions
}

void SRDDecoderList::Update() {
    //Run only one single time
    //Take decoderList and write elements to VALUE
    //Check if this can be done on system startup or after client connected
}

void SRDDecoderList::OnNewInternal() {
    //Do nothing
}

void CreateDecoderList() {
    /*
    * Create decoderList
    * srd_decoder_load_all
    * srd_decoder_list
    * Loop gslist and put info into decoderList
    */
}
