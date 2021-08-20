#include "MeasuredData.hpp"
#include "../lib/nlohmann/jsonWrapper.hpp"

MeasuredData::MeasuredData(std::string _name, int _size, std::string _def_value):
SContainer(_name, _size, _def_value) {

}

void MeasuredData::Update() {
    VALUE->Set(measuredData);
}

void MeasuredData::addData(std::string acqChannel, std::vector<float> data) {
   nlohmann::json tmp;
   tmp["acqChannel"]=acqChannel;
   tmp["data"]=data;

   //LOG_F(INFO, "Added measured data: %s", tmp.dump().c_str());

   measuredData.push_back(tmp.dump());

   Update();
}

void MeasuredData::resetData() {
    measuredData.clear();
    Update(); 
}