#include "AnnotationData.hpp"
#include "../lib/nlohmann/jsonWrapper.hpp"
#include <loguru.hpp>
#include <math.h>

AnnotationData::AnnotationData(std::string _name, int _size, std::string _def_value, srd_session *_srdSession):
SContainer(_name, _size, _def_value) {
    LOG_F(INFO, "Created annotation data");
    srdSession = _srdSession;

    LOG_F(INFO, "Adding callback");
    srd_error_code ret = SRD_OK;
    if((ret = ToErr srd_pd_output_callback_add(srdSession, SRD_OUTPUT_ANN, &callbackAnnotation, this)) != SRD_OK)
    {
        LOG_F(ERROR, "Couldn't send data to srd session: Error code : %d", ret);
    }
}

void AnnotationData::Update() {
    //LOG_F(INFO, "Update in AnnotationData");
    if (annotationData.size() % 250 == 0)
        LOG_F(INFO, "Current annotationData vector size: %d", annotationData.size());
    VALUE->Set(annotationData);
}

void AnnotationData::callbackAnnotation(struct srd_proto_data *pdata, void *cb_data) {
    //TODO: Implement function following the old lsrd 0.3.0 api version...
    //LOG_F(INFO, "Got ann data");
    srd_proto_data_annotation *data = (srd_proto_data_annotation *)pdata->data;
    //LOG_F(INFO, "Retrieved data from pdata");
    char **annString = (gchar **)g_slist_nth_data(pdata->pdo->di->decoder->annotations, data->ann_class); //double pointer!!, ann_format is called ann_class in lsrd4
    //LOG_F(INFO, "Retrieved **annString %s", *annString);

    //LOG_F(INFO, "Got start %d", pdata->start_sample);
    //LOG_F(INFO, "Got end %d", pdata->end_sample);
    //LOG_F(INFO, "Got ann_format %d", data->ann_format);
    //LOG_F(INFO, "Got annString %s", *annString);
    //LOG_F(INFO, "Got ann_text %s", *(data->ann_text));

    //LOG_F(INFO, "Got annotation data: %d-%d: %d: %s: %s", pdata->start_sample, pdata->end_sample, data->ann_format, *annString, *(data->ann_text));

    //Testing of uart decoding
    std::string s = *annString;
    if (s == "rx-data" || s == "tx-data" || s == "data" || s == "data-read" || s == "data-write")
        LOG_F(INFO, "Got %s: %s", *annString, *data->ann_text);
    
    nlohmann::json tmp;
    tmp["start"]=pdata->start_sample;
    tmp["end"]=pdata->end_sample;
    tmp["annotationClass"]=*annString;
    tmp["annotationText"]=*(data->ann_text);

    //LOG_F(INFO, "Trying to get back to right object instance and send data: %s", tmp.dump().c_str());
    ((AnnotationData *)cb_data)->annotationData.push_back(tmp.dump());
    ((AnnotationData *)cb_data)->Update();
}

void AnnotationData::resetData() {
    LOG_F(INFO, "Restet annotation data");
    annotationData.clear();
}
