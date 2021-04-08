/* Red Pitaya C API example Acquiring a signal from a buffer
 * This application acquires a signal on a specific channel */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "rp.h"
#include <libsigrokdecode.h>

#define ToErr (srd_error_code)

int main(int argc, char **argv){

    //Libsigrokdecode init
    if ((ToErr srd_init(nullptr)) != SRD_OK)
    {
        printf("%s", "LibSigrokDecode init failed\n");
        return EXIT_FAILURE;
    }
    else
    {
        printf("LibSigrokDecode init success: Using version: %s\n", srd_lib_version_string_get());
    }
}
