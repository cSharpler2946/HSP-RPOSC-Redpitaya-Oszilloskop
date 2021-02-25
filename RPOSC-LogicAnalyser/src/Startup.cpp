#include "Startup.hpp"

Startup::Startup(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update, SRDDecoderList * _decoderList):
PContainer(name, am, defaultVal, fpga_update) {
    decoderList = _decoderList;
}
void Startup::OnNewInternal() {
    decoderList->CreateDecoderList();
}