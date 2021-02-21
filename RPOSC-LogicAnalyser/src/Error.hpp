#pragma once

#include "SContainer.hpp"
#include <string>
#include <list>

class Error: public SContainer {
    public:
        Error(std::string _name, int _size, std::string _def_value);
        void callbackError(void* user_data, const loguru::Message& message);
        int maxSize;
        std::List<string> errorList;
}