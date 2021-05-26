#include "LogicSession.hpp"
#include <loguru.hpp>
#include <map>
#include <string>
#include <vector>


LogicSession::LogicSession(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update, srd_session *_srdSession, Acquirer *_acquirer, AllOptionsValid *_allOptionsValid, SRDChannelMap *_channelMap, MeasuredData *_measuredData, AnnotationData *_annotationData):
PContainer(name, am, defaultVal, fpga_update) {
    LOG_F(INFO, "LogicSession instantiated");
    srdSession = _srdSession;
    acquirer = _acquirer;
    allOptionsValid = _allOptionsValid;
    channelMap = _channelMap;
    measuredData = _measuredData;
    annotationData = _annotationData;
}

void LogicSession::Update() {
    nlohmann::json tmp;
    tmp["measurementState"]=measurementState; //Should use strings because of declaration "NLOHMANN_JSON_SERIALIZE_ENUM" in .hpp

    VALUE->Set(tmp.dump());
}

void LogicSession::OnNewInternal() {
    nlohmann::json tmp = VALUE->Value();
    measurementState = tmp.get<MeasurementState>();

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

        /* //TODO: Completly untested and not implemented completely
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

        acquirer->startAcq();
        
        int sampleCount;
        std::map<std::string, std::vector<double>> dataMap;
        for(auto channel: acquirer->availableChannels)
        {
            vector<double> data = acquirer->getData(channel);
            measuredData->addData(channel, data);
            dataMap.insert(std::pair<std::string, std::vector<double>(channel, data));
            sampleCount = data.size;
        }

        //Generate an set the srd channel map
        vector<std::string> setChannels;
        int i = 0;
        GHashTable * channels = g_hash_table_new(g_int_hash, g_int_equal);
        for(std::pair paar: channelMap->channelMap)
        {
            GVariant * gkey = (GVariant *)(paar.first); //Looks like they raped glib...
            GVariant * gvalue = g_variant_new_int32(i);
            i++;
            g_hash_table_insert(channels, gkey, gvalue);

            setChannels.push_back(paar.second);
        }

        if((srd_error_code ret = ToErr srd_inst_channel_set_all(inst, channels)) != SRD_OK)
        {
            LOG_F(ERROR, "Couldn't set channels: Error code : %d", ret);
        }

        //Interleave data from acq channels which were used in channelMap into mixedData array
        double *mixedData = new double(sampleCount*setChannels.size());
        for(int i = 0; i<sampleCount; i++)
        {
            for(int y=0; y<setChannels.size; y++)
            {
                mixedData[(i*setChannels.size)+y]=dataMap[setChannels[y]][i];
            }

        }
        
        //parse mixedData to be in range uint_8:0-255

        //run decoder
        */
   }
}
