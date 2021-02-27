#include "ChosenDecoder.hpp"
#include <libsigrokdecode/libsigrokdecode.h>
#include "../lib/nlohmann/jsonWrapper.hpp"
#include "../lib/loguru/loguru.hpp"

#define ToErr (srd_error_code)

ChosenDecoder::ChosenDecoder(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update, SRDRequestedOptions *_reqOptions, SRDChannels *_channels, srd_session *_srdSession, srd_decoder_inst *_decoderInstance, AllOptionsValid *_allOptionsValid):
PContainer(name, am, defaultVal, fpga_update) {
    LOG_F(INFO, "ChosenDecoder instatiated");
    reqOptions = _reqOptions;
    channels = _channels;
    srdSession = _srdSession;
    decoderInstance = _decoderInstance;
    allOptionsValid = _allOptionsValid;
}

void ChosenDecoder::loadChosenDecoder() {
    // TODO: Remove function. Functionality already in OnNewInternal()
    //Load the chosen decoder
    //Set AllOptionsValid::decoderValid=false;
}

void ChosenDecoder::OnNewInternal() {
    LOG_F(INFO, "Unloading all old decoders");
    srd_error_code err;
    if((err = ToErr srd_decoder_unload_all()) != SRD_OK) {
        LOG_F(ERROR, "Failed unloading old decoders! (srd_error_coder: %d)", err);
    }
    // make sure to delete all the \n
    /*string jsonString = VALUE->Value();
    std::replace(jsonString.begin(), jsonString.end(), '\n', ' ');
    nlohmann::json tmp = nlohmann::json::parse(jsonString);*/
    nlohmann::json tmp = nlohmann::json::parse(VALUE->Value())
    LOG_F(INFO, "Loading decoder with id \"%s\"...", tmp["id"]);
    if((err = ToErr srd_decoder_load(tmp["id"].dump().c_str())) != SRD_OK) {
        LOG_F(ERROR, "Failed loading decoder (srd_error_coder: %d). Please select new one", err);
        return;
    }
    decoderInstance = srd_inst_new(srdSession, tmp["id"].dump().c_str(), nullptr); //Set decoder instance
    allOptionsValid->setDecoderValidity(false); //After a new decoder is loaded srd_set_options must be called at least once
    LOG_F(INFO, "Deocder successfully loaded");

    reqOptions->Update();
    channels->Update();
}
