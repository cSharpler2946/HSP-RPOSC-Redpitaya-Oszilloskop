#pragma once

#include "SContainer.hpp"
#include "../lib/loguru/loguru.hpp"
#include <string>
#include <list>

class Error: public SContainer {
    public:
        Error(std::string _name, int _size, std::string _def_value);
        void Update();
        static void callbackError(void* user_data, const loguru::Message& message);
        int maxSize;
        std::list<std::string> errorList;
};