#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <gmodule.h>
#include <libsigrokdecode/libsigrokdecode.h>
#include <stdint.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#define ToErr (srd_error_code)

using namespace std;

void callbackAnnotation(struct srd_proto_data *pdata, void *cb_data)
{
    srd_proto_data_annotation *data = (srd_proto_data_annotation *)pdata->data;
    char **annString = (gchar **)g_slist_nth_data(pdata->pdo->di->decoder->annotations, data->ann_class); //double pointer!!
    printf("CB: %d-%d: %d: %s: %s\n", pdata->start_sample, pdata->end_sample, data->ann_class, *annString, *data->ann_text);

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

void printCurrentOptions(srd_decoder_inst * inst)
{
    PyObject *pyOptions = PyObject_GetAttrString(static_cast<PyObject *>(inst->py_inst), "options");
    GSList *i;
    for (i = inst->decoder->options; i; i = i->next) {
        struct srd_decoder_option *p = static_cast<srd_decoder_option *>(i->data);
        gchar *key = p->id;
        string value;
        PyObject *tmpVal = PyDict_GetItemString(pyOptions, key);
        value = Py_STRINGIFY(tmpVal);
        //TODO: Stringyfy does not work as expected.
        /*
        if(g_variant_is_of_type(p->def, G_VARIANT_TYPE_INT64)) {
            //int64
            PyFromLo
        } else if (g_variant_is_of_type(p->def, G_VARIANT_TYPE_STRING)) {
            //string
        } else if (g_variant_is_of_type(p->def, G_VARIANT_TYPE_DOUBLE)) {
            //double
        }
         */
        cout << value << endl;
    }
}

const char *decoderId = "counter";

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
    //err = ToErr srd_pd_output_callback_add(sess, SRD_OUTPUT_ANN, &callbackAnnotation, nullptr);
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

    printOptions(inst);

    //Add channel
    //GHashTable *channels = g_hash_table_new(g_int_hash, g_int_equal);
    //err = ToErr srd_inst_channel_set_all(inst, channels);

    //GArray *pinStates = g_array_new(false, true, 5);
    //srd_inst_initial_pins_set_all(inst, pinStates);

    printCurrentOptions(inst);

    //Add options to decoder instance
    const gchar *key = "data_edge";
    GVariant *value = g_variant_new("s","rising");
    GHashTable *options = g_hash_table_new(g_int_hash, g_int_equal);
    g_hash_table_insert(options, (gpointer) key, value);
    //Printing contents of gHashTable
    //g_hash_table_foreach(options, [](gpointer key, gpointer value, gpointer user_data){ cout << "Table:" << (char **)key << (char *)value << endl; } ,nullptr);
    err = ToErr srd_inst_option_set(inst, options);

    printCurrentOptions(inst);

    //Setting SRD_CONF_SAMPLERATE
    GVariant *metadata = g_variant_new ("t", 100000); //Following function only supports the uint64 (="t") data type!!
    err = ToErr srd_session_metadata_set(sess, SRD_CONF_SAMPLERATE, metadata);

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

    this_thread::sleep_for(chrono::milliseconds(10000));

    return 0;
}
