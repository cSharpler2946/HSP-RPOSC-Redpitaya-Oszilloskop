#include "SRDChosenOptions.hpp"
#include <string>
#include <vector>
#include <loguru.hpp>
#include "../lib/nlohmann/jsonWrapper.hpp"
#include <gmodule.h> //I think there are the glib functions
#include "Helpers.hpp"

#define ToErr (srd_error_code)

SRDChosenOptions::SRDChosenOptions(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update, srd_decoder_inst **_decoderInst, AllOptionsValid *_allOptionsValid):
PContainer(name, am, defaultVal, fpga_update) {
    LOG_F(INFO, "SRDChosenOptions instantiated");
    decoderInst = _decoderInst;
    allOptionsValid = _allOptionsValid;
}

void SRDChosenOptions::OnNewInternal() {
    //Data should theoretically look like this:  "[{\"id\":\"baudrate\",\"value\":115200}","{\"id\":\"num_data_bits\",\"value\":\"6\"}",...
    LOG_F(INFO, "Received SRDChosenOptions; Setting...");
    
    nlohmann::json tmp = nlohmann::json::parse(VALUE->Value());
    LOG_F(INFO, "JSON: %s", tmp.dump().c_str());
    for(nlohmann::json curr : tmp)
    {
        std::string id = curr["id"];
        std::string value = Helpers::JSONtoString(curr["value"]);
        LOG_F(INFO, "Setting option %s to %s", id.c_str(), value.c_str());
        if(setOption(id, value) == SRD_OK)
        {
            LOG_F(INFO, "Successfully set srd option");
            allOptionsValid->setDecoderValidity(true);
        }
        else
        {
            LOG_F(ERROR, "Failed to set srd option");
        }
    }
}

srd_error_code SRDChosenOptions::setOption(std::string id, std::string value)
{
    GHashTable *table = g_hash_table_new(g_int_hash, g_int_equal);
    const gchar *gkey = id.c_str();
    //Searches srd_decoder_options for type string of option. Ignores options that can not be found in decoderInst->decoder->options
    GSList * tmp = g_slist_find_custom((*decoderInst)->decoder->options, gkey, [](gconstpointer elem, gconstpointer cmp){
        return strcmp(((srd_decoder_option *)elem)->id, (char *)cmp);
    });
    if (tmp == nullptr) {
        LOG_F(ERROR, "Supplied option could not be found in requested options: %s, Ignoring...", gkey);
        return SRD_ERR_ARG;
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
        return SRD_ERR_ARG;
    }

    g_hash_table_insert(table, (gpointer)gkey, (gpointer)gvalue);

    return ToErr srd_inst_option_set((*decoderInst), table);
}