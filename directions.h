//
// Author: Guy Maor in emum movdir of options for manipulating robot corresponding to signs
// Author: Bang-Li Weng in emum movdir of "options for checking surrounding" and enum MoveCamera
//
#ifndef _MOVE_DIR_
#define MOVE_DIR
#define NUM_DIR 7
typedef enum movdir
{
	// options for manipulating robot corresponding to signs
	//VERY IMPORTANT: Forward is the first direction listed.
	FORWARD,
	TURNBACK,
	LEFT,
	RIGHT,
	STOP,
	// options for checking surrounding 
	Isp_Left,
	Isp_Right	
} DIRECTION;

typedef enum MoveCamera
{
	// options for moving camera
	Horizon,
	Up,
	Down
} McDir;

#endif