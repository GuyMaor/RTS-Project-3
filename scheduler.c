/**
 * Author: Joshua Newhouse
 */
 
#include <pob-eye.h>
#include <timer.h>
#include <stdlib.h>
#include "scheduler.h"
#include "actions.h"
#include "directions.h"
#include "detectImage.h"

#define MILS_TO_MICROS(ms) ((ms) * 1000)
#define CLOCK1_TO_CLOCK2_OFFSET 1
#define CLOCK1_CORRECTION(clock1) ((clock1) + CLOCK1_TO_CLOCK2_OFFSET)

#define FRAME_SIZE 1000
#define NUM_FRAMES 2
#define NUM_SPORADIC_TASKS 1

/******************** SPORADIC ********************/

typedef void (*sporadicJob_tfp)(void*);

typedef struct {
	sporadicJob_tfp job;
	void* args;
	UInt8 isReady;
} SporadicTask_ts;

static void sporadicjob1(void* args) {
	PrintToUart0("Move Bot: %d\n", (DIRECTION)args);
	MoveBot((DIRECTION)args);
	SendString("Move complete.\n");
}

static void sporadicjob2(void* args) {
	PrintToUart0("sporadic job 2: %s\n", (char*)args);
}

SporadicTask_ts sporadicTask[NUM_SPORADIC_TASKS] = {
	{sporadicjob1, NULL, 0}
//	{sporadicjob2, NULL, 0}
};

int SporadicJobReady(void) {
	int i;
	for(i = 0; i < NUM_SPORADIC_TASKS; i++)
		if(sporadicTask[i].isReady)
			return 1;
	return 0;
}

void SporadicJobExecute(void) {
	int i;
	for(i = 0; i < NUM_SPORADIC_TASKS; i++)
		if(sporadicTask[i].isReady) {
			sporadicTask[i].isReady = 0;
			sporadicTask[i].job(sporadicTask[i].args);
			sporadicTask[i].args = NULL;
		}
}

/******************** PERIODIC ********************/

static void periodicjob1(void) {
//	SendString("Move camera\n");
//	static McDir dir = Horizon;

//	MoveCamera(dir, UP_ANGLE);
//	dir = (dir + 1) % NUM_MCDIR_ENUM;
}

static void periodicjob2(void) {
	SendString("Sign recognition\n");
	DIRECTION out = SignRecognition();
	PrintToUart0("Recognized: %d\n", out);
	sporadicTask[0].args = (void*)out;
	sporadicTask[0].isReady = 1;
}

#define TASKSET1_EXEC_TIME 50
static void periodicTaskSet1(void) {
	periodicjob1();
	periodicjob2();
	
	if(SporadicJobReady()) {
		SendString("Sporadic job ready\n");
		SporadicJobExecute();
	}
}

#define TASKSET2_EXEC_TIME 50
static void periodicTaskSet2(void) {
	periodicjob1();
	periodicjob2();
	
	if(SporadicJobReady()) {
		SendString("Sporadic job ready\n");
		SporadicJobExecute();
	}
}

typedef void (*periodicTaskSet_tfp)(void);

typedef struct {
	periodicTaskSet_tfp tasks;
	UInt8 executionTime;	
} PeriodicTaskSet_ts;

PeriodicTaskSet_ts periodicTaskSet[NUM_FRAMES] = {
	{periodicTaskSet1, TASKSET1_EXEC_TIME},
	{periodicTaskSet2, TASKSET2_EXEC_TIME}
};

/******************** SCHEDULER ********************/

static UInt8 currentFrame;

static void Scheduler(void) {
	PrintToUart0("Scheduler running: %d\nCurrent Frame: %d\n", CLOCK1_CORRECTION(GetClock()), currentFrame);
	PrintToUart0("", currentFrame);
	periodicTaskSet[currentFrame].tasks();
	
	currentFrame = (currentFrame + 1) % NUM_FRAMES;

	PrintToUart0("Scheduler terminating: %d\n", CLOCK1_CORRECTION(GetClock()));
}

void SchedulerInit(void) {
	SendString("Initializing scheduler\n");
MoveCamera(Up, UP_ANGLE - 1);
	InitTimer0(MILS_TO_MICROS(FRAME_SIZE));
	StartClock();
	Timer0AttachFunction(Scheduler);

	UInt32 currentTime = GetClock();
	PrintToUart0("Init currentTime: %d\n", currentTime);
}