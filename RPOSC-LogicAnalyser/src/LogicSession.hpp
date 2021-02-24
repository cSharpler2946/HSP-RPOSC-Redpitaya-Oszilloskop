#pragma once

#include "PContainer.hpp"
#include "Acquirer.hpp"
#include <libsigrokdecode/libsigrokdecode.h>
#include "MeasuredData.hpp"
#include "../lib/nlohmann/jsonWrapper.hpp"

enum MeasurementState{
    stopped,
    starting, //Frontend button "start" was pressed
    running //aquisition started
};

NLOHMANN_JSON_SERIALIZE_ENUM(MeasurementState, {
    {stopped, "stopped"},
    {starting, "starting"},
    {running, "running"},
})

class LogicSession: public PContainer {
    public:
        LogicSession(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update, Acquirer *_acquirer, srd_session *_srdSession, MeasuredData *_measuredData);
        void Update();
        void OnNewInternal();
        void runMeasurement();
        MeasurementState measurementState;
        Acquirer *acquirer;
        srd_session *srdSession;
        MeasuredData *measuredData;
};