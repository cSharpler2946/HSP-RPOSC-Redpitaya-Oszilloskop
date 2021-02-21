#pragma once 

#include <SContainer.hpp>
#include <string>
#include <vector>

class MeasuredData: public SContainer {
    public:
        MeasuredData(std::string _name, int _size, std::string _def_value);
        void addData(std::string acqChannel, std::vector<double> data);
        void resetData();
        vector<string> measuredData;
};
