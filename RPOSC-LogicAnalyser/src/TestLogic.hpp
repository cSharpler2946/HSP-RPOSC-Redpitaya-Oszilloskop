#pragma once

#include "PContainer.hpp"
#include "Acquirer.hpp"
#include "SRDChannelMap.hpp"
#include <libsigrokdecode/libsigrokdecode.h>
#include "MeasuredData.hpp"
#define ToErr (srd_error_code)
#include "MeasuredData.hpp"
#include "AllOptionsValid.hpp"
#include "AnnotationData.hpp"
#include "../lib/nlohmann/jsonWrapper.hpp"
#include "ChosenDecoder.hpp"

enum MeasurementState{
    stopped,
    starting, //Frontend button "start" was pressed
    running //aquisition started
};

NLOHMANN_JSON_SERIALIZE_ENUM(MeasurementState, {
    {MeasurementState::stopped, "stopped"},
    {MeasurementState::starting, "starting"},
    {MeasurementState::running, "running"},
})

class TestLogic: public PContainer {
    public:
        TestLogic(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update, srd_session *_srdSession, srd_decoder_inst **_decoderInst, Acquirer *_acquirer, AllOptionsValid *_allOptionsValid, SRDChannelMap *_channelMap, MeasuredData *_measuredData, std::vector<SContainer*> *_sContainerList, ACQChoosenOptions *_acqChoosenOptions);
        void Update();
        void OnNewInternal();
        void runMeasurement();
        MeasurementState measurementState;
        Acquirer *acquirer;
        AllOptionsValid *allOptionsValid;
        SRDChannelMap *channelMap;
        srd_session *srdSession;
        srd_decoder_inst **decoderInst;
        MeasuredData *measuredData;
        std::vector<SContainer*> *sContainerList;
        ACQChoosenOptions *acqChoosenOptions;
};