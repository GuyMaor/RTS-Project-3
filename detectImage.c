#include "detectImage.h"
#include "pattern.h"



void DetectImageInit()
{
//	PrevDir = FORWARD;
//	InitPobeye2();
	InitI2C(I2C_100_KHZ);
	InitCameraPobeye2();
//	InitLCD();	

	FrameFromCam = GetRGBFrame();
/*		
	InitGraphicBuffer( &ScreenBuffer,LCD_WIDTH,LCD_HEIGHT,ONE_BIT,LCD_Buffer);

	ClearGraphicBuffer(&ScreenBuffer);
*/
}

DIRECTION SignRecognition()
{
		
	GrabRGBFrame();				
	BinaryRGBFrame(FrameFromCam); 
	Nb_Identify=IdentifyForm(FrameFromCam,ListOfForm,pattern);
	
	//shapeAmmount is an array that keeps track
	//of how many instances of each shape is seen
	//in the picture.
	char shapeAmmount[NUM_DIR];
	for(i=0;i<NUM_DIR;i++)
	{
		shapeAmmount[i]=0;
	}		
	for (i=0;i<Nb_Identify;i++)
	{
		
		switch (ListOfForm[i].id)
		{
			case IDP_0_CROSS:
				shapeAmmount[FORWARD]++;
				break;
			case IDP_1_BIGA:
				shapeAmmount[TURNBACK]++;
				break;
			case IDP_2_KING:
				shapeAmmount[RIGHT]++;
				break;
			case IDP_3_TOWER:
				shapeAmmount[STOP]++;
				break;		
			case IDP_5_TRIANGLE:
				shapeAmmount[LEFT]++;
				break;	

		}				
	}
	
	DIRECTION bestDir = DEFAULT_DIR;
	for(i = 0; i < NUM_DIR; i++)
	{
		if(shapeAmmount[bestDir]<shapeAmmount[i])
			bestDir = i;
	}
	return bestDir;
	//if (Nb_Identify == 0)
	//{
	//	return FORWARD;//PrevDir;	
	//}
	//return FORWARD;
}