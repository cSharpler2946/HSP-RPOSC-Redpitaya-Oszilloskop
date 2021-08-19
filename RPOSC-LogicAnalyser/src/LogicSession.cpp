#include "LogicSession.hpp"
#include <loguru.hpp>
#include <map>
#include <string>
#include <vector>
#include <unistd.h>


LogicSession::LogicSession(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update, srd_session *_srdSession, srd_decoder_inst **_decoderInst, Acquirer *_acquirer, AllOptionsValid *_allOptionsValid, SRDChannelMap *_channelMap, MeasuredData *_measuredData, AnnotationData *_annotationData, ACQChoosenOptions *_acqChoosenOptions):
PContainer(name, am, defaultVal, fpga_update) {
    LOG_F(INFO, "LogicSession instantiated");
    srdSession = _srdSession;
    decoderInst = _decoderInst;
    acquirer = _acquirer;
    allOptionsValid = _allOptionsValid;
    channelMap = _channelMap;
    measuredData = _measuredData;
    annotationData = _annotationData;
    acqChoosenOptions = _acqChoosenOptions;
}

void LogicSession::Update() {
    nlohmann::json tmp;
    tmp["measurementState"]=measurementState; //Should use strings because of declaration "NLOHMANN_JSON_SERIALIZE_ENUM" in .hpp

    VALUE->Set(tmp.dump());
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
       * - run "ToErr srd_session_send(sess, 0, arrsize-1, inbuf, arrsize, 1);"
       * - set measurementState=stopped and call Update()
       */

        LOG_F(INFO, "Starting logic session");

        //TODO: Completly untested and not implemented completely
        if (allOptionsValid->areAllOptionsValid() == false)
            {
                LOG_F(ERROR, "Tried to start aquisition, but not all options were valid");
                measurementState = stopped;
                Update();
                return;
            }

        measuredData->resetData();
        annotationData->resetData();

        measurementState = running;
        Update();

        if(!acquirer->startAcq())
        {
            LOG_F(ERROR, "Acquirer was not able to fill its buffers.");
            measurementState = stopped;
            Update();
            return;
        }
        
        int sampleCount;
        std::map<std::string, std::vector<double>> dataMap;
        for(int i = 0; i<AcquirerConstants::availableChannels.size(); i++)
        {
            vector<double> data = acquirer->getData(i);
            measuredData->addData(AcquirerConstants::availableChannels[i], data);
            dataMap.insert(std::pair<std::string, std::vector<double> >(AcquirerConstants::availableChannels[i], data));
            sampleCount = data.size();
        }

        //Generate an set the srd channel map
        LOG_F(INFO, "Generating srd channel map");
        vector<std::string> setChannels;
        int i = 0;
        GHashTable * channels = g_hash_table_new(g_int_hash, g_int_equal);
        for(std::pair<std::string, std::string> paar: channelMap->channelMap)
        {
            char * tmpFirst = new char[paar.first.length()]; 
            strcpy(tmpFirst, paar.first.c_str());
            GVariant * gkey = (GVariant *)(tmpFirst); //Invalid cast... Comp options? ... Looks like they raped glib...
            //GVariant * gkey = g_variant_new_string(paar.first.c_str());
            GVariant * gvalue = g_variant_new_int32(i);
            g_hash_table_insert(channels, gkey, gvalue);
            LOG_F(INFO, "Inserted into srd channel map: %s -> %d", (const char*)gkey, i);

            setChannels.push_back(paar.second);
            i++;
        }

        //Test if element is really in hashmap. Only for testing
        /*GVariant* test;
        const char * testKey = channelMap->channelMap.begin()->first.c_str();
        if( (test = (GVariant *)g_hash_table_lookup (channels, testKey))==NULL)
            LOG_F(ERROR, "Could not find %s in hash table", testKey);
        else
            LOG_F(INFO, "Found for %s, value: %d", testKey, g_variant_get_int32(test));

        usleep(1000);
        */

        LOG_F(INFO, "Setting the channels");
        srd_error_code ret;
        if((ret = ToErr srd_inst_channel_set_all(*decoderInst, channels, 1)) != SRD_OK)
        {
            LOG_F(ERROR, "Couldn't set channels: Error code : %d", ret);
        }

        //Interleave data from acq channels which were used in channelMap into mixedData array
        LOG_F(INFO, "Interleaving data");
        int mixedDataLength = sampleCount*setChannels.size();
        double *mixedData = new double[mixedDataLength];
        for(int i = 0; i<sampleCount; i++)
        {
            for(int y = 0; y<setChannels.size(); y++)
            {
                std::string channelId = setChannels[y];
                //LOG_F(INFO, "ChannelId: %s", channelId.c_str());
                std::vector<double> vect = dataMap[channelId];
                //LOG_F(INFO, "Got vect, empty: %d", vect.empty()); //Empty happens if Channel names sent from frontend are not in AcquirerConstants::availableChannels
                double dataPoint = vect[i];
                //LOG_F(INFO, "DataPoint: %f", dataPoint);
                mixedData[(i*setChannels.size())+y]=dataPoint;
            }
        }
        measuredData->addData("Interleaved data", vector<double>(mixedData, mixedData+mixedDataLength));
        
        //parse mixedData to be in range uint_8:0-255
        LOG_F(INFO, "Parsing mixedData into range");
        double min = DBL_MAX;
        double max = -DBL_MAX;
        for(int i = 0; i<mixedDataLength; i++) //Move min/max finding to last loop
        {
            if(mixedData[i] < min)
                min = mixedData[i];
            if(mixedData[i] > max)
                max = mixedData[i];
        }
        double step = UINT8_MAX/max-min;
        LOG_F(INFO, "LS: Range: %f to %f -> step: %f", min, max, step);

        vector<double> tmp;
        uint8_t *normalizedData = new uint8_t[sampleCount*setChannels.size()];
        for(int i = 0; i<mixedDataLength; i++)
        {
            normalizedData[i] = (uint8_t)(mixedData[i]*step);
            tmp.push_back(normalizedData[i]);
        }
        measuredData->addData("Normalized data", tmp);

        //set sample rate
        long sampleRate = acqChoosenOptions->sampleRate;
        LOG_F(INFO, "Setting sample rate to: %d", sampleRate);
        GVariant *metadata = g_variant_new_uint64(sampleRate);
        if((ret = ToErr srd_session_metadata_set(srdSession, SRD_CONF_SAMPLERATE, metadata)) != SRD_OK )
        {
            LOG_F(ERROR, "Couldn't set sampleRate: Error code : %d", ret);
        }

        //run decoder
        LOG_F(INFO, "Starting decoding");
        if((ret = ToErr srd_session_start(srdSession)) != SRD_OK)
        {
            LOG_F(ERROR, "Couldn't start srd session: Error code : %d", ret);
        }

        if((ret = ToErr srd_session_send(srdSession, 0, mixedDataLength-1, normalizedData, mixedDataLength)) != SRD_OK)
        {
            LOG_F(ERROR, "Couldn't send data to srd session: Error code : %d", ret);
        }

        LOG_F(INFO, "Decoding finished, setting measurementState back to stopped");
        measurementState = stopped;
        Update();

        delete mixedData;
        //TODO: delete normalizedData
   }
}
