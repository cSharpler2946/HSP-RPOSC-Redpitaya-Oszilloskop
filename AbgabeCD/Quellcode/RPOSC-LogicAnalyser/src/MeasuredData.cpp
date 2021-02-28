#include "MeasuredData.hpp"
#include "../lib/nlohmann/jsonWrapper.hpp"

MeasuredData::MeasuredData(std::string _name, int _size, std::string _def_value):
SContainer(_name, _size, _def_value) {

}

void MeasuredData::Update() {
    VALUE->Set(measuredData);
}

void MeasuredData::addData(std::string acqChannel, std::vector<double> data) {
   nlohmann::json tmp;
   tmp["acqChannel"]=acqChannel;
   tmp["data"]=data;

   measuredData.push_back(tmp.dump());

   Update();
}

void MeasuredData::resetData() {
    measuredData.clear();
    Update(); 
}