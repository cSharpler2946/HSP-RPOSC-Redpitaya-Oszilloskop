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
std::map<std::string, std::string> SRDChosenOptions::chosenOptions;

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
    SRDChosenOptions::chosenOptions = optionsMap;
    LOG_F(INFO, "Successfully set srd options");
    //printCurrentOptions(*decoderInst);
}