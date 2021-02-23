#include "Error.hpp"
#include "../lib/loguru/loguru.hpp"
#include <vector>

void callbackError(void* user_data, const loguru::Message& message) {
    Error obj* = (Error*) user_data;
    std::string completeMessage(message.preamble);
    completeMessage.append(message.indentation).append(message.prefix).append(message.message);
    obj->errorList.push_back(completeMessage);
    if(obj->errorList.size() > obj->maxSize)
        obj->errorList.pop_front();

    obj->Update();
}

Error::Error(std::string _name, int _size, std::string _def_value):
SContainer(_name, _size, _def_value) {
    LOG_F(INFO, "Loaded Error object");
    maxSize=_size;
    loguru::add_callback("network_logger", callbackError, this, loguru::Verbosity_INFO);
    LOG_F(INFO, "Added error callback for verbosity INFO");
}

void Error::Update() {
    std::vector<std::string> v{ std::begin(errorList), std::end(errorList) }; //TODO: Check if this works. Reason: Use List to create a buffer, that is never bigger than maxSize, Evtl. create OnNewInternal to sync data with frontend
    VALUE->Set(v);
}