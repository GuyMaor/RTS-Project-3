//
//	Created by Name on Date.
//

#include <pob-eye.h>

//#include "directions.h"

#include "detectImage.h"
#include "actions.h"



int main(void)
{
	//Initialize POB-Eye 2
	InitPobeye2();
	InitUART0(115200);

	DIRECTION cmd;
	McDir cm_ps = Horizon;
	int t  = 0;
DetectImageInit();
	while(1)
	{
	
	cmd = SignRecognition(); 

	MoveCamera( &cm_ps,&t);	

	MoveBot(&cmd);

	}
	
	return 0;
}