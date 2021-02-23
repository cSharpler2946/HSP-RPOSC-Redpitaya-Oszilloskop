#include "Error.hpp"
#include "../lib/loguru/loguru.hpp"
#include <vector>

Error::Error(std::string _name, int _size, std::string _def_value):
SContainer(_name, _size, _def_value) {
    LOG_F(INFO, "Loaded Error object");
    maxSize=_size;
    loguru::add_callback("network_logger", this->callbackError, nullptr, loguru::Verbosity_INFO);
    LOG_F(INFO, "Added error callback for verbosity INFO");
}

void Error::Update() {
    std::vector<std::string> v{ std::begin(errorList), std::end(errorList) }; //TODO: Check if this works. Reason: Use List to create a buffer, that is never bigger than maxSize, Evtl. create OnNewInternal to sync data with frontend
    VALUE->Set(v);
}

void Error::callbackError(void* user_data, const loguru::Message& message) {
    std::string completeMessage(message.preamble);
    completeMessage.append(message.indentation).append(message.prefix).append(message.message);
    errorList.push_back(completeMessage);
    if(errorList.size() > maxSize)
        errorList.pop_front();

    Update();
}