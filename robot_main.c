/**
 * Author: Joshua Newhouse
 */
 
#include <pob-eye.h>
#include <utils.h>
#include "scheduler.h"
#include "detectImage.h"
#include "actions.h"

/* Forward declarations */
void Init(void);

int main(void) {
	SendString("Beginning execution\n");
	Init();

	while(1);
	
	return 0;
}

/**
 * Init: This procedure calls all of the initialization procedures necessary
 */
void Init(void) {
	InitPobeye2();
	InitLCD();
	DetectImageInit();
	InitPobProto();
	SwitchOnAllServo();
	InitUART0(115200);
	SchedulerInit();
}