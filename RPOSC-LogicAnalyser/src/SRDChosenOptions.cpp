#include "SRDChosenOptions.hpp"
#include <string>
#include <vector>
#include "../lib/loguru/loguru.hpp"
#include "../lib/nlohmann/jsonWrapper.hpp"
#include <gmodule.h> //I think there are the glib functions

SRDChosenOptions::SRDChosenOptions(std::string _name, int _size, std::string _def_value, srd_decoder_inst *_decoderInst, AllOptionsValid *_allOptionsValid):
SContainer(_name, _size, _def_value) {
    LOG_F(INFO, "SRDChosenOptions instantiated");
    decoderInst = _decoderInst;
    allOptionsValid = _allOptionsValid;
}

void SRDChosenOptions::OnNewInternal() {
   //Data should theoretically look like this: {[{"id"="bla", "value"="blablub"}, ...]}
    LOG_F(INFO, "Received ChosenOptions; Setting...");
    std::vector<std::string> optionsV = VALUE->GetData();
    for(std::string curr : optionsV)
    {
        nlohmann::json currentOption = curr;
        if(setOption(currentOption["id"], currentOption["value"] == SRD_OK))
            allOptionsValid->setDecoderValidity(true);
    }
}

srd_error_code setOption(std::string id, std::string value)
{
    /* TODO: Testing
    GHashTable *table = g_hash_table_new(g_int_hash, g_int_equal);
    const gchar *gkey = id.c_str();
    //Searches srd_decoder_options for type string of option. Ignores options that can not be found in decoderInst->decoder->options
    GSList * tmp = g_slist_find_custom(decoderInst->decoder->options, gkey, [](gconstpointer elem, gconstpointer cmp){
        return strcmp(((srd_decoder_option *)elem)->id, (char *)cmp);
    });
    if (tmp == nullptr) {
        LOG_F(ERROR, "Supplied option could not be found in requested options: %s, Ignoring...", gkey);
        continue;
    }
    srd_decoder_option * p = static_cast<srd_decoder_option *>(tmp->data);

    GVariant * gvalue;
    if(g_variant_is_of_type(p->def, G_VARIANT_TYPE_INT64)) {
        //int64
        gvalue = g_variant_new_int64(stol(value));
    } else if (g_variant_is_of_type(p->def, G_VARIANT_TYPE_STRING)) {
        //string
        gvalue = g_variant_new_string(value.c_str());
    } else if (g_variant_is_of_type(p->def, G_VARIANT_TYPE_DOUBLE)) {
        //double
        gvalue = g_variant_new_double(stod(value));
    }

    if(gvalue == nullptr) {
        LOG_F(ERROR, "Error converting option to gvariant: %s, Ignoring...", gkey);
        continue;
    }

    g_hash_table_insert(table, (gpointer)gkey, (gpointer)gvalue);

    return ToErr srd_inst_option_set(decoderInst, table);
    */
}
