#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <vector>
#include <DataManager.h>
#include <CustomParameters.h>
#include "rpApp.h"

#include "main.h"

#define SIGNAL_SIZE_DEFAULT 1024
#define SIGNAL_UPDATE_INTERVAL      10

CFloatSignal ss_signal_1("VOLTAGE", 16384, 0.0f);
CIntParameter write_pointer_parameter("WRITE_POINTER", CBaseParameter::AccessMode::RW, 0, false, 0, 20000);
CBooleanParameter start_acquisition("START_ACQUISITION", CBaseParameter::AccessMode::RW, false, false);

bool acquisition_pending = false;
bool acquisition_complete = false;

uint32_t buff_size = 16382;
float buff[16384];

uint32_t previous_write_pointer = 0;

const char *rp_app_desc(void)
{
    return (const char *)"Template application.\n";
}


int rp_app_init(void)
{
    fprintf(stderr, "Loading read voltage application\n");

    if (rp_Init()/*App_Init()*/ != RP_OK) 
    {
        fprintf(stderr, "Red Pitaya API init failed!\n");
        return EXIT_FAILURE;
    }
    else fprintf(stderr, "Red Pitaya API init success!\n");

    //rpApp_Reset();

    //Set signal update interval
    CDataManager::GetInstance()->SetSignalInterval(SIGNAL_UPDATE_INTERVAL);
    CDataManager::GetInstance()->SetParamInterval(SIGNAL_UPDATE_INTERVAL);
    
    rp_AcqReset();
    usleep(50);
    rp_AcqSetDecimation(RP_DEC_8192);
    rp_AcqSetTriggerDelay(8192);
    rp_AcqSetGain(RP_CH_1, RP_HIGH);
    rp_AcqStart();
    usleep(100);
    

    return 0;
}


int rp_app_exit(void)
{
    fprintf(stderr, "Unloading read voltage application\n");

    rp_Release();

    return 0;
}


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








void UpdateSignals(void)
{
    if(acquisition_pending && acquisition_complete)
    {
        acquisition_pending = acquisition_complete = false;
        rp_AcqGetOldestDataV(RP_CH_1, &buff_size, buff);
        std::vector<float> v(buff, buff + 16384);
        ss_signal_1.Set(v);
    }
}


void UpdateParams(void)
{
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

void OnNewParams(void) {}


void OnNewSignals(void){}


void PostUpdateSignals(void){}