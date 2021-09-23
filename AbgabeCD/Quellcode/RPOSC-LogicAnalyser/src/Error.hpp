#pragma once

#include "SContainer.hpp"
#include <loguru.hpp>
#include <string>
#include <list>

class Error: public SContainer {
    public:
        Error(std::string _name, int _size, std::string _def_value);
        void Update();
        //void callbackError(void* user_data, const loguru::Message& message); //It is not really possible to create callbacks as member functions
        int maxSize;
        std::list<std::string> errorList;
};