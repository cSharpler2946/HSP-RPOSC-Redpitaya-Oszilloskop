#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <vector>

#include <gmodule.h>
#include <libsigrokdecode/libsigrokdecode.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "../lib/loguru/loguru.hpp"

#include "../lib/nlohmann/json.hpp"

#include "main.h"

// Define to cast int return into srd_error_codes
#define ToErr (srd_error_code)

//Signal size
#define SIGNAL_SIZE_DEFAULT      1024
#define SIGNAL_UPDATE_INTERVAL      10

const char* logfile = "log/debug.log";
vector<PContainer*> pContainerList;
vector<SContainer*> sContainerList;

static srd_session *srdSession;
static srd_decoder_inst *srdDecoderInst;
static Acquirer *activeAcquirer;

const char *rp_app_desc(void)
{
    return (const char *)"RPOSC Logic Analyzer\n";
}

int rp_app_init(void)
{
    fprintf(stderr, "Loading RPOSC Logic Analyzer\n");

    //Tests
    //loguru::init();
    loguru::add_file(logfile, loguru::Append, loguru::Verbosity_INFO);
    LOG_F(INFO, "THE app staaaarts maaaan....");
    //Testing if JSON Library works
    nlohmann::json j;

    if (rp_Init() != RP_OK)
    {
        fprintf(stderr, "Red Pitaya API init failed!\n");
        return EXIT_FAILURE;
    }
    else
    {
        fprintf(stderr, "Red Pitaya API init success!\n");
    }

    //Libsigrokdecode init
    if ((ToErr srd_init(nullptr)) != SRD_OK)
    {
        fprintf(stderr, "LibSigrokDecode init failed\n");
        return EXIT_FAILURE;
    }
    else
    {
        fprintf(stderr, "LibSigrokDecode init success: Using version: %s\n", srd_lib_version_string_get());
    }
    //End: Tests

    //Set update intveral for signals
    CDataManager::GetInstance()->SetSignalInterval(SIGNAL_UPDATE_INTERVAL);
    //TODO: How about paramters

    //Intitialize main app
    srd_session_new(&srdSession);

    //Initiaize all PContainers and SContainers
    SRDDecoderList *decoderList = new SRDDecoderList("SRDDecoderList", 256, "");
    sContainerList.push_back(decoderList);
    SRDRequestedOptions *reqOptions = new SRDRequestedOptions("SRDRequestedOptions", 127, "", srdDecoderInst);
    sContainerList.push_back(reqOptions);
    ChosenDecoder *chosenDecoder = new ChosenDecoder("ChosenDecoder", CBaseParameter::RW, "", false, reqOptions, srdDecoderInst);
    pContainerList.push_back(chosenDecoder);


    //activeAcquirer = new Acquirer(); //TODO: Get parameter (ACQChosenOption)

    usleep(100);

    return 0;
}

int rp_app_exit(void)
{
    fprintf(stderr, "Unloading RPOSC Logic Analyzer\n");


    rpApp_Release();
    return 0;
}

/* Internal functions */
int rp_set_params(rp_app_params_t *p, int len)
{
    return 0;
}


int rp_get_params(rp_app_params_t **p)
{
    return 0;
}

int rp_get_signals(float ***s, int *sig_num, int *sig_len)
{
    return 0;
}
/* Internal functions end */

void UpdateSignals(void){
  if(acquisition_pending && acquisition_complete)
    {
        acquisition_pending = acquisition_complete = false;
        rp_AcqGetOldestDataV(RP_CH_1, &buff_size, buff);
        std::vector<float> v(buff, buff + 16384);
        ss_signal_1.Set(v);
    }
}

void UpdateParams(void){
  uint32_t write_pointer;
    rp_AcqGetWritePointer(&write_pointer);
    write_pointer_parameter.SendValue(write_pointer);
    if(previous_write_pointer == write_pointer)
    {
        acquisition_complete = true;
    }
    previous_write_pointer = write_pointer;

    if(start_acquisition.IsNewValue())
    {
        start_acquisition.Set(false);
        start_acquisition.Update();
        rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);
        acquisition_pending = true;
    }
}

/**
 * Callback function, which gets called when paramters changed.
 */
void OnNewParams(void){}

/**
 * Callback function, which gets called when signals changed.
 */
void OnNewSignals(void){}

void PostUpdateSignals(void){}
