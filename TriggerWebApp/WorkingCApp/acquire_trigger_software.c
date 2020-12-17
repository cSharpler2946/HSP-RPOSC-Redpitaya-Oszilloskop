/* Red Pitaya C API example Acquiring a signal from a buffer  
 * This application acquires a signal on a specific channel */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "rp.h"

int main(int argc, char **argv)
{
	if(rp_Init() != RP_OK){
		fprintf(stderr, "Rp api init failed!\n");
		return 0;
	}
	rp_AcqReset();
	//rp_AcqSetTriggerDelay(1);					//scope is allready stopped, since program exits with stopped scope
	//rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);
	uint32_t pos, tr_pos;
	rp_acq_trig_src_t src = 0;
	rp_acq_trig_state_t state = 0;
	int k = 3;
	while(k>0){
		usleep(10);
		--k;
		rp_AcqGetWritePointerAtTrig(&tr_pos);
		rp_AcqGetWritePointer(&pos);
		rp_AcqGetTriggerSrc(&src);
		rp_AcqGetTriggerState(&state);
		printf("pos: %d; pos when triggered: %d; trigger source: %d; trigger state: %d\n",pos, tr_pos, src, state);
	}
	rp_AcqSetTriggerDelay(16384);
	printf("start\n");
	rp_AcqStart();
	k=3;
	while(k>0){
		usleep(10);
		--k;
		rp_AcqGetWritePointerAtTrig(&tr_pos);
		rp_AcqGetWritePointer(&pos);
		rp_AcqGetTriggerSrc(&src);
		rp_AcqGetTriggerState(&state);
		printf("pos: %d; pos when triggered: %d; trigger source: %d; trigger state: %d\n",pos, tr_pos, src, state);
	}
	printf("trigger\n");
	rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);
	k=20;
	while(k>0){
		usleep(1);
		--k;
		rp_AcqGetWritePointerAtTrig(&tr_pos);
		rp_AcqGetWritePointer(&pos);
		rp_AcqGetTriggerSrc(&src);
		rp_AcqGetTriggerState(&state);
		printf("pos: %d; pos when triggered: %d; trigger source: %d; trigger state: %d\n",pos, tr_pos, src, state);
	}
	//scope is not acquiring samples to buffer anymore ->'stopped'
	rp_Release();
}

