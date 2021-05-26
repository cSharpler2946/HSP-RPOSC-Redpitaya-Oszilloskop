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

#include <loguru.hpp>

#include "../lib/nlohmann/jsonWrapper.hpp"

#include "main.h"

//Signal size
#define SIGNAL_SIZE_DEFAULT      1024
#define SIGNAL_UPDATE_INTERVAL      1000
#define PARAMETER_UPDATE_INTERVAL      1000

std::vector<PContainer*> pContainerList;
std::vector<SContainer*> sContainerList;

static srd_session *srdSession;
static srd_decoder_inst *srdDecoderInst;
static Acquirer *activeAcquirer;
MeasuredData *measuredData;
SRDDecoderList *decoderList;

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
        LOG_F(INFO, "Creating srd_session object");
        srd_session_new(&srdSession);
    }
    //End: Tests

    //Set update intveral for signals
    CDataManager::GetInstance()->SetSignalInterval(SIGNAL_UPDATE_INTERVAL);
    CDataManager::GetInstance()->SetParamInterval(PARAMETER_UPDATE_INTERVAL);


    //Initiaize all PContainers and SContainers
    decoderList = new SRDDecoderList("SRD_DECODER_LIST", 256, "");
    sContainerList.push_back(decoderList);

    ACQRequestedOptions *acqOptions = new ACQRequestedOptions("ACQ_REQUESTED_OPTIONS", CBaseParameter::RW, "", false);
    pContainerList.push_back(acqOptions);

    Startup * startup = new Startup("WEBSOCKET_OPENED", CBaseParameter::RW, "", false, decoderList, acqOptions);
    pContainerList.push_back(startup);

    SRDRequestedOptions *reqOptions = new SRDRequestedOptions("SRD_REQUESTED_OPTIONS", 127, "", &srdDecoderInst);
    sContainerList.push_back(reqOptions);
    SRDChannels *srdChannels = new SRDChannels("SRD_CHANNELS", 16, "", &srdDecoderInst);
    sContainerList.push_back(srdChannels);
    AllOptionsValid *allOptionsValid = new AllOptionsValid("ALL_OPTIONS_VALID", CBaseParameter::RW, "", false);
    pContainerList.push_back(allOptionsValid);
    ChosenDecoder *chosenDecoder = new ChosenDecoder("CHOSEN_DECODER", CBaseParameter::RW, "", false, reqOptions, srdChannels, srdSession, &srdDecoderInst, allOptionsValid);
    pContainerList.push_back(chosenDecoder);
    SRDChosenOptions *chosenOptions = new SRDChosenOptions("SRD_CHOSEN_OPTIONS", CBaseParameter::RW, "", false, &srdDecoderInst, allOptionsValid);
    pContainerList.push_back(chosenOptions);
    ACQChoosenOptions *acquirerChosenOptions = new ACQChoosenOptions("ACQ_CHOSEN_OPTIONS", CBaseParameter::RW, "", false, allOptionsValid);
    pContainerList.push_back(acquirerChosenOptions);
    // Dummy daten for ACQChosenOptions
    /*
    ACQChoosenOptions *chosenOptions = new ACQChoosenOptions();
    chosenOptions->sampleRate = 1;
    chosenOptions->decimation = 1;
    chosenOptions->sampleCount = 16384;
    chosenOptions->pinState = 1;
    //start acquisition
    activeAcquirer = new Acquirer(chosenOptions); //TODO: Get parameter (ACQChosenOption)
    bool result = activeAcquirer->startAcq();
    // get and send data
    vector<double> data = activeAcquirer->getData(0);
    LOG_F(INFO, "%f", data[0]);
    measuredData = new MeasuredData("MEASURED_DATA", data.size(), "");
    measuredData->addData("Channel 1", data);
    LOG_F(INFO, "added data to measuredData");
    sContainerList.push_back(measuredData);
    */

    //usleep(1000);

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
    //LOG_F(INFO, "In UpdateSignals");
    //OnNewSignals();
    //decoderList->CreateDecoderList();
}

void UpdateParams(void){
    //LOG_F(INFO, "In UpdateParams");
    //OnNewParams();
}

/**
 * Callback function, which gets called when paramters changed.
 */
void OnNewParams(void){
    for(PContainer *curr: pContainerList)
    {
        curr->OnNew();
    }
}

/**
 * Callback function, which gets called when signals changed.
 */
void OnNewSignals(void){
    /* //No need to call this function, as it does not work to receive signals
    for(SContainer *curr: sContainerList)
    {
        curr->OnNew();
    }
    */
}

void PostUpdateSignals(void){}