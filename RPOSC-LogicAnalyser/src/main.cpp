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

#include "Acquirer.hpp"
#include "SRDDecoderList.hpp"
#include "ChosenDecoder.hpp"
#include "SRDRequestedOptions.hpp"

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
    ChosenDecoder *chosenDecoder = new ChosenDecoder("ChosenDecoder", CBaseParameter::RW, false, 0, *reqOptions, srdDecoderInst);
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
    // Call Update UpdateFunctions of all signals.
    /*
    for(SContainer curr: sContainerList)
    {
        curr.Update();
    }
    */
}

void UpdateParams(void){
    // Call Update functions of all parameters.
    /*
    for(PContainer curr: pContainerList)
    {
        curr.Update();
    }
    */
}

/**
 * Callback function, which gets called when paramters changed.
 */
void OnNewParams(void){
    for(PContainer *curr: pContainerList)
    {
        curr->onNew();
    }
}

/**
 * Callback function, which gets called when signals changed.
 */
void OnNewSignals(void){
    for(SContainer *curr: sContainerList)
    {
        curr->onNew();
    }
}

void PostUpdateSignals(void){}
