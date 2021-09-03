#include <libsigrokdecode/libsigrokdecode.h>
#include <map>
#include <string>

class SRDHelpers{
    public:
    static srd_error_code setOptions(std::map<std::string,std::string> options, srd_decoder_inst * decoderInst);
};