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

#include "../lib/nlohmann/jsonWrapper.hpp"

#include "main.h"

//Signal size
#define SIGNAL_SIZE_DEFAULT      1024
#define SIGNAL_UPDATE_INTERVAL      10
#define PARAMETER_UPDATE_INTERVAL      10

vector<PContainer*> pContainerList;
vector<SContainer*> sContainerList;

static srd_session *srdSession;
static srd_decoder_inst *srdDecoderInst;

CStringParameter *startup;
CStringSignal *decoderList;


const char *rp_app_desc(void)
{
    return (const char *)"RPOSC Logic Analyzer\n";
}

int rp_app_init(void)
{
    LOG_F(INFO, "Loading RPOSC Logic Analyzer\n");

    //Tests
    //loguru::init();
    //Testing if JSON Library works
    nlohmann::json j;

    if (rp_Init() != RP_OK)
    {
        LOG_F(INFO, "Red Pitaya API init failed!\n");
        return EXIT_FAILURE;
    }
    else
    {
        LOG_F(INFO, "Red Pitaya API init success!\n");
    }

    //Libsigrokdecode init
    if ((ToErr srd_init(nullptr)) != SRD_OK)
    {
        LOG_F(INFO, "LibSigrokDecode init failed\n");
        return EXIT_FAILURE;
    }
    else
    {
        LOG_F(INFO, "LibSigrokDecode init success: Using version: %s\n", srd_lib_version_string_get());
    }
    //End: Tests

    //Set update intveral for signals
    CDataManager::GetInstance()->SetSignalInterval(SIGNAL_UPDATE_INTERVAL);
    CDataManager::GetInstance()->SetParamInterval(PARAMETER_UPDATE_INTERVAL);


    //Initiaize all PContainers and SContainers
    startup = new CStringParameter("WEBSOCKET_OPENED", CBaseParameter::RW, "", false);
    decoderList = new CStringSignal("SRD_DECODER_LIST", 50, "");

    usleep(100);

    return 0;
}

int rp_app_exit(void)
{
    LOG_F(INFO, "Unloading RPOSC Logic Analyzer\n");

    srd_exit();

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
    LOG_F(INFO, "In UpdateSignals");
    //OnNewSignals();
}

void UpdateParams(void){
    LOG_F(INFO, "In UpdateParams");
    //OnNewParams();
}

/**
 * Callback function, which gets called when paramters changed.
 */
void OnNewParams(void){
    LOG_F(INFO, "In OnNewParams");
    if(startup->IsValueChanged())
    {
        LOG_F(INFO, "Creating and sending decoder list");
        vector<string> decoders;
        decoders.push_back("{\"id\": \"I2C\",\"name\": \"I2C\",\"longname\": \"I squared C\",\"desc\": \"Synchronous open drain protocol with one controller and many peripherals.\"}");
        decoders.push_back("{\"id\": \"UART\",\"name\": \"UART\",\"longname\": \"UART RS232\",\"desc\": \"Asynchronous point-to-point protocol. Very old.\"}");

        //Update VALUE
        VALUE->Set(decoders);

        startup->Update();
    }
}

/**
 * Callback function, which gets called when signals changed.
 */
void OnNewSignals(void){
    LOG_F(INFO, "In OnNewSignals");
}

void PostUpdateSignals(void){}
