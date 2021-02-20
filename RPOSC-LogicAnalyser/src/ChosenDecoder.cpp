#include "ChosenDecoder.hpp"

ChosenDecoder::ChosenDecoder(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update, SRDRequestedOptions *_reqOptions, srd_decoder_inst *_decoderInstance):
PContainer(name, am, defaultVal, fpga_update) {
    reqOptions = _reqOptions;
    decoderInstance = _decoderInstance;
}

ChosenDecoder::loadChosenDecoder() {
    //Load the chosen decoder
}

ChosenDecoder::OnNewInternal() {
    /*
    * Read value
    * Call loadChosenDecoder()
    * Initiatate reqOptions to load and update options
    */

    reqOptions->Update();
}

