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
		SendString("Waiting\n");
		if(SporadicJobReady()) {
			SendString("Sporadic job ready\n");
			SporadicJobExecute();
		}
	}
	
	return 0;
}