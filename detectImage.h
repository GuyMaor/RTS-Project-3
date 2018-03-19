#ifndef DETECT_IMAGE
#define DETECT_IMAGE
#ifndef DIRH
#define DIRH
#include "directions.h"
#endif
#include <pob-eye.h>

UInt8 i;
UInt8 Nb_Identify;
UInt8 * FrameFromCam;
GraphicBuffer ScreenBuffer ;
DIRECTION PrevDir;

Form ListOfForm[MAX_OF_FORM];
UInt8 LCD_Buffer [LCD_WIDTH*LCD_HEIGHT*BITS] ;

void DetectImageInit();
DIRECTION SignRecognition();

#endif