#include "SRDChannelMap.hpp"

SRDChannelMap::SRDChannelMap(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update, AllOptionsValid *_allOptionsValid, srd_decoder_inst *_decoderInst, Acquirer *_currentAcquirer):
PContainer(name, am, defaultVal, fpga_update) {
    allOptionsValid = _allOptionsValid;
    decoderInst = _decoderInst;
    currentAcquirer = _currentAcquirer;
}

void SRDChannelMap::OnNewInternal() {
    /* TODO: Implement OnNewInternal for SRDChannelMap
    * Read the new data from VALUE
    * Interpret json
    * Check if srdChannel and acqChannel exists
    * convert to map<string, string> where first string is srdChannel and second is acqChannel
    */

    /* TODO: Untested
    nlohmann::json cm = nlohmann::json::parse(VALUE->GetData());
        channelMap.clear();
        for (auto& el : cm.items())
        {
            LOG_F(INFO, "Inserting into channelMap: %s - %s", el.key(), el.value());
            channelMap.insert(std::pair<std::string, std::string>(el.key(), el.value())); //Test if this really returns strings
        }

        bool valid = false;
        GSList * i;
        for (i = decoderInst->decoder->channels; i; i = i->next)
        {
            srd_channel * ch = static_cast<srd_channel *>(i->data);
            if(channelMap.count(ch->id)==0) //Returns the number of accurences of key in map, so 1 or 0
            {
                valid = false;
                break;
            } else {
                valid = true;
            }
        }
        allOptionsValid->setChannelMapValidity(valid);
    */
}

void SRDChannelMap::resetChannelMap() {
    allOptionsValid->setChannelMapValidity(false);
    channelMap.clear();
}