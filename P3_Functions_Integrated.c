//
//	Created by Name on Date.
//

#include <pob-eye.h>

#include "detectImage.h"
#include "actions.h"



int main(void)
{
	//Initialize POB-Eye 2
	InitPobeye2();
	InitUART0(115200);

	DIRECTION cmd;
	McDir cm_ps = Horizon;
	int t = 0;
	int i = 0;
	

	DetectImageInit();

	// recognize signs on the map
	while(i < 5)
	{
	
		cmd = SignRecognition(); 

		MoveCamera( &cm_ps,&t);	

		MoveBot(&cmd);
		
		i++;

	}

	// move camera
	cm_ps = Up;
	t = 20;
	MoveCamera( &cm_ps,&t);

	cm_ps = Down;
	t = 2;
	MoveCamera( &cm_ps,&t);

	cm_ps = Horizon;
	t = 2;
	MoveCamera( &cm_ps,&t);

	// inspect left side
	cmd = Isp_Left;
	
	cmd = SignRecognition(); 

	MoveBot(&cmd);

	// insepct right side
	cmd = Isp_Right;
	
	cmd = SignRecognition(); 

	MoveBot(&cmd);

	

		
	return 0;
}