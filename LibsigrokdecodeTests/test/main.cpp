#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <gmodule.h>
#include <libsigrokdecode/libsigrokdecode.h>

using namespace std;

void callback(struct srd_proto_data *pdata, void *cb_data)
{
    cout << "Received callback" << endl;
}

const char *decoderId = "uart";

int main() {
    cout << "Starting libsigrokdecode test application" << endl;
    //Setting the loglevel (0-5) of sigrokdecode. Does NOT log in order with rest of program!!
    srd_log_loglevel_set(5);
    //Print version
    cout << "libsigrokdecode version: " << srd_lib_version_string_get() << endl;

    srd_error_code err = (srd_error_code) srd_init(NULL);

    // List contains correct path "/usr/share/libsigrokdecode/decoders"
    // GSList * tmp = srd_searchpaths_get();

    //Create new decoder session
    srd_session *sess;
    srd_session_new(&sess);
    err = (srd_error_code) srd_session_start(sess);

    //Load an list all decoders
    //err = (srd_error_code) srd_decoder_load_all();
    //Load only one decoder by name
    err = (srd_error_code) srd_decoder_load(decoderId);
    const GSList *l = srd_decoder_list();
    //Get decoder by id: ids are unique identifier strings for each decoder. Can be found in "/usr/share/libsigrokdecode/decoders/<decoder>/pd.py"
    cout << srd_decoder_doc_get(srd_decoder_get_by_id(decoderId)) << endl;

    //Create protocol decoder instance
    srd_decoder_inst *inst = srd_inst_new(sess, decoderId, NULL);

    //Add samplerate to decoder instance TODO: Create GHashTable (GLib) containing options
    GHashTable *options = g_hash_table_new(g_int_hash, g_int_equal);
    g_hash_table_insert(options, "samplerate", "9800");



    //Generate Sample data
    uint8_t inbuf[1024];
    srand(time(NULL));
    for(int i = 0; i<1024; i++)
    {
        inbuf[i]=rand() % 255;
    }

    //Add callback
    err = (srd_error_code) srd_pd_output_callback_add(sess, SRD_OUTPUT_BINARY, &callback, NULL);

    //Send data to session
    err = (srd_error_code) srd_session_send(sess, 0, 1023, inbuf, 1024, 1);

    this_thread::sleep_for(chrono::milliseconds(10000));

    return 0;
}
