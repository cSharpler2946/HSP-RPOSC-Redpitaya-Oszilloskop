#include "PContainer.hpp"
#include "../lib/loguru/loguru.hpp"

PContainer::PContainer(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update){
    VALUE = new CStringParameter(name, am, defaultVal, fpga_update);
}

void PContainer::OnNew() {
    LOG_F(INFO, "Here in PContainer::OnNew");
    if(VALUE->IsValueChanged())
    {
        OnNewInternal();
        VALUE->Update();
    }
    
}