#pragma once
#include <Lib/Types.h>
#include <Lib/PointerList.h>

typedef enum { UNLOCKED, LOCKED } THREAD_LOCK;

typedef enum
{
    THREAD_HALTED,
    THREAD_WAITING,
    THREAD_RUNNING,
    THREAD_TERMINATED,
} THREAD_STATE;

typedef enum
{
    THREAD_PRIORITY_LOW,
    THREAD_PRIORITY_NORMAL,
    THREAD_PRIORITY_HIGH,
} THREAD_PRIORITY;

typedef struct
{
    float    seconds;
    float    start;
    uint64_t ticks;
    uint32_t tps;
    uint8_t  cpu_usage;
} TOS_ThreadTime;

typedef struct
{
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax, eflags;
} TOS_ThreadRegisters;

typedef int (*TOS_ThreadProtocol)(TOS_PtrList* args);

typedef struct
{
    TOS_ThreadRegisters registers;
    TOS_ThreadProtocol  protocol;
    TOS_ThreadTime      time;
    THREAD_STATE        state;
    THREAD_PRIORITY     priority;
    THREAD_LOCK         lock;
    TOS_PtrList         args;
    size_t              stacksz;
    uint32_t*           stack;
    uint32_t            id;
    char*               name;
} TOS_Thread;

TOS_Thread* TOS_NewThread(const char* name, size_t stacksz, TOS_ThreadProtocol protocol, THREAD_PRIORITY priority, TOS_PtrList args);
void TOS_FreeThread(TOS_Thread* thread);

void TOS_UpdateThreadTime(TOS_Thread* thread);

extern TOS_Thread* THIS_THREAD;
extern TOS_Thread* NEXT_THREAD;