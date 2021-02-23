#include "LogicSession.hpp"


LogicSession::LogicSession(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update, Acquirer *_acquirer, srd_session *_srdSession, MeasuredData *_measuredData):
PContainer(name, am, defaultVal, fpga_update) {
    acquirer = _acquirer;
    srdSession = _srdSession;
    measuredData = _measuredData;
}

void LogicSession::Update() {
    nlohmann::json tmp;
    tmp["measurementState"]=measurementState; //Should use strings because of declaration "NLOHMANN_JSON_SERIALIZE_ENUM" in .hpp

    VALUE->Set(tmp.dump());
}

void LogicSession::OnNewInternal() {
    nlohmann::json tmp = VALUE->value;
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
   }
}