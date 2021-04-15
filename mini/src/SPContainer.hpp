#pragma once

#include <string>
#include <loguru.hpp>

class SPContainer {
    public:
        //virtual std::string toJsonString()=0;
        //virtual void fromJsonString(std::string)=0;
        virtual void OnNew()=0;
        virtual void OnNewInternal(){LOG_F(INFO, "In SPContainer::OnNewInternal -- Unimplemented receive funtion!!");};
        virtual void Update(){};
};