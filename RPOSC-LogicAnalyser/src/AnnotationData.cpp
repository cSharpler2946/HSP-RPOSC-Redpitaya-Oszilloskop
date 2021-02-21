#include "AnnotationData.hpp"
#include <nlohmann/json.hpp>
#include "../lib/loguru/loguru.hpp"

AnnotationData::AnnotationData(std::string _name, int _size, std::string _def_value, srd_decoder_inst *_decoderInst):
SContainer(_name, _size, _def_value) {
    LOG_F(INFO, "Created annotation data");
    decoderInst = _decoderInst;
}

void AnnotationData::Update() {
    VALUE->Set(annotationData);
}

void AnnotationData::callbackAnnotation(struct srd_proto_data *pdata, void *cb_data) {
    srd_proto_data_annotation *data = (srd_proto_data_annotation *)pdata->data;
    char **annString = (gchar **)g_slist_nth_data(pdata->pdo->di->decoder->annotations, data->ann_class); //double pointer!!
    LOG_F(INFO, "Got annotation data: %d-%d: %d: %s: %s\n", pdata->start_sample, pdata->end_sample, data->ann_class, *annString, *data->ann_text);

    nlohmann::json tmp;
    json["start"]=pdata->start_sample;
    json["end"]=pdata->end_sample;
    json["annotationClass"]=*annString;
    json["annotationText"]=*data->ann_text; //TODO: Test if these derefences are right

    annotationData.push_back(tmp.dump());
    Update();
}

void AnnotationData::resetData() {
    LOG_F(INFO, "Restet annotation data");
    annotaionData.erase();
}