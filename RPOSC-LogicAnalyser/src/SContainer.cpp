#include "SContainer.hpp"
#include <loguru.hpp>

SContainer::SContainer(std::string _name, int _size, std::string _def_value) {
    LOG_F(INFO, "Instantiating SContainer \"%s\"", _name.c_str());
    VALUE = new CStringSignal(_name, CBaseParameter::RW, _size, _def_value);
}

void SContainer::OnNew() {
    LOG_F(ERROR, "OBSOLETE!! In SContainer::OnNew");
    /*
    LOG_F(INFO, "In SContainer::OnNew");
    VALUE->Update(); //Seems to have another implementation as the parameter update function
    if(VALUE->IsValueChanged())
    {
        OnNewInternal();
    }
    */
}
