#include "PContainer.hpp"

PContainer::PContainer(std::string name, CBaseParameter::AccessMode am, string defaultVal, int fpga_update){
    VALUE = new CStringParameter(name, am, defaultVal, fpga_update);
}

void PContainer::OnNew() {
    VALUE->Update();

    if(VALUE->IsValueChanged())
    {
        OnNewInternal();
    }
}