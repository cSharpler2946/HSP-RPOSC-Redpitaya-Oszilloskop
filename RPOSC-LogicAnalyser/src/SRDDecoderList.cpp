#include "SRDDecoderList.hpp"
#include <libsigrokdecode/libsigrokdecode.h>
#include "../lib/loguru/loguru.hpp"

SRDDecoderList::SRDDecoderList(std::string _name, int _size, std::string _def_value):
SContainer(_name, _size, _def_value) {
    LOG_F(INFO, "SRDDecoder list instantiating");
    CreateDecoderList();
    Update();
}

void SRDDecoderList::Update() {
    /* TODO: Implement real function to convert decoder list into json bzw vector<string>
    * Run only one single time
    * Take decoderList and write elements to VALUE
    * Check if this can be done on system startup or after client connected
    */
    vector<string> decoders;
    decoders.push_back("{'id': 'I2C','name': 'I2C','longname': 'I squared C','desc': 'Synchronous open drain protocol with one controller and many peripherals.'}");
    decoders.push_back("{'id': 'UART','name': 'UART','longname': 'UART RS232','desc': 'Asynchronous point-to-point protocol. Very old.'}");

    //Update VALUE
    VALUE->Set(decoders);
}

void SRDDecoderList::CreateDecoderList() {
    /* TODO: Implement conversion from gslist to jsons.
    * !!!This function could probably be implemented to Update()!!!
    * Create decoderList
    * srd_decoder_load_all
    * srd_decoder_list
    * Loop gslist and put info into decoderList
    * Call Update function or set flag
    */
}
