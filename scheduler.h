/**
 * Author: Joshua Newhouse
 */
 
#ifndef SCHEDULER_H
#define SCHEDULER_H

extern UInt32 nextFrameTime;

void SchedulerInit(void);
int SporadicJobReady(void);
void SporadicJobExecute(void);

#endif