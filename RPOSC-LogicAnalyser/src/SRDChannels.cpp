#include "SRDChannels.hpp"
#include <libsigrokdecode/libsigrokdecode.h>
#include <gmodule.h>
#include "../lib/nlohmann/jsonWrapper.hpp"
#include <loguru.hpp>
#include <string>
#include <vector>

SRDChannels::SRDChannels(std::string _name, int _size, std::string _def_value, srd_decoder_inst *_decoderInst):
SContainer(_name, _size, _def_value) {
    LOG_F(INFO, "SRDChannels instantiated");
    decoderInst = _decoderInst;
}

void SRDChannels::Update() {
    LOG_F(INFO, "Updating SRDChannels"); //TODO: This function generates segementation fault
    std::vector<std::string> channelV;

    GSList * i;
    for (i = decoderInst->decoder->channels; i; i = i->next)
    {
        srd_channel * ch = static_cast<srd_channel *>(i->data);
        nlohmann::json tmp;
        tmp["id"]=ch->id;
        tmp["name"]=ch->name;
        tmp["desc"]=ch->desc;
        tmp["isOptional"]=false;

        channelV.push_back(tmp.dump());
    }

    GSList * j;
    for (j = decoderInst->decoder->opt_channels; j; j = j->next)
    {
        srd_channel * ch = static_cast<srd_channel *>(i->data);
        nlohmann::json tmp;
        tmp["id"]=ch->id;
        tmp["name"]=ch->name;
        tmp["desc"]=ch->desc;
        tmp["isOptional"]=true;

        channelV.push_back(tmp.dump());
    }

    VALUE->Set(channelV);
}
