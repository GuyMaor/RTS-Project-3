/**
 * Author: Joshua Newhouse
 */
 
#include <pob-eye.h>
#include <timer.h>
#include <stdlib.h>
#include "scheduler.h"

#define FRAME_SIZE 15
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
	PrintTextOnPobTerminal("sporadic job 1\n");
}

static void sporadicjob2(void* args) {
	PrintTextOnPobTerminal("sporadic job 1\n");
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
		}
}

/******************** PERIODIC ********************/

static void periodicjob1(void) {
	PrintTextOnPobTerminal("periodic job 1\n");
}

static void periodicjob2(void) {
	PrintTextOnPobTerminal("periodic job 2\n");
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
	{periodicTaskSet1, 5},
	{periodicTaskSet2, 10}
};

/******************** SCHEDULER ********************/

UInt32 nextFrameTime;
static UInt8 currentFrame;

static void Scheduler(void) {
	periodicTaskSet[currentFrame].tasks();
	
	currentFrame = (currentFrame + 1) % NUM_FRAMES;
	nextFrameTime += FRAME_SIZE;
	InitTimer0(nextFrameTime - GetClock() - periodicTaskSet[currentFrame].executionTime);
}

void SchedulerInit(void) {
	Timer0AttachFunction(Scheduler);
	InitTimer0(FRAME_SIZE - periodicTaskSet[0].executionTime);
	nextFrameTime = GetClock() + FRAME_SIZE;
}