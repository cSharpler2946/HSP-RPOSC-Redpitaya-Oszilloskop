#include "Startup.hpp"
#include "../lib/loguru/loguru.hpp"

Startup::Startup(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update, SRDDecoderList * _decoderList):
PContainer(name, am, defaultVal, fpga_update) {
    decoderList = _decoderList;
    LOG_F(INFO, "Startup instantiated");
}
void Startup::OnNewInternal() {
    LOG_F(INFO, "Got startup from frontend -> Sending DecoderList");
    decoderList->CreateDecoderList();
}