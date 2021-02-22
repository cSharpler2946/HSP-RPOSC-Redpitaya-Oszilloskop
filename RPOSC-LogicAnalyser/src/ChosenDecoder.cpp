#include "ChosenDecoder.hpp"
#include <libsigrokdecode/libsigrokdecode.h>
#include "../lib/nlohmann/json.hpp"
#include "../lib/loguru/loguru.hpp"

ChosenDecoder::ChosenDecoder(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update, SRDRequestedOptions *_reqOptions, srd_decoder_inst *_decoderInstance, AllOptionsValid *_allOptionsValid):
PContainer(name, am, defaultVal, fpga_update) {
    reqOptions = _reqOptions;
    decoderInstance = _decoderInstance;
    allOptionsValid = _allOptionsValid;
}

void ChosenDecoder::loadChosenDecoder() {
    // TODO: Remove function. Functionality already in OnNewInternal()
    //Load the chosen decoder
    //Set AllOptionsValid::decoderValid=false;
}

void ChosenDecoder::OnNewInternal() {
    nlohmann::json tmp = VALUE.Value();
    LOG_F(INFO, "Loading decoder with id \"%s\"...", tmp["id"]);
    srd_error_code err;
    if((err = ToErr srd_decoder_load(tmp["id"]) != SRD_OK) {
        LOG_F(ERROR, "Failed loading decoder (srd_error_coder: %d). Please select new one", err);
        return;
    }
    allOptionsValid->setDecoderValidity(true);
    LOG_F(INFO, "Deocder successfully loaded");

    reqOptions->Update();
}
