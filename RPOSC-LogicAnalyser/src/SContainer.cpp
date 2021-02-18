#include "SContainer.hpp"

SContainer::SContainer(std::string _name, int _size, std::string _def_value) {
    VALUE = new CStringSignal(_name, _size, _def_value);
}

void SContainer::OnNew() {
    VALUE->Update();

    if(VALUE->Value() == true)
    {
        OnNewInternal();
    }
}