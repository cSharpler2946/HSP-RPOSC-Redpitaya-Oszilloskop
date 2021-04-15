#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sysinfo.h>

#include "main.h"


const char *rp_app_desc(void)
{
    return (const char *)"RPOSC Logic Analyzer\n";
}

int rp_app_init(void)
{
    if (rp_Init() != RP_OK)
    {
        fprintf(stderr, "Red Pitaya API init failed!\n");
        return EXIT_FAILURE;
    }
    else
    {
        fprintf(stderr, "Red Pitaya API init failed!\n");
    }

   
    return 0;
}

int rp_app_exit(void)
{
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
    /*for(PContainer *curr: pContainerList)
    {
        curr->OnNew();
    }*/
}

/**
 * Callback function, which gets called when signals changed.
 */
void OnNewSignals(void){
    /*for(SContainer *curr: sContainerList)
    {
        curr->OnNew();
    }*/
}

void PostUpdateSignals(void){}
