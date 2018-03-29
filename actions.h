//
// Author: Bang-Li Weng
//
// This .h file is responsible for moving rob with corresponding sign.
//
//
#ifndef DIRH
#define DIRH
#include "directions.h"
#endif

#ifndef _ACTIONS
#define ACTIONS

#define RIGHT_LEFT_AXE	1		//the right-left axe is on the RA1 analogic input
#define UP_DOWN_AXE	0		//the up-down axe is on the RA0 analogic input
#define BUTTON	4						//the button is on the RA4 analogic input

//Value definitions for commands in robot manipulation 
#define run 0x05
#define stop 0
#define left 0x09
#define right 0x06
#define back 0x0A

//Value definitions for commands in camera adjustment
#define CM_INCREMENT 5
#define H_Position 25
#define Cm_Max_Ps 130
#define Cm_min_Ps 0


//Initialize ports of robot
void InitPobProto();
//Function for adjusting camera
void MoveCamera(McDir *cm_ps, int *times);
//Function for manipulating robot
void MoveBot(DIRECTION *cmd);
//Function for checking command in robot manipulation
void is_valid(DIRECTION *cmd, int *f, char *status);

#endif 