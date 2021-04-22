#include "AnnotationData.hpp"
#include "../lib/nlohmann/jsonWrapper.hpp"
#include <loguru.hpp>

AnnotationData::AnnotationData(std::string _name, int _size, std::string _def_value, srd_decoder_inst *_decoderInst):
SContainer(_name, _size, _def_value) {
    LOG_F(INFO, "Created annotation data");
    decoderInst = _decoderInst;

    //TODO: register callback for annotations
    // ToErr srd_pd_output_callback_add(sess, SRD_OUTPUT_ANN, &callbackAnnotation, nullptr);
}

void AnnotationData::Update() {
    VALUE->Set(annotationData);
}

void AnnotationData::callbackAnnotation(struct srd_proto_data *pdata, void *cb_data) {
    /* TODO: Implement function following the old lsrd 0.3.0 api version...
    srd_proto_data_annotation *data = (srd_proto_data_annotation *)pdata->data;
    char **annString = (gchar **)g_slist_nth_data(pdata->pdo->di->decoder->annotations, data->ann_format); //double pointer!!, ann_format is called ann_class in lsrd4
    LOG_F(INFO, "Got annotation data: %d-%d: %d: %s: %s\n", pdata->start_sample, pdata->end_sample, data->ann_format, *annString, *data->ann_text);
    
    nlohmann::json tmp;
    tmp["start"]=pdata->start_sample;
    tmp["end"]=pdata->end_sample;
    tmp["annotationClass"]=*annString;
    tmp["annotationText"]=*data->ann_text; //TODO: Test if these dereferences are right

    annotationData.push_back(tmp.dump());
    Update();
    */
}

void AnnotationData::resetData() {
    LOG_F(INFO, "Restet annotation data");
    annotationData.clear();
}
