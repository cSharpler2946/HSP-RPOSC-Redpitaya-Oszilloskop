#pragma once

// Define to cast int return into srd_error_codes
#define ToErr (srd_error_code)

#include <DataManager.h>
#include <CustomParameters.h>
#include "rpApp.h"

#include "CStringSignal.h"

#include "Acquirer.hpp"
#include "SRDDecoderList.hpp"
#include "ChosenDecoder.hpp"
#include "SRDRequestedOptions.hpp"
#include "SRDChannels.hpp"
#include "Startup.hpp"
#include "MeasuredData.hpp"

#ifdef __cplusplus
extern "C" {
#endif


/* Parameters description structure - must be the same for all RP controllers */
typedef struct rp_app_params_s {
    char  *name;
    float  value;
    int    fpga_update;
    int    read_only;
    float  min_val;
    float  max_val;
} rp_app_params_t;



/* module entry points */
/**
 * Function contains description of the app.
 * @return Description of the app.
*/
const char *rp_app_desc(void);
/**
 * Initialization function: Get called at app startup.
 */
int rp_app_init(void);
/**
 * Exit function: Gets called at app exit.
 */
int rp_app_exit(void);
/**
 * RP internal function for keeping parameters in sync.
 */
int rp_set_params(rp_app_params_t *p, int len);
/**
 * RP internal function for keeping parameters in sync.
 */
int rp_get_params(rp_app_params_t **p);
/**
 * RP internal function for keeping signals in sync.
 */
int rp_get_signals(float ***s, int *sig_num, int *sig_len);


#ifdef __cplusplus
}
#endif