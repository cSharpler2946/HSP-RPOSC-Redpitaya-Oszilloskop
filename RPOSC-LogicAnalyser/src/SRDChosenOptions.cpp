#include "SRDChosenOptions.hpp"
#include <gmodule.h> //I think there are the glib functions

SRDChosenOptions::SRDChosenOptions(std::string _name, int _size, std::string _def_value, srd_decoder_inst *_decoderInst):
SContainer(_name, _size, _def_value) {
    decoderInst = _decoderInst;
}

SRDChosenOptions::OnNewInternal() {
    /*
    * Read options from json to map<string, string>
    * Call setOptions function
    * Set optionsValid if
    */
}

srd_error_code setOptions(srd_decoder_inst * inst, std::map<std::string,std::string> options)
{
    //Construct g_hash_table from options:map<>
    GHashTable *table = g_hash_table_new(g_int_hash, g_int_equal);
    for (const auto& name [key, value] : options)   // TODO: change name to a sensible name put it only in there to fix build error xD
    {
        const gchar *gkey = key.c_str();
        //Searches srd_decoder_options for type string of option. Ignores options that can not be found in inst->decoder->options
        GSList * tmp = g_slist_find_custom(inst->decoder->options, gkey, [](gconstpointer elem, gconstpointer cmp){
            return strcmp(((srd_decoder_option *)elem)->id, (char *)cmp);
        });
        if (tmp == nullptr) {
            cout << "Provided option not found: " << gkey << endl;
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
            cout << "Error converting option to gvariant: " << gkey << endl;
            continue;
        }

        g_hash_table_insert(table, (gpointer)gkey, (gpointer)gvalue);
    }

    return ToErr srd_inst_option_set(inst, table);
}
