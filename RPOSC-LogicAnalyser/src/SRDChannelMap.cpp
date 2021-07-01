#include "SRDChannelMap.hpp"

//TODO: For what is aquirer used? remove?
SRDChannelMap::SRDChannelMap(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update, AllOptionsValid *_allOptionsValid, srd_decoder_inst **_decoderInst, Acquirer *_currentAcquirer):
PContainer(name, am, defaultVal, fpga_update) {
    allOptionsValid = _allOptionsValid;
    decoderInst = _decoderInst;
    currentAcquirer = _currentAcquirer;
}

void SRDChannelMap::OnNewInternal() {
    /*
    * Read the new data from VALUE
    * Interpret json
    * Check if srdChannel and acqChannel exists
    * convert to map<string, string> where first string is srdChannel and second is acqChannel
    */
    LOG_F(INFO, "SRDChannelMap new data received");
    
    nlohmann::json cm = nlohmann::json::parse(VALUE->Value());
    channelMap.clear();
    for (auto& el : cm.items())
    {
        std::string acqC=el.value()["acqChannel"];
        std::string srdC=el.value()["srdChannel"];
        LOG_F(INFO, "Inserting into channelMap: %s - %s", srdC.c_str(), acqC.c_str());
        channelMap.insert(std::pair<std::string, std::string>(srdC, acqC)); //Test if this really returns strings
    }

    bool valid = false;
    GSList * i;
    for (i = (*decoderInst)->decoder->channels; i; i = i->next)
    {
        srd_channel * ch = static_cast<srd_channel *>(i->data);
        if(channelMap.count(ch->id)==0) //Returns the number of occurences of key in map, so 1 or 0
        {
            valid = false;
            break;
        } else {
            valid = true;
        }
    }
    for (i = (*decoderInst)->decoder->opt_channels; i; i = i->next)
    {
        srd_channel * ch = static_cast<srd_channel *>(i->data);
        LOG_F(INFO, "Checking occurance: %s, %d", ch->id, channelMap.count(ch->id));
        if(channelMap.count(ch->id)==0) //Returns the number of occurences of key in map, so 1 or 0
        {
            valid = false;
            break;
        } else {
            valid = true;
        }
    }
    LOG_F(INFO, "Setting channelMapValidty to %d", valid);
    allOptionsValid->setChannelMapValidity(valid);
}

void SRDChannelMap::resetChannelMap() {
    allOptionsValid->setChannelMapValidity(false);
    channelMap.clear();
}