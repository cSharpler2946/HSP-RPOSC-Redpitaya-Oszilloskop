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
}

void SRDChannelMap::resetChannelMap() {
    allOptionsValid->setChannelMapValidity(false);
    channelMap.clear();
}