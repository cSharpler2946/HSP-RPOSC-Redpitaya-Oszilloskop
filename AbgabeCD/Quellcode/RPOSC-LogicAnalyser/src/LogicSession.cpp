#include "LogicSession.hpp"
#include <loguru.hpp>
#include <map>
#include <string>
#include <vector>
#include <unistd.h>
#include "uartTestData.h"
#include <Python.h>
#include <libsigrokdecode/libsigrokdecode.h>
#include <SRDChosenOptions.hpp>

LogicSession::LogicSession(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update, srd_session *_srdSession, srd_decoder_inst **_decoderInst, Acquirer *_acquirer, AllOptionsValid *_allOptionsValid, SRDChannelMap *_channelMap, MeasuredData *_measuredData, std::vector<SContainer*> *_sContainerList, ACQChoosenOptions *_acqChoosenOptions):
PContainer(name, am, defaultVal, fpga_update) {
    LOG_F(INFO, "LogicSession instantiated");
    srdSession = _srdSession;
    decoderInst = _decoderInst;
    acquirer = _acquirer;
    allOptionsValid = _allOptionsValid;
    channelMap = _channelMap;
    measuredData = _measuredData;
    sContainerList = _sContainerList;
    acqChoosenOptions = _acqChoosenOptions;
}

void LogicSession::Update() {
    nlohmann::json tmp;
    tmp["measurementState"]=measurementState; //Should use strings because of declaration "NLOHMANN_JSON_SERIALIZE_ENUM" in .hpp

    VALUE->Set(tmp.dump());
}

void printOptions(srd_decoder_inst * inst)
{
    GSList *i;
    for (i = inst->decoder->options; i; i = i->next) {
        struct srd_decoder_option *p = static_cast<srd_decoder_option *>(i->data);
        const GVariantType *type = g_variant_get_type(p->def);
        const gchar * typeS = g_variant_type_peek_string(type);
        //cout << p->id << ":" << typeS << ":";
        LOG_F(INFO, "%d : %s", p->id, typeS);
        if(g_variant_is_of_type(p->def, G_VARIANT_TYPE_INT64)) {
            LOG_F(INFO, "%d", g_variant_get_uint64(p->def));
            //cout << g_variant_get_uint64(p->def) << ":";
            g_list_foreach(reinterpret_cast<GList *>(p->values), [](gpointer data, gpointer user_data){ cout << g_variant_get_int64((GVariant*)data) << ","; }, nullptr);
        } else if (g_variant_is_of_type(p->def, G_VARIANT_TYPE_STRING)) {
            LOG_F(INFO, "%s", g_variant_get_string(p->def, nullptr));
            //cout << g_variant_get_string(p->def, nullptr) << ":";
            g_list_foreach(reinterpret_cast<GList *>(p->values), [](gpointer data, gpointer user_data){ cout << g_variant_get_string((GVariant*)data, nullptr) << ","; }, nullptr);
        } else if (g_variant_is_of_type(p->def, G_VARIANT_TYPE_DOUBLE)) {
            LOG_F(INFO, "%f", g_variant_get_double(p->def));
            //cout << g_variant_get_double(p->def) << ":";
            g_list_foreach(reinterpret_cast<GList *>(p->values), [](gpointer data, gpointer user_data){ cout << g_variant_get_double((GVariant*)data) << ","; }, nullptr);
        }
        //cout << endl;
    }
}

srd_error_code setOptions(srd_decoder_inst * inst, map<string,string> options)
{
    //Construct g_hash_table from options:map<>
    GHashTable *table = g_hash_table_new(g_int_hash, g_int_equal);
    for (const auto& [key, value] : options)
    {
        const gchar *gkey = key.c_str();
        //Searches srd_decoder_options for type string of option. Ignores options that can not be found in inst->decoder->options
        GSList * tmp = g_slist_find_custom(inst->decoder->options, gkey, [](gconstpointer elem, gconstpointer cmp){
            return strcmp(((srd_decoder_option *)elem)->id, (char *)cmp);
        });
        if (tmp == nullptr) {
            cout << "Provided option not found: " << gkey << endl;
            continue;
        }
        srd_decoder_option * p = static_cast<srd_decoder_option *>(tmp->data);

        GVariant * gvalue;
        if(g_variant_is_of_type(p->def, G_VARIANT_TYPE_INT64)) {
            //int64
            gvalue = g_variant_new_int64(stol(value));
        } else if (g_variant_is_of_type(p->def, G_VARIANT_TYPE_STRING)) {
            //string
            gvalue = g_variant_new_string(value.c_str());
        } else if (g_variant_is_of_type(p->def, G_VARIANT_TYPE_DOUBLE)) {
            //double
            gvalue = g_variant_new_double(stod(value));
        }

        if(gvalue == nullptr) {
            cout << "Error converting option to gvariant: " << gkey << endl;
            continue;
        }

        g_hash_table_insert(table, (gpointer)gkey, (gpointer)gvalue);
    }

    return ToErr srd_inst_option_set(inst, table);
}

srd_error_code setSampleRate(srd_session* srdSession, uint64_t rate)
{
    GVariant *metadata = g_variant_new ("t", rate); //Following function only supports the uint64 (="t") data type!!
    return ToErr srd_session_metadata_set(srdSession, SRD_CONF_SAMPLERATE, metadata);
}

void LogicSession::OnNewInternal() {
    nlohmann::json tmp = nlohmann::json::parse(VALUE->Value());
    measurementState = tmp["measurementState"].get<MeasurementState>();
    LOG_F(INFO, "Recieved new measurement state %s -> %d", tmp.dump().c_str(), measurementState);

   if(measurementState==starting) {
       /* TODO: Add logic to logicSessions
       * Start measurement chain:
       * - Evtl. check the AllDataValid object
       * - reset MeasuredData and AnnotationData
       * - set measurementState=running and call Update()
       * - Aquirer.startAcq()
       * - Aquirer.getData()
       * - Calls to MeasuredData::addData()
       * - Mix data accoriding to the SRDChannelMap
       * - Prepare mixed data to be in uInt8 range
       * - run "ToErr srd_session_send(srdSession, 0, arrsize-1, inbuf, arrsize, 1);"
       * - set measurementState=stopped and call Update()
       */
        //Libsigrokdecode init

        LOG_F(INFO, "Starting logic session");
        const char *decoderId = ChosenDecoder::decoderId.c_str();

        srd_error_code err;
        //Initialize srd
        //srd_session *srdSession;
        if ((ToErr srd_exit()) != SRD_OK)
        {
            LOG_F(ERROR, "Libsigrok exit failed");
            return;
        }
        if ((ToErr srd_init(nullptr)) != SRD_OK)
        {
            LOG_F(INFO, "LibSigrokDecode init failed\n");
            return;
        }
        else
        {
            LOG_F(INFO, "LibSigrokDecode init success: Using version: %s\n", srd_lib_version_string_get());
            LOG_F(INFO, "Creating srd_session object");
            srd_session_new(&srdSession);
            srd_log_loglevel_set(SRD_LOG_DBG);
        }

        AnnotationData *annotationData = new AnnotationData("ANNOTATION_DATA", 512, "", srdSession);
        sContainerList->push_back(annotationData);
        annotationData->resetData();

        //Load decoder w. id
        LOG_F(INFO, "Loading decoder with id \"%s\"...", ChosenDecoder::decoderId.c_str());
        if((err = ToErr srd_decoder_load(decoderId)) != SRD_OK) {
            LOG_F(ERROR, "Failed loading decoder (srd_error_coder: %d). Please select new one", err);
            return;
        }
        srd_decoder_inst *inst = srd_inst_new(srdSession, decoderId, nullptr);

        //Add options to decoder instance
        map<string, string> newOptions = SRDChosenOptions::chosenOptions;
        err = setOptions(inst, newOptions);
        if (err == SRD_OK)
            allOptionsValid->setDecoderValidity(true);

        if (allOptionsValid->areAllOptionsValid() == false)
        {
            LOG_F(ERROR, "Tried to start aquisition, but not all options were valid");
            measurementState = stopped;
            Update();
            return;
        }

        //Reset measured data
        measuredData->resetData();

        //Set state
        measurementState = running;
        Update();

        //Start acquisition
        if(!acquirer->startAcq())
        {
            LOG_F(ERROR, "Acquirer was not able to fill its buffers.");
            measurementState = stopped;
            Update();
            return;
        }

        //Get acquired data
        int sampleCount;
        std::map<std::string, std::vector<float>> dataMap;
        for(int i = 0; i<AcquirerConstants::availableChannels.size(); i++)
        {
            vector<float> data = acquirer->getData(i);
            //vector<float> data(testdata, testdata + sizeof(testdata)/sizeof(testdata[0])); //Testdata
            measuredData->addData(AcquirerConstants::availableChannels[i], data);
            dataMap.insert(std::pair<std::string, std::vector<float> >(AcquirerConstants::availableChannels[i], data));
            sampleCount = data.size();
        }

        //Set channels
        LOG_F(INFO, "Generating srd channel map");
        vector<std::string> setChannels;
        int i = 0;
        GHashTable * channels = g_hash_table_new(g_int_hash, g_int_equal);
        for(std::pair<std::string, std::string> paar: channelMap->channelMap)
        {
            char * tmpFirst = new char[paar.first.length()]; 
            strcpy(tmpFirst, paar.first.c_str());
            GVariant * gkey = (GVariant *)(tmpFirst); //Invalid cast... Comp options? ... Looks like they raped glib...
            GVariant * gvalue = g_variant_new_int32(i);
            g_hash_table_insert(channels, gkey, gvalue);
            LOG_F(INFO, "Inserted into srd channel map: %s -> %d", (const char*)gkey, i);

            setChannels.push_back(paar.second);
            i++;
        }
        LOG_F(INFO, "Setting the channels");
        if((err = ToErr srd_inst_channel_set_all(inst, channels)) != SRD_OK)
        {
            LOG_F(ERROR, "Couldn't set channels: Error code : %d", err);
        }

        //Setting SRD_CONF_SAMPLERATE
        err = setSampleRate(srdSession, acqChoosenOptions->sampleRate);
        if (err != SRD_OK)
        {
            LOG_F(ERROR, "Couldn't set sampleRate: Error code : %d", err);
            return;
        }
        
        //Concatenate data
        LOG_F(INFO, "Concatenate data");
        int mixedDataLength = sampleCount*setChannels.size();
        float *mixedData = new float[mixedDataLength];

        for(int i = 0; i<sampleCount; i++)
        {
            for(int y = 0; y<setChannels.size(); y++)
            {
                std::string channelId = setChannels[y];
                std::vector<float> vect = dataMap[channelId];
                float dataPoint = vect[i];
                mixedData[(sampleCount*y)+i]=dataPoint;
            }
        }

        measuredData->addData("concatenated data", vector<float>(mixedData, mixedData+mixedDataLength));
        
        //parse mixedData to be in range uint_8:0-255
        LOG_F(INFO, "Parsing mixedData into range");
        float min = FLT_MAX;
        float max = FLT_MIN;
        for(int i = 0; i<mixedDataLength; i++) //Move min/max finding to last loop
        {
            if(mixedData[i] < min)
                min = mixedData[i];
            if(mixedData[i] > max)
                max = mixedData[i];
        }
        float step = UINT8_MAX/(max-min);
        LOG_F(INFO, "Range: %f to %f -> step: %f", min, max, step);

        vector<float> tmp;
        uint8_t *normalizedData = new uint8_t[mixedDataLength];
        for(int i = 0; i<mixedDataLength; i++)
        {
            //normalizedData[i] = (uint8_t) (mixedData[i]*step);
            normalizedData[i] = (mixedData[i] > max/2) ? UINT8_MAX : 0; //Decoder does not work with in between data on edges
            tmp.push_back(normalizedData[i]);
        }
        measuredData->addData("Normalized data", tmp);

        //Start session
        LOG_F(INFO, "Starting decoding");
        err = ToErr srd_session_start(srdSession);
        if (err != SRD_OK)
        {
            LOG_F(ERROR, "Couldn't start srd session: Error code : %d", err);
        }

        //Send prepared test data to decoder
        err = ToErr srd_session_send(srdSession, 0, mixedDataLength-1, normalizedData, mixedDataLength, 1);
        if (err != SRD_OK)
        {
            LOG_F(ERROR, "Couldn't send data to srd session: Error code : %d", err);
        }

        LOG_F(INFO, "Decoding finished, setting measurementState back to stopped");
        measurementState = stopped;
        Update();
        
        delete mixedData;
        delete normalizedData;
   }
}
