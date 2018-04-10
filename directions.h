//
// Author: Guy Maor in emum movdir of options for manipulating robot corresponding to signs
// Author: Bang-Li Weng in emum movdir of "options for checking surrounding" and enum MoveCamera
//
#ifndef DIRECTIONS_H
#define DIRECTIONS_H

#define NUM_DIR 8
typedef enum movdir
{
	// options for manipulating robot corresponding to signs
	//VERY IMPORTANT: Forward is the first direction listed.
	FORWARD=0,
	TURNBACK=1,
	LEFT=2,
	RIGHT=3,
	STOP=4,
	// options for checking surrounding 
	Isp_Left=5,
	Isp_Right=6,
	DEFAULT_DIR=7	
} DIRECTION;

#define NUM_MCDIR_ENUM 2
#define UP_ANGLE 9
typedef enum MoveCamera
{
	// options for moving camera
	Horizon,
	Up,
	Down	
} McDir;

#endif