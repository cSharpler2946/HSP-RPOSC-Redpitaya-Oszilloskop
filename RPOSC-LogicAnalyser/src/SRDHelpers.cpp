#include "SRDHelpers.hpp"
#include <loguru.hpp>
#include <cstring>
#include <libsigrokdecode/libsigrokdecode.h>
#include <stdio.h>

#define ToErr (srd_error_code)

/*
 * Sets the current options.
 *
 * @param inst Pointer to Decoder Instance
 * @param options New options as map<string, string>
 *
 * @return srd_error_code produced by srd_inst_option_set()
 */
srd_error_code SRDHelpers::setOptions(std::map<std::string,std::string> options, srd_decoder_inst * decoderInst)
{
    //Construct g_hash_table from options:map<>
    GHashTable *table = g_hash_table_new(g_int_hash, g_int_equal);
    for (const auto& elem : options)
    {
        auto &key = elem.first;
        auto &value = elem.second;
        const gchar *gkey = key.c_str();
        LOG_F(INFO, "Setting srdOption %s to %s", gkey, value.c_str());
        //Searches srd_decoder_options for type string of option. Ignores options that can not be found in inst->decoder->options
        GSList * tmp = g_slist_find_custom((decoderInst)->decoder->options, gkey, [](gconstpointer elem, gconstpointer cmp){
            return strcmp(((srd_decoder_option *)elem)->id, (char *)cmp);
        });
        if (tmp == nullptr) {
            LOG_F(INFO, "Provided option not found: %s", gkey);
            continue;
        }
        srd_decoder_option * p = static_cast<srd_decoder_option *>(tmp->data);

        GVariant * gvalue;
        if(g_variant_is_of_type(p->def, G_VARIANT_TYPE_INT64)) {
            //int64
            gvalue = g_variant_new_int64(stoll(value));
        } else if (g_variant_is_of_type(p->def, G_VARIANT_TYPE_STRING)) {
            //string
            gvalue = g_variant_new_string(value.c_str());
        } else if (g_variant_is_of_type(p->def, G_VARIANT_TYPE_DOUBLE)) {
            //double
            gvalue = g_variant_new_double(stod(value));
        }

        if(gvalue == nullptr) {
            LOG_F(ERROR, "Error converting option to gvariant: %s", gkey);
            continue;
        }

        g_hash_table_insert(table, (gpointer)gkey, (gpointer)gvalue);
    }

    return ToErr srd_inst_option_set((decoderInst), table);
}