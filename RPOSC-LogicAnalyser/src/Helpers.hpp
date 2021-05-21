#pragma once

#include <string>
#include "../lib/nlohmann/jsonWrapper.hpp"

class Helpers {
public:
    static std::string JSONtoString(nlohmann::json j);
};