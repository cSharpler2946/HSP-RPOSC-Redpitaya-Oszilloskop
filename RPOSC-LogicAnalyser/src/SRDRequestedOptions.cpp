#include "SRDRequestedOptions.hpp"
#include <string>
#include <vector>
#include <loguru.hpp>
#include "../lib/nlohmann/jsonWrapper.hpp"

SRDRequestedOptions::SRDRequestedOptions(std::string _name, int _size, std::string _def_value, srd_decoder_inst **_decoderInst):
SContainer(_name, _size, _def_value) {
    LOG_F(INFO, "SRDRequestedOptions instatiated");
    decoderInst=_decoderInst;
}

void SRDRequestedOptions::Update() { //TODO: Segmentation fault on second decoder change, or second time loading one decoder
    LOG_F(INFO, "Getting requested options");
    
    std::vector<std::string> options;
    GSList *i;
    for (i = (*decoderInst)->decoder->options; i; i = i->next) {
        nlohmann::json tmp;
        struct srd_decoder_option *p = static_cast<srd_decoder_option *>(i->data);
        const GVariantType *type = g_variant_get_type(p->def);
        const gchar * typeS = g_variant_type_peek_string(type);
        tmp["id"]=p->id;
        tmp["desc"]=p->desc;
        if(g_variant_is_of_type(p->def, G_VARIANT_TYPE_INT64)) {
            tmp["default"]=g_variant_get_uint64(p->def);
            std::vector<uint64_t> values;
            g_list_foreach(reinterpret_cast<GList *>(p->values), [](gpointer data, gpointer user_data){ ((std::vector<uint64_t>*)user_data)->push_back(g_variant_get_int64((GVariant*)data)); }, (gpointer)&values);
            tmp["values"]=values;
        } else if (g_variant_is_of_type(p->def, G_VARIANT_TYPE_STRING)) {
            tmp["default"]=g_variant_get_string(p->def, nullptr);
            std::vector<std::string> values;
            g_list_foreach(reinterpret_cast<GList *>(p->values), [](gpointer data, gpointer user_data){ ((std::vector<std::string>*)user_data)->push_back(g_variant_get_string((GVariant*)data, nullptr)); }, (gpointer)&values);
            tmp["values"]=values;
        } else if (g_variant_is_of_type(p->def, G_VARIANT_TYPE_DOUBLE)) {
            tmp["default"]=g_variant_get_double(p->def);
            std::vector<double> values;
            g_list_foreach(reinterpret_cast<GList *>(p->values), [](gpointer data, gpointer user_data){ ((std::vector<double>*)user_data)->push_back(g_variant_get_double((GVariant*)data)); }, (gpointer)&values);
            tmp["values"]=values;
        }
        options.push_back(tmp.dump());
    }
    VALUE->Set(options);
    
    /*
    std::vector<std::string> options;
    options.push_back("{\"id\": \"baudrate\", \"desc\": \"Baud rate\", \"default\": 115200}");
    options.push_back("{\"id\": \"data_bits\", \"desc\": \"Data bits\", \"default\": 8, \"values\": [5, 6, 7, 8, 9]}");
    options.push_back("{\"id\": \"parity\", \"desc\": \"Parity\", \"default\": \"none\", \"values\": [\"none\", \"odd\", \"even\", \"zero\", \"one\", \"ignore\"]}");
    options.push_back("{\"id\": \"stop_bits\", \"desc\": \"Stop bits\", \"default\": 1.0,\"values\": [0.0, 0.5, 1.0, 1.5]}");
    */

    VALUE->Set(options);
}
