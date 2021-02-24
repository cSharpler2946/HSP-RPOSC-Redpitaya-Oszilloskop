#include "SRDRequestedOptions.hpp"
#include <string>
#include <vector>
#include "../lib/loguru/loguru.hpp"
#include "../lib/nlohmann/jsonWrapper.hpp"

SRDRequestedOptions::SRDRequestedOptions(std::string _name, int _size, std::string _def_value, srd_decoder_inst *decoderInst):
SContainer(_name, _size, _def_value) {
    LOG_F(INFO, "SRDRequestedOptions instatiated");
}

void SRDRequestedOptions::Update() {
    LOG_F(INFO, "Getting requested options");
    
    /* Implementation to test
    std::vector<std::string> options;
    GSList *i;
    for (i = decoderInst->decoder->options; i; i = i->next) {
        nlohmann::json tmp;
        struct srd_decoder_option *p = static_cast<srd_decoder_option *>(i->data);
        const GVariantType *type = g_variant_get_type(p->def);
        const gchar * typeS = g_variant_type_peek_string(type);
        tmp["id"]=p->id;
        if(g_variant_is_of_type(p->def, G_VARIANT_TYPE_INT64)) {
            tmp["default"]=g_variant_get_uint64(p->def);
            vector<uint64_t> values; //TODO: Test if you can access this in lamba function... Otherwise use user_data
            g_list_foreach(reinterpret_cast<GList *>(p->values), [](gpointer data, gpointer user_data){ values.push_back(g_variant_get_int64((GVariant*)data)); }, nullptr);
            tmp["values"]=values;
        } else if (g_variant_is_of_type(p->def, G_VARIANT_TYPE_STRING)) {
            tmp["default"]=g_variant_get_string(p->def, nullptr);
            vector<std::string> values;
            g_list_foreach(reinterpret_cast<GList *>(p->values), [](gpointer data, gpointer user_data){ values.push_back(g_variant_get_string((GVariant*)data, nullptr)); }, nullptr);
            tmp["values"]=values;
        } else if (g_variant_is_of_type(p->def, G_VARIANT_TYPE_DOUBLE)) {
            tmp["default"]=g_variant_get_double(p->def);
            vector<double> values;
            g_list_foreach(reinterpret_cast<GList *>(p->values), [](gpointer data, gpointer user_data){ values.push_back(g_variant_get_double((GVariant*)data)); }, nullptr);
            tmp["values"]=values;
        }
        options.push_back(tmp.dump());
    }
    VALUE->Set(options);
    */

    std::vector<std::string> options;
    options.push_back("{'id': 'baudrate', 'desc': 'Baud rate', 'default': 115200}");
    options.push_back("{'id': 'data_bits', 'desc': 'Data bits', 'default': 8, 'values': (5, 6, 7, 8, 9)}");
    options.push_back("{'id': 'parity', 'desc': 'Parity', 'default': 'none', 'values': ('none', 'odd', 'even', 'zero', 'one', 'ignore')}");
    options.push_back("{'id': 'stop_bits', 'desc': 'Stop bits', 'default': 1.0,'values': (0.0, 0.5, 1.0, 1.5)}");

    VALUE->Set(options);
}
