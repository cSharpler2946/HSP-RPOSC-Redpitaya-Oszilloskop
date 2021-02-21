#include "MeasuredData.hpp"
#include <nlohmann/json.hpp>

MeasuredData::MeasuredData(std::string _name, int _size, std::string _def_value):
SContainer(_name, _size, _def_value) {

}

void MeasuredData::Update() {
    VALUE->Set(measuredData);
}

void MeasuredData::addData(std::string acqChannel, std::vector<double> data) {
    /*
    * Convert the acqChannel and the data into a JSON
    * {acqChannel="CHannel1", data=[1,24, 1,345, ...]}
    */
   Update();
}

void MeasuredData::resetData() {
    measuredData.clear();
    Update(); 
}