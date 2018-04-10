/**
 * Author: Joshua Newhouse
 */
 
#include <pob-eye.h>
#include <timer.h>
#include <stdlib.h>
#include "scheduler.h"
#include "actions.h"

#define FRAME_SIZE 80
#define NUM_FRAMES 2
#define NUM_SPORADIC_TASKS 2

/******************** SPORADIC ********************/

typedef void (*sporadicJob_tfp)(void*);

typedef struct {
	sporadicJob_tfp job;
	void* args;
	UInt8 isReady;
} SporadicTask_ts;

static void sporadicjob1(void* args) {
	PrintToUart0("sporadic job 1: %s\n", (char*)args);
}

static void sporadicjob2(void* args) {
	PrintToUart0("sporadic job 2: %s\n", (char*)args);
}

SporadicTask_ts sporadicTask[NUM_SPORADIC_TASKS] = {
	{sporadicjob1, NULL, 0},
	{sporadicjob2, NULL, 0}
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
	SendString("periodic job 1\n");

	static UInt32 counter;
	if(counter++ % 4 == 0) {
		sporadicTask[0].args = (void*)"Sporadic job 1.";
		sporadicTask[0].isReady = 1;
	}
}

static void periodicjob2(void) {
	SendString("periodic job 2\n");
	
	static UInt32 counter;
	if(counter++ % 8 == 0) {
		sporadicTask[1].args = (void*)"Sporadic job 2.";
		sporadicTask[1].isReady = 1;
	}
}

static void periodicTaskSet1(void) {
	periodicjob1();
}

static void periodicTaskSet2(void) {
	periodicjob1();
	periodicjob2();
}

typedef void (*periodicTaskSet_tfp)(void);

typedef struct {
	periodicTaskSet_tfp tasks;
	UInt8 executionTime;	
} PeriodicTaskSet_ts;

PeriodicTaskSet_ts periodicTaskSet[NUM_FRAMES] = {
	{periodicTaskSet1, 20},
	{periodicTaskSet2, 40}
};

/******************** SCHEDULER ********************/

UInt32 nextFrameTime;
static UInt8 currentFrame;

static void Scheduler(void) {
	PrintToUart0("Scheduler running: %d\n", GetClock());
	periodicTaskSet[currentFrame].tasks();
	
	currentFrame = (currentFrame + 1) % NUM_FRAMES;
	nextFrameTime += FRAME_SIZE;
	UInt32 timer = (nextFrameTime - GetClock() - (UInt32)periodicTaskSet[currentFrame].executionTime) * 1000;
	PrintToUart0("Timer: %d\n", timer);
	InitTimer0(timer);
	Timer0AttachFunction(Scheduler);
	PrintToUart0("Scheduler terminating: %d\n", GetClock());
}

void SchedulerInit(void) {
	SendString("Initializing scheduler\n");
	
	enableIRQ();

	UInt32 timer = ((UInt32)FRAME_SIZE - (UInt32)periodicTaskSet[0].executionTime) * 1000;
	PrintToUart0("Init timer: %d\n", timer);
	InitTimer0(timer);
	Timer0AttachFunction(Scheduler);
	StartClock();
	UInt32 currentTime = GetClock();
	nextFrameTime = currentTime + FRAME_SIZE;
	PrintToUart0("Init currentTime: %d\nnextFrameTime: %d\n", currentTime, nextFrameTime);
}