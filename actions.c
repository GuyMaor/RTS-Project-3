//
// Author: Bang-Li Weng
//
#include <pob-eye.h>
#include <stdlib.h>
#include <stdio.h>
#include "math.h"
#include "bitmap_m.h"
#include "actions.h"

// external functions declarations (for PrintTextOnPobLCD)
extern void InitAsciiBuffer();
extern void PrintTextOnPobLCD(int row, int col, char *string, UInt8 *Screen_Buffer);
extern void HLightText(int row, int col, int size, UInt8 *Screen_Buffer);


// Brief This initialize the PobProto
void InitPobProto (void)
{
	PobProto	Proto; // struct to set the pob-proto
	
	//Proto.porta=ALL_PORTA_AS_ANA;	//to get the position of the analogic joystick, you have to set the PORTA as analogic input
	Proto.portc=RC7_AS_DO| RC6_AS_DO |RC3_AS_DO |RC2_AS_SERVO|RC1_AS_SERVO |RC0_AS_SERVO;	//all pin of PORTC are configured to manage servomotors
	Proto.portd=RD7_AS_DI	| RD6_AS_DI	|RD5_AS_DI |RD4_AS_DI|RD3_AS_DO	|RD2_AS_DO	|RD1_AS_DO	|RD0_AS_DO;		//RD0 RD1 RD2 RD3 are configured as digitals 	    output to gear DC motor, RD4 RD5 RD6 RD7 are configured as digitals input
	SetPobProto(&Proto);	//set the pob proto
}

//array for display the graphic interface
static GraphicBuffer LCD_Buffer_Video;
static UInt8         LCD_Buffer[LCD_WIDTH*LCD_HEIGHT*BITS];
static UInt8		 ASCII_Buffer[LCD_WIDTH*LCD_HEIGHT*BITS]; // Buffer to stock the ascii table in bitmap format

// Set the bitmap  picture of the first 127 ASCII Char in ASCII_GRAPHIC_BUFFER
// ASCII_GRAPHIC_BUFFER will be used to get the pictures of characters to display on the LCD

void InitAsciiBuffer() 
{
	GraphicBuffer ASCII_GRAPHIC_BUFFER;
	
	InitGraphicBuffer(&ASCII_GRAPHIC_BUFFER,LCD_WIDTH,LCD_HEIGHT,ONE_BIT,ASCII_Buffer); 
	ClearGraphicBuffer(&ASCII_GRAPHIC_BUFFER);
	DrawBitmap(0,0, IDB_ASCII,bitmap,&ASCII_GRAPHIC_BUFFER); //bitmap is an array built by pob-tools
}

/****************************************************************************************************************************
* PrintTextOnPobLCD : displays a string in the screen buffer
*
* row : LCD display Row of the first char
* col :  LCD display column of the first char
* string : string to display, Use 'sprintf' function to build a string including floats and intergers
* Screen_Bufffer : Screen buffer to display
*
* ASCII_Buffer is the buffer oh the fisrt 127 chars of the ASCII table. Each char is 8x8 pixels, it is possible to display 16 char per row and 8 columns
*****************************************************************************************************************************/

void PrintTextOnPobLCD(int row, int col, char *string, UInt8 *Screen_Buffer)
{
	int i,j,k=0; // i = ascii char starting buffer; j = 0 to 7 (8 times 8 points)

while(*string) 
	{
		// starting point in the ASCII_Buffer of the char to display,128 (16charx8lines of pixels) is a complete row of text
		i = (string[0]/16)*128 + (string[0]%16); 	
	// display char on Screen_Buffer, 8 bytes
	for (j=0; j<8; j++) 
		{		
		// 8 intergers to define a bitmap of a char
		Screen_Buffer[col+row*128+k*16]=ASCII_Buffer[i+j*16];
		k++;
		}
	string++; 	
	k=0;
	// 1 byte right
	col+=1; 					
	if (col%16==0) 
		{ 			
		// Manages end of line and go to the next line on the left
		row++; 
		col=0;
			// Go to the top of the screen if the bottom is reached
			if (row%8 == 0) 	row=0; 	
		}
	}
}
//
//Initialize ports of robot
//
void Initialization(void)
{
	//Initialize POB-Eye 2
	InitPobeye2();
	InitPobProto();
	InitLCD();
	SwitchOnAllServo();

	//This buffers will stock the pixels to display, 
	GraphicBuffer		LCD_Buffer_Video;
	UInt8				LCD_Buffer[LCD_WIDTH*LCD_HEIGHT*BITS];
	UInt8				ASCII_Buffer[LCD_WIDTH*LCD_HEIGHT*BITS];  // Buffer to stock the ascii table in bitmap format	
}
//
//Function for adjusting camera
//
//Parameters:
//cm_ps : command for adjusting camera
//times : factor for moving camera to desired position
//        e.g. distance of 25 from default level, Horizon = 25
//		       25 = CM_INCREMENT, 5 * times, 5
void MoveCamera(McDir *cm_ps, int *times)
{
	int position = H_Position;
	int flag = -1;
	char print_ps[5]="";

	int step = 0;

	Initialization();
	
		
	flag = 8;
	PrintToABuffer(print_ps, "%d", position);
	PrintStatus(&flag, print_ps);
	
	while (step < 100)
	{
		SetServoMotor(0, position);
		step++;
	}
	if (*cm_ps == Horizon)
	{
		
		flag = 9;
		PrintToABuffer(print_ps, "%d", position);
		PrintStatus(&flag, print_ps);
	}
	else if (*cm_ps == Up)
	{		
		position = position + CM_INCREMENT*(*times);
		
		if (position < Cm_Max_Ps)
		{			
			SetServoMotor(0, position);
			flag = 10;
			PrintToABuffer(print_ps, "%d", position);
			PrintStatus(&flag, print_ps);
		}
		
		else if (position == Cm_Max_Ps)
		{
			SetServoMotor(0, position);
			flag = 11;
			PrintToABuffer(print_ps, "%d", position);
			PrintStatus(&flag, print_ps);
		}

		else
		{
			// Error handling for camera adjustment 
			position = Cm_Max_Ps;
			SetServoMotor(0, position);
			flag = -3;
			PrintToABuffer(print_ps, "%d", position);
			PrintStatus(&flag, print_ps);
		}

	}
	else if (*cm_ps == Down)
	{
		position = position - CM_INCREMENT*(*times);

		if (position > Cm_min_Ps)
		{			
			SetServoMotor(0, position);
			flag = 10;
			PrintToABuffer(print_ps, "%d", position);
			PrintStatus(&flag, print_ps);
		}
		else if (position == Cm_min_Ps)
		{			
			SetServoMotor(0, position);
			flag = 12;
			PrintToABuffer(print_ps, "%d", position);
			PrintStatus(&flag, print_ps);
		}
		// Error handling for camera adjustment
		else
		{ 
			position = Cm_min_Ps;
			SetServoMotor(0, position);
			flag = -4;
			PrintToABuffer(print_ps, "%d", position);
			PrintStatus(&flag, print_ps);

		}	
	}


}
//
//Function for manipulating robot
//
//Parameter:
//cmd: command of robot manipulation
void MoveBot(DIRECTION *cmd)
{
	int step = 0;
	//int Way = 0;
	int flag = -1;
	char status[15]=""; 

	Initialization();

	is_valid(cmd,&flag,&status);
	
	PrintStatus(&flag,&status);

	// Forward
	
	while( flag == 1)
	{
		if (step >= 0 && step < 100)
		{
			SetPortD(run);
			step++;
		}
		else if (step == 100)
		{
			SetPortD(stop);
			flag = 0; 
		}
		else 
			flag = 1;
	}

	// Turnback
	
	while( flag == 2)
	{
		if (step >= 0 && step < 100)
		{
			SetPortD(back);
			step++;
		}
		else if (step >= 100 && step < 171)
		{
			SetPortD(right);
			step++;
		}
		else if (step >= 171 && step < 242)
		{
			SetPortD(right);
			step++;
		}
		else if (step == 242)
		{
			SetPortD(stop);
			flag = 0; 
		}
		else 
			flag = 2;
	}

	// Left Turn

	while( flag == 3)
	{
		if (step >= 0 && step < 21)
		{
			SetPortD(back);
			step++;
		}
		else if (step >= 21 && step < 92)
		{
			SetPortD(left);
			step++;
		}
		else if (step == 92)
		{
			SetPortD(stop);
			flag = 0; 
		}
		else 
			flag = 3; 
	}

	// Right Turn
		 
	while( flag == 4)
	{
		if (step >= 0 && step < 21)
		{
			SetPortD(back);
			step++;
		}
		else if (step >= 21 && step < 92)
		{
			SetPortD(right);
			step++;
		}
		else if (step == 92)
		{
			SetPortD(stop);
			flag = 0; 
		}
		else 
			flag = 4; 
	}


	// Stop
		 
	while( flag == 5)
	{
		if (step >= 0 && step < 100)
		{
			SetPortD(stop);
			step++;
		}

		else 
			flag = 5; 
	}

	// Inspect Left 
		 
	while( flag == 6)
	{
		if (step >= 0 && step < 51)
		{
			SetPortD(left);
			step++;
		}
		else if (step >= 51 && step < 102)
		{
			SetPortD(stop);
			SetServoMotor(0, Cm_Max_Ps);			
			step++; 
		}
		else if (step >= 102 && step < 153)
		{			
			SetServoMotor(0, Cm_min_Ps);
			step++; 
		}
		else if (step >= 153 && step < 204)
		{
			SetServoMotor(0, H_Position);
			step++; 
		}
		else if (step >= 204 && step < 255 )
		{
			SetPortD(right);
			step++;
		}
		else
		{
			SetPortD(stop);
			flag=0;
		}
	}

	// Inspect Right 
		 
	while( flag == 7)
	{
		if (step >= 0 && step < 51)
		{
			SetPortD(right);
			step++;
		}
		else if (step >= 51 && step < 102)
		{
			SetPortD(stop);
			SetServoMotor(0, Cm_Max_Ps);			
			step++; 
		}
		else if (step >= 102 && step < 153)
		{			
			SetServoMotor(0, Cm_min_Ps);
			step++; 
		}
		else if (step >= 153 && step < 204)
		{
			SetServoMotor(0, H_Position);
			step++; 
		}
		else if (step >= 204 && step < 255 )
		{
			SetPortD(left);
			step++;
		}
		else
		{
			SetPortD(stop);
			flag=0;
		}
	}
			
	PrintStatus(&flag,&status);
	

}
//
//Function for checking command in robot manipulation
//
//Parameters:
//cmd: command of robot manipulation
//f: flag for representing each command
//status: char array for stating each command process 
void is_valid(DIRECTION *cmd, int *f, char *status)
{

	
	if (*cmd == FORWARD)
	{		
		*f = 1;
		status[0] = 'o';
 		status[1] =	'f';
		status[2] = ' ';		
		status[3] = 'F';
		status[4] = 'o';
		status[5] = 'r';
		status[6] = 'w';
		status[7] = 'a';
		status[8] = 'r';
		status[9] = 'd';
		status[10] = '\0';
	}
	else if (*cmd == TURNBACK)
	{		
		*f = 2;
		status[0] = 'o';
 		status[1] =	'f';
		status[2] = ' ';		
		status[3] = 'T';
		status[4] = 'u';
		status[5] = 'r';
		status[6] = 'n';
		status[7] = 'b';
		status[8] = 'a';
		status[9] = 'c';
		status[10] = 'k';		
		status[11] = '\0';
	}
	else if (*cmd == LEFT)
	{		
		*f = 3;
		status[0] = 'o';
 		status[1] =	'f';
		status[2] = ' ';		
		status[3] = 'L';
		status[4] = 'e';
		status[5] = 'f';
		status[6] = 't';
		status[7] = ' ';
		status[8] = 'T';
		status[9] = 'u';
		status[10] = 'r';
		status[11] = 'n';
		status[12] = '\0';			
	}
	else if (*cmd == RIGHT)
	{		
		*f = 4;
		status[0] = 'o';
 		status[1] =	'f';
		status[2] = ' ';		
		status[3] = 'R';
		status[4] = 'i';
		status[5] = 'g';
		status[6] = 'h';
		status[7] = 't';
		status[8] = ' ';
		status[9] = 'T';
		status[10] = 'u';
		status[11] = 'r';
		status[12] = 'n';
		status[13] = '\0';			
	}
	else if (*cmd == STOP)
	{		
		*f = 5;
		status[0] = 'o';
 		status[1] =	'f';
		status[2] = ' ';		
		status[3] = 'S';
		status[4] = 't';
		status[5] = 'o';
		status[6] = 'p';
		status[7] = '\0';			
	}

	else if (*cmd == Isp_Left)
	{		
		*f = 6;
		status[0] = 'o';
 		status[1] =	'f';
		status[2] = ' ';		
		status[3] = 'L';
		status[4] = 'e';
		status[5] = 'f';
		status[6] = 't';
		status[7] = ' ';
		status[8] = 'C';
		status[9] = 'h';
		status[10] = 'e';
		status[11] = 'c';
		status[12] = 'k';
		status[13] = '\0';			
	}	


	else if (*cmd == Isp_Right)
	{		
		*f = 7;
		status[0] = 'o';
 		status[1] =	'f';
		status[2] = ' ';		
		status[3] = 'R';
		status[4] = 'i';
		status[5] = 'g';
		status[6] = 'h';
		status[7] = 't';		
		status[8] = ' ';
		status[9] = 'C';
		status[10] = 'h';
		status[11] = 'e';
		status[12] = 'c';
		status[13] = 'k';		
		status[14] = '\0';						
	}	

	// Error handling for robot manipulation
	else
	{		
		*f = -2;
		status[0] = 'C';
 		status[1] =	'm';
		status[2] = 'd';		
		status[3] = ' ';
		status[4] = 'N';
		status[5] = 'o';
		status[6] = ' ';
		status[7] = 'F';
		status[8] = 'o';
		status[9] = 'u';
		status[10] = 'n';
		status[11] = 'd';
		status[12] = '!';
		status[13] = '\0';			
	}
}
//
//Function for printing out the status of process
//
//Parameters:
//f: flag for representing each command
//status: char array for stating each command process 
void PrintStatus(int *f, char *status)
{
	
	InitGraphicBuffer( &LCD_Buffer_Video, LCD_WIDTH,LCD_HEIGHT,ONE_BIT,LCD_Buffer);
	ClearGraphicBuffer(&LCD_Buffer_Video);
	DrawLCD(&LCD_Buffer_Video);
	InitAsciiBuffer();
	 
	//
	// Tasks start
	//
	if	(*f >0 && *f <= 7)
	{
		PrintTextOnPobLCD(0,0, "Movement Porcess", LCD_Buffer);
		PrintTextOnPobLCD(1,0,status, LCD_Buffer);
		PrintTextOnPobLCD(2,0, "Status:", LCD_Buffer);
		PrintTextOnPobLCD(4,0, "Started!", LCD_Buffer);
	}

 	else if (*f == 8 )
	{
		PrintTextOnPobLCD(0,0, "Camera Movement", LCD_Buffer);
		PrintTextOnPobLCD(1,0, "Process Status:", LCD_Buffer);
		PrintTextOnPobLCD(2,0, "Began", LCD_Buffer);
		PrintTextOnPobLCD(3,0, "Current Position:", LCD_Buffer);
		PrintTextOnPobLCD(4,0, status, LCD_Buffer);
	}

 	else if (*f == 9 )
	{
		PrintTextOnPobLCD(0,0, "Camera Movement", LCD_Buffer);
		PrintTextOnPobLCD(1,0, "Process Status:", LCD_Buffer);
		PrintTextOnPobLCD(2,0, "Completed", LCD_Buffer);
		PrintTextOnPobLCD(3,0, "Horizon Reached", LCD_Buffer);
		PrintTextOnPobLCD(4,0, "Position: ", LCD_Buffer);
		PrintTextOnPobLCD(5,0, status, LCD_Buffer);
	}

 	else if (*f == 10 )
	{
		PrintTextOnPobLCD(0,0, "Camera Movement", LCD_Buffer);
		PrintTextOnPobLCD(1,0, "Process Status:", LCD_Buffer);
		PrintTextOnPobLCD(2,0, "Completed", LCD_Buffer);
		PrintTextOnPobLCD(3,0, "Curret Position:", LCD_Buffer);
		PrintTextOnPobLCD(4,0, status, LCD_Buffer);
	}

 	else if (*f == 11 )
	{
		PrintTextOnPobLCD(0,0, "Camera Movement", LCD_Buffer);
		PrintTextOnPobLCD(1,0, "Process Status:", LCD_Buffer);
		PrintTextOnPobLCD(2,0, "Completed", LCD_Buffer);
		PrintTextOnPobLCD(3,0, "Maximum Reached", LCD_Buffer);
		PrintTextOnPobLCD(4,0, "Position: ", LCD_Buffer);
		PrintTextOnPobLCD(5,0, status, LCD_Buffer);
	}

 	else if (*f == 12 )
	{
		PrintTextOnPobLCD(0,0, "Camera Movement", LCD_Buffer);
		PrintTextOnPobLCD(1,0, "Process Status:", LCD_Buffer);
		PrintTextOnPobLCD(2,0, "Completed", LCD_Buffer);
		PrintTextOnPobLCD(3,0, "Minimum Reached", LCD_Buffer);
		PrintTextOnPobLCD(4,0, "Position: ", LCD_Buffer);
		PrintTextOnPobLCD(5,0, status, LCD_Buffer);
	}
	//
	// Task completion
	//		
	else if (*f == 0)
	{
		PrintTextOnPobLCD(0,0, "Movement Porcess", LCD_Buffer);
		PrintTextOnPobLCD(1,0,status, LCD_Buffer);
		PrintTextOnPobLCD(2,0, "Status:", LCD_Buffer);
		PrintTextOnPobLCD(4,0, "Completed!", LCD_Buffer);
	}
	//
	// Error handlings
	//
 	else if (*f == -2 )
	{
	 	PrintTextOnPobLCD(0,0,status, LCD_Buffer);
		SetPortD(stop);
	}

	
 	else if (*f == -3 )
	{
		PrintTextOnPobLCD(0,0, "Camera Movement", LCD_Buffer);
		PrintTextOnPobLCD(1,0, "Process Status:", LCD_Buffer);
		PrintTextOnPobLCD(2,0, "Terminated", LCD_Buffer);
		PrintTextOnPobLCD(3,0, "Exceed Maximum", LCD_Buffer);
		PrintTextOnPobLCD(4,0, "Current Position: ", LCD_Buffer);
		PrintTextOnPobLCD(5,0, status, LCD_Buffer);
	}

 	else if (*f == -4 )
	{
		PrintTextOnPobLCD(0,0, "Camera Movement", LCD_Buffer);
		PrintTextOnPobLCD(1,0, "Process Status:", LCD_Buffer);
		PrintTextOnPobLCD(2,0, "Terminated", LCD_Buffer);
		PrintTextOnPobLCD(3,0, "Exceed Minimum", LCD_Buffer);
		PrintTextOnPobLCD(4,0, "Current Position: ", LCD_Buffer);
		PrintTextOnPobLCD(5,0, status, LCD_Buffer);
	}
	// Print to LCD
	DrawLCD(&LCD_Buffer_Video);
}

