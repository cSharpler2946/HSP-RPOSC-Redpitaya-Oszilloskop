#include <fstream>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sysinfo.h>

#include "main.h"

extern "C" {
    #include "rpApp.h"
}




CBooleanParameter ledState("LED_STATE", CBaseParameter::RW, false, 0);
CIntParameter ss_int_p("SS_INT_P", CBaseParameter::RW, 100,	0,	1,65535);
CFloatSignal ss_signal_1("SS_SIGNAL_1", 1000, 0.0f);
CFloatSignal ss_signal_2("SS_SIGNAL_2", 2 << 20, 0.0f);

const char *rp_app_desc(void)
{
    return (const char *)"Template application.\n";
}


int rp_app_init(void)
{
    srand(time(0));
    rp_Init();
    fprintf(stderr, "Loading template application\n");
    CDataManager::GetInstance()->SetParamInterval(100);
	CDataManager::GetInstance()->SetSignalInterval(1);
    ss_int_p.SendValue(100);
    for(int i = 0; i < 1000; i++)
    {
        ss_signal_1[i] = 
    }
    return 0;
}


int rp_app_exit(void)
{
    fprintf(stderr, "Unloading template application\n");
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

}


void UpdateParams(void)
{
    ss_int_p.SendValue(ss_int_p.Value() + 1);
}


void OnNewParams(void)
{
    ledState.Update();
    if(ledState.Value() == false)
    {
        rp_DpinSetState(RP_LED0, RP_LOW);
    }
    else
    {
        rp_DpinSetState(RP_LED0, RP_HIGH);
    }
}


void OnNewSignals(void){}


void PostUpdateSignals(void){}
