Created by Bang-Li Weng

I. Functions for device manipulation in Functions.c:

   1. Function for manipulating robot: 
      void MoveBot(DIRECTION *cmd)
      Parameters:
      cmd: command of robot manipulation

   2. void MoveCamera(McDir *cm_ps, int *times)
      Parameters:
      a.) cm_ps : command for adjusting camera
      b.) times : factor for moving camera to desired position
               e.g. distance of 25 from default level, Horizon = 25
		    25 = CM_INCREMENT, 5 * times, 5

II. Error handling features:

   1. When assigned position of camera exceeds maximum or minimum,
      the warning information will display on LCD and position of 
      camera will maintain at position of maximum or minimum.

   2. When command is unrecognized, program will not be compiled. 
      Error message will display on IDE dialog.


III. LCD information features:

   1. Status of process in robot manipulation and camera adjustment will shows on LCD.
      The following information will show on LCD.
      a.) Process name
      b.) Status - start or complet

   2. Warning message will display on LCD while anticipated error occurs.

IV. Head files:

   1. actions.h
   2. directions.h
   3. bitmap_m.h

/////////////////////////////////////////////////////////////////////
//For Functional testing
////////////////////////////////////////////////////////////////////

V. Folder path for compiling and testing
   
   ~\POB-Technology\POBTOOLS4\sdk\pobeye2\Movement_P3

VI. POB-porject file:

   Movement.pobtools

VII. Sample test file:
   
   Movement.c