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
#define SIGNAL_SIZE_DEFAULT      1


//Signal
CFloatSignal VOLTAGE("VOLTAGE", SIGNAL_SIZE_DEFAULT, 0.0f);


//Parameter
CBooleanParameter READ_VALUE("READ_VALUE", CBaseParameter::RW, false, 0);






const char *rp_app_desc(void)
{
    return (const char *)"RPOSC Logic Analyzer\n";
}


int rp_app_init(void)
{
    fprintf(stderr, "Loading RPOSC Logic Analyzer\n");
    loguru::init();

    //Testing if JSON Library works
    nlohmann::json j;

    // Initialization of API
    // TODO: Change to rp_Init 
    if (rpApp_Init() != RP_OK) 
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




void UpdateSignals(void){}


void UpdateParams(void){}

/**
 * Callback function, which gets called when paramters changed.
 */
void OnNewParams(void)
{
    READ_VALUE.Update();

    if (READ_VALUE.Value() == true)
    {
        float val;
    
        //Read data from pin
        rp_AIpinGetValue(0, &val);

        //Write data to signal
        VOLTAGE[0] = val;

        //Reset READ value
        READ_VALUE.Set(false);
    }
}

/**
 * Callback function, which gets called when signals changed.
 */
void OnNewSignals(void){}


void PostUpdateSignals(void){}