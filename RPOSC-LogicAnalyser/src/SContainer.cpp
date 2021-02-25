#include "SContainer.hpp"
#include "../lib/loguru/loguru.hpp"

SContainer::SContainer(std::string _name, int _size, std::string _def_value) {
    VALUE = new CStringSignal(_name, _size, _def_value);
}

void SContainer::OnNew() {
    LOG_F(INFO, "Here in SContainer::OnNew");
    if(VALUE->IsValueChanged())
    {
        OnNewInternal();
        VALUE->Update();
    }
    
}