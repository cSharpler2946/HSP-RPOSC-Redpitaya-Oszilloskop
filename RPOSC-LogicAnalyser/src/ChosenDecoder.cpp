#include "ChosenDecoder.hpp"

ChosenDecoder::ChosenDecoder(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update, SRDRequestedOptions *_reqOptions, srd_decoder_inst *_decoderInstance, AllOptionsValid *_allOptionsValid):
PContainer(name, am, defaultVal, fpga_update) {
    reqOptions = _reqOptions;
    decoderInstance = _decoderInstance;
    allOptionsValid = _allOptionsValid;
}

ChosenDecoder::loadChosenDecoder() {
    //Load the chosen decoder
    //Set AllOptionsValid::decoderValid=false;
}

ChosenDecoder::OnNewInternal() {
    /*
    * Read value
    * Call loadChosenDecoder()
    * Initiatate reqOptions to load and update options
    */

    reqOptions->Update();
}

