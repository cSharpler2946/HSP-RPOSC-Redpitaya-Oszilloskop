#pragma once

#include "SContainer.hpp"
#include <libsigrokdecode/libsigrokdecode.h>
#include <string>

class AnnotationData: public SContainer{
    public:
        AnnotationData(std::string _name, int _size, std::string _def_value, srd_decoder_inst *_decoderInst);
        void callbackAnnotation(struct srd_proto_data *pdata, void *cb_data);
        vector<std::string> annotationData;
        srd_decoder_inst *decoderInst;
};