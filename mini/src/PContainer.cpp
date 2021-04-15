#include "PContainer.hpp"
#include <loguru.hpp>

PContainer::PContainer(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update){
    VALUE = new CStringParameter(name, am, defaultVal, fpga_update);
}

void PContainer::OnNew() {
    LOG_F(INFO, "In PContainer::OnNew");
    VALUE->Update();
    if(VALUE->IsValueChanged())
    {
        OnNewInternal();
    }

}
