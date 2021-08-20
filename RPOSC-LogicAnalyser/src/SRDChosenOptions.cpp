#include "SRDChosenOptions.hpp"
#include <string>
#include <vector>
#include <loguru.hpp>
#include "../lib/nlohmann/jsonWrapper.hpp"
#include <gmodule.h> //I think there are the glib functions
#include "Helpers.hpp"
#include <Python.h>

#define ToErr (srd_error_code)

SRDChosenOptions::SRDChosenOptions(std::string name, CBaseParameter::AccessMode am, std::string defaultVal, int fpga_update, srd_decoder_inst **_decoderInst, AllOptionsValid *_allOptionsValid):
PContainer(name, am, defaultVal, fpga_update) {
    LOG_F(INFO, "SRDChosenOptions instantiated");
    decoderInst = _decoderInst;
    allOptionsValid = _allOptionsValid;
}

/*
 * !!!Terminates the app currently!!!!
 * Prints current options from python object.
 * **setOptions() must be called at least once before!!**
 * Supports only GVariantTypes s(string), x(int64) and d(double).
 *
 * @param inst Pointer to Decoder Instance.
 */
void printCurrentOptions(srd_decoder_inst * inst)
{
    PyObject *pyOptions = PyObject_GetAttrString(static_cast<PyObject *>(inst->py_inst), "options");
    if(PyTuple_Check(pyOptions)) {
        LOG_F(ERROR, "Error: Run setOptions() before using printCurrentOptions()");
        return;
    }
    GSList *i;
    for (i = inst->decoder->options; i; i = i->next) {
        struct srd_decoder_option *p = static_cast<srd_decoder_option *>(i->data);
        gchar *key = p->id;
        std::string value = "NOT SET";
        PyObject *tmpVal = PyDict_GetItemString(pyOptions, key);
        if(g_variant_is_of_type(p->def, G_VARIANT_TYPE_INT64)) {
            //int64
            value = std::to_string(PyLong_AsLong(tmpVal));
        } else if (g_variant_is_of_type(p->def, G_VARIANT_TYPE_STRING)) {
            //string
            value = PyUnicode_AsUTF8(tmpVal);
        } else if (g_variant_is_of_type(p->def, G_VARIANT_TYPE_DOUBLE)) {
            //double
            value = std::to_string(PyFloat_AsDouble(tmpVal));
        }
        LOG_F(INFO, "Current options %s: %s", key, value.c_str());
    }
}

void SRDChosenOptions::OnNewInternal() {
    //Data should theoretically look like this:  "[{\"id\":\"baudrate\",\"value\":115200}","{\"id\":\"num_data_bits\",\"value\":\"6\"}",...
    LOG_F(INFO, "Received SRDChosenOptions; Setting...");
    
    nlohmann::json tmp = nlohmann::json::parse(VALUE->Value());
    LOG_F(INFO, "JSON: %s", tmp.dump().c_str());
    std::map<std::string, std::string> optionsMap;
    for(nlohmann::json curr : tmp)
    {
        std::string id = curr["id"];
        std::string value = Helpers::JSONtoString(curr["value"]);
        optionsMap.insert(std::pair<std::string, std::string>(id, value));
    }
    if(setOptions(optionsMap) == SRD_OK)
    {
        allOptionsValid->setDecoderValidity(true);
        LOG_F(INFO, "Successfully set srd options");
    }
    else
    {
        LOG_F(ERROR, "Failed to set at least one srd option");
    }
    //printCurrentOptions(*decoderInst);
}

/*
 * Sets the current options.
 *
 * @param inst Pointer to Decoder Instance
 * @param options New options as map<string, string>
 *
 * @return srd_error_code produced by srd_inst_option_set()
 */
srd_error_code SRDChosenOptions::setOptions(std::map<std::string,std::string> options)
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
        GSList * tmp = g_slist_find_custom((*decoderInst)->decoder->options, gkey, [](gconstpointer elem, gconstpointer cmp){
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

    return ToErr srd_inst_option_set((*decoderInst), table);
}