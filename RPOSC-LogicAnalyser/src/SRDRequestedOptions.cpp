#include "SRDRequestedOptions.hpp"
#include <string>

SRDRequestedOptions::SRDRequestedOptions(std::string _name, int _size, std::string _def_value, srd_decoder_inst *decoderInst):
SContainer(_name, _size, _def_value) {

}

void SRDRequestedOptions::Update() {  //again added void as dummy
    /*
    * Create vector of options of decoderInst instance
    * Update value with them
    */

    std::vector<std::string> options;
    options.push_back("{'id': 'baudrate', 'desc': 'Baud rate', 'default': 115200}");
    options.push_back("{'id': 'data_bits', 'desc': 'Data bits', 'default': 8, 'values': (5, 6, 7, 8, 9)}");
    options.push_back("{'id': 'parity', 'desc': 'Parity', 'default': 'none', 'values': ('none', 'odd', 'even', 'zero', 'one', 'ignore')}");
    options.push_back("{'id': 'stop_bits', 'desc': 'Stop bits', 'default': 1.0,'values': (0.0, 0.5, 1.0, 1.5)}");

    VALUE.Set(options);
}
