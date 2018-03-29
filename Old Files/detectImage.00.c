#include "detectImage.h"
#include "pattern.h"



void DetectImageInit()
{
	PrevDir = FORWARD;
	InitPobeye2();
	InitI2C(I2C_100_KHZ);
	InitCameraPobeye2();
	InitLCD();	

	FrameFromCam = GetRGBFrame();
		
	InitGraphicBuffer( &ScreenBuffer,LCD_WIDTH,LCD_HEIGHT,ONE_BIT,LCD_Buffer);

	ClearGraphicBuffer(&ScreenBuffer);
}

DIRECTION SignRecognition()
{
		
	GrabRGBFrame();				
	BinaryRGBFrame(FrameFromCam); 
	Nb_Identify=IdentifyForm(FrameFromCam,ListOfForm,pattern);	
	for (i=0;i<Nb_Identify;i++)
	{
		switch (ListOfForm[i].id)
		{
			case IDP_0_CROSS:
				PrevDir = FORWARD;
				return FORWARD;
			//case IDP_1_BIGA:
				//PrevDir = TURNBACK;
				//return TURNBACK;
			case IDP_2_KING:
				PrevDir = RIGHT;
				return RIGHT;
			case IDP_3_TOWER:
				PrevDir = STOP;
				return STOP;			
			case IDP_5_TRIANGLE:
				PrevDir = LEFT;
				return LEFT;
			default:
				return FORWARD;//PrevDir;
		}				
	}		
	if (Nb_Identify == 0)
	{
		return FORWARD;//PrevDir;	
	}
	return FORWARD;
}