#pragma once
#include <Lib/Types.h>
#include <Core/Multitasking/Thread.h>

void TOS_InitScheduler();
void TOS_InitKernelThread();
void TOS_SwitchThread(bool unlock);

void TOS_LoadThread(TOS_Thread* thread);
void TOS_UnloadThread(TOS_Thread* thread);
void TOS_StartThread(TOS_Thread* thread);
void TOS_StopThread(TOS_Thread* thread);
void TOS_TerminateThread(TOS_Thread* thread);
void TOS_LockThread(TOS_Thread* thread);
void TOS_UnlockThread(TOS_Thread* thread);

void     TOS_ToggleScheduler(bool enabled);
uint32_t TOS_GenerateThreadID();
uint32_t TOS_GetThreadCount();