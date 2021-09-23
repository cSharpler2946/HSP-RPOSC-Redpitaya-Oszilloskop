#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <gmodule.h>
#include <libsigrokdecode/libsigrokdecode.h>
#include <stdint.h>
#include <map>
#include <nlohmann/json.hpp>
#include <fstream>

#include "uartTestDataNew.h"

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#define ToErr (srd_error_code)

using namespace std;

nlohmann::json allAnnotations;
void callbackAnnotation(struct srd_proto_data *pdata, void *cb_data)
{
    srd_proto_data_annotation *data = (srd_proto_data_annotation *)pdata->data;
    char **annString = (gchar **)g_slist_nth_data(pdata->pdo->di->decoder->annotations, data->ann_class); //double pointer!!
    //printf("CB: %d-%d: %d: %s: %s\n", pdata->start_sample, pdata->end_sample, data->ann_class, *annString, *data->ann_text);

    string s = *annString;
    if(s == "rx-data") {
        printf("%s", *data->ann_text);
    }

    /*
    nlohmann::json currentAnn;
    currentAnn["start"]=pdata->start_sample;
    currentAnn["end"]=pdata->end_sample;
    currentAnn["annotationClass"]=*annString;
    currentAnn["annotationText"]=*data->ann_text;

    //printf("%s\n", currentAnn.dump().c_str());

    allAnnotations.push_back(currentAnn);
     */
}

void callbackBinary(struct srd_proto_data *pdata, void *cb_data)
{
    int annIndex = *(int*)pdata->data;
    char **annString = (gchar **)g_slist_nth_data(pdata->pdo->di->decoder->annotations, annIndex); //double pointer!!
    printf("CB: %d-%d: %d: %s\n", pdata->start_sample, pdata->end_sample, annIndex, *annString);
}

/*
 * Prints options from .py file. NOT the current options!
 * Supports only GVariantTypes s(string), x(int64) and d(double).
 * Currently has glib error, but works: g_variant_get_uint64: assertion 'g_variant_is_of_type (value, G_VARIANT_TYPE_UINT64)' failed
 *
 * @param inst Pointer to Decoder Instance
 */
void printOptions(srd_decoder_inst * inst)
{
    GSList *i;
    for (i = inst->decoder->options; i; i = i->next) {
        struct srd_decoder_option *p = static_cast<srd_decoder_option *>(i->data);
        const GVariantType *type = g_variant_get_type(p->def);
        const gchar * typeS = g_variant_type_peek_string(type);
        cout << p->id << ":" << typeS << ":";
        if(g_variant_is_of_type(p->def, G_VARIANT_TYPE_INT64)) {
            cout << g_variant_get_uint64(p->def) << ":";
            g_list_foreach(reinterpret_cast<GList *>(p->values), [](gpointer data, gpointer user_data){ cout << g_variant_get_int64((GVariant*)data) << ","; }, nullptr);
        } else if (g_variant_is_of_type(p->def, G_VARIANT_TYPE_STRING)) {
            cout << g_variant_get_string(p->def, nullptr) << ":";
            g_list_foreach(reinterpret_cast<GList *>(p->values), [](gpointer data, gpointer user_data){ cout << g_variant_get_string((GVariant*)data, nullptr) << ","; }, nullptr);
        } else if (g_variant_is_of_type(p->def, G_VARIANT_TYPE_DOUBLE)) {
            cout << g_variant_get_double(p->def) << ":";
            g_list_foreach(reinterpret_cast<GList *>(p->values), [](gpointer data, gpointer user_data){ cout << g_variant_get_double((GVariant*)data) << ","; }, nullptr);
        }
        cout << endl;
    }
}

/*
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
        cout << "Error: Run setOptions() before using printCurrentOptions()" << endl;
        return;
    }
    GSList *i;
    for (i = inst->decoder->options; i; i = i->next) {
        struct srd_decoder_option *p = static_cast<srd_decoder_option *>(i->data);
        gchar *key = p->id;
        string value;
        PyObject *tmpVal = PyDict_GetItemString(pyOptions, key);
        if(g_variant_is_of_type(p->def, G_VARIANT_TYPE_INT64)) {
            //int64
            value = to_string(PyLong_AsLong(tmpVal));
        } else if (g_variant_is_of_type(p->def, G_VARIANT_TYPE_STRING)) {
            //string
            value = PyUnicode_AsUTF8(tmpVal);
        } else if (g_variant_is_of_type(p->def, G_VARIANT_TYPE_DOUBLE)) {
            //double
            value = to_string(PyFloat_AsDouble(tmpVal));
        }
        cout << key << ":" <<value << endl;
    }
}

/*
 * Sets the current options.
 *
 * @param inst Pointer to Decoder Instance
 * @param options New options as map<string, string>
 *
 * @return srd_error_code produced by srd_inst_option_set()
 */
srd_error_code setOptions(srd_decoder_inst * inst, map<string,string> options)
{
    //Construct g_hash_table from options:map<>
    GHashTable *table = g_hash_table_new(g_int_hash, g_int_equal);
    for (const auto& [key, value] : options)
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

srd_error_code setSampleRate(srd_session* sess, uint64_t rate)
{
    GVariant *metadata = g_variant_new ("t", rate); //Following function only supports the uint64 (="t") data type!!
    return ToErr srd_session_metadata_set(sess, SRD_CONF_SAMPLERATE, metadata);
}

const char *decoderId = "uart";

int main() {
    cout << "Starting libsigrokdecode test application" << endl;
    //Setting the loglevel (0-5) of sigrokdecode. Does NOT log in order with rest of program!!
    //srd_log_loglevel_set(5);
    //Print version
    cout << "libsigrokdecode version: " << srd_lib_version_string_get() << endl;

    srd_error_code err = ToErr srd_init(nullptr);

    // List contains correct path "/usr/share/libsigrokdecode/decoders"
    // GSList * tmp = srd_searchpaths_get();

    //Create new decoder session
    srd_session *sess;
    srd_session_new(&sess);
    //Add callback
    err = ToErr srd_pd_output_callback_add(sess, SRD_OUTPUT_ANN, &callbackAnnotation, nullptr);
    //err = ToErr srd_pd_output_callback_add(sess, SRD_OUTPUT_BINARY, &callbackBinary, nullptr);

    //Load and list all decoders
    //err = ToErr srd_decoder_load_all();
    //Load only one decoder by name
    err = ToErr srd_decoder_load(decoderId);
    const GSList *l = srd_decoder_list();
    //Get decoder by id: ids are unique identifier strings for each decoder. Can be found in "/usr/share/libsigrokdecode/decoders/<decoder>/pd.py"
    //cout << srd_decoder_doc_get(srd_decoder_get_by_id(decoderId)) << endl;

    //Create protocol decoder instance
    srd_decoder_inst *inst = srd_inst_new(sess, decoderId, nullptr);

    cout << "--- Available options:" << endl;
    printOptions(inst);

    cout << "--- Opt channels:" << endl;
    GSList * i;
    for (i = inst->decoder->opt_channels; i; i = i->next)
    {
        srd_channel * ch = static_cast<srd_channel *>(i->data);
        cout << ch->id << endl;
    }

    //Add channel
    GHashTable * channels = g_hash_table_new(g_int_hash, g_int_equal);
    GVariant * gkey = (GVariant *)("rx"); //Looks like they raped glib...
    GVariant * gvalue = g_variant_new_int32(0);
    g_hash_table_insert(channels, gkey, gvalue);
    err = ToErr srd_inst_channel_set_all(inst, channels);

    //GArray *pinStates = g_array_new(false, true, 5);
    //srd_inst_initial_pins_set_all(inst, pinStates);

    //Add options to decoder instance
    map<string, string> newOptions = {{"baudrate", "4800"}, {"data_bits", "8"},
                                      {"parity", "even"}, {"stop_bits", "1"},
                                      {"bit_order", "lsb-first"}, {"format", "ascii"}};
    err = setOptions(inst, newOptions);

    cout << "--- Current options:" << endl;
    printCurrentOptions(inst);

    //Setting SRD_CONF_SAMPLERATE
    err = setSampleRate(sess, 15258);

    /*
    //Generate Sample data
    uint8_t inbuf[1024];
    srand(time(nullptr));
    for(int i = 0; i<1024; i++)
    {
        inbuf[i]=rand() % 255;
    }

    //Start session
    err = ToErr srd_session_start(sess);

    //Send data to session
    err = ToErr srd_session_send(sess, 0, 1023, inbuf, 1024, 1);
    */

    //Prepare UART data
    cout << "--- Prepare data:" << endl;
    double min = DBL_MAX;
    double max = -DBL_MAX;
    for(double curr: testdata)
    {
        if(curr < min)
            min = curr;
        if(curr > max)
            max = curr;
    }
    int arrsize = sizeof(testdata)/sizeof(testdata[0]);
    cout << "Range: " << min << " to " << max << ", Number of samples: " << arrsize << endl;
    double step = UINT8_MAX/max-min;
    uint8_t * inbuf = new uint8_t[arrsize];
    for(int i = 0; i<arrsize; i++)
    {
        inbuf[i] = (uint8_t)testdata[i]*step;
        //cout << (int)inbuf[i] << endl;
    }

    //Start session
    err = ToErr srd_session_start(sess);

    //Send prepared test data to decoder
    err = ToErr srd_session_send(sess, 0, arrsize-1, inbuf, arrsize, 1);

    this_thread::sleep_for(chrono::milliseconds(5000));

    printf("%s\n", allAnnotations.dump().c_str());
    fstream file;
    file.open("annotationData.txt", ios::out);
    file << allAnnotations.dump();
    file.close();

    return 0;
}
