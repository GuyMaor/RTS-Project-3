/**
 * Author: Joshua Newhouse
 */
 
#include <pob-eye.h>
#include <utils.h>
#include "scheduler.h"

int main(void) {
	InitPobeye2();
	InitUART0(115200);

	SchedulerInit();

	while(1) {
		if(SporadicJobReady())
			SporadicJobExecute();
		else
			WaitMs(nextFrameTime - GetClock());
	}
	
	return 0;
}