//-----------------------------------------------------------------------------
// @file    : LibPinPad.h
// @brief   : Multi-Task-related functions
// @details : 
// @author  : 
// @version :      
// @date    :      
// @history :                  
//-----------------------------------------------------------------------------
#ifndef __LIB_OSI_H__
#define __LIB_OSI_H__
#include "LibTypedef.h"

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

typedef enum osiThreadPriority
{
    OSI_PRIORITY_IDLE = 1, // reserved
    OSI_PRIORITY_LOW = 8,
    OSI_PRIORITY_BELOW_NORMAL = 16,
    OSI_PRIORITY_NORMAL = 24,
    OSI_PRIORITY_ABOVE_NORMAL = 32,
    OSI_PRIORITY_HIGH = 40,
    OSI_PRIORITY_REALTIME = 48,
    OSI_PRIORITY_HISR = 56, // reserved
} osiThreadPriority_t;

/**
 * function type of thread entry
 */
typedef void (*osiThreadEntry_t)(void *argument);

/**
 * function type of callback
 */
typedef void (*osiCallback_t)(void *ctx);

// Multi-task API interfaces
//-----------------------------------------------------------------------------
// Name    : Lib_osiThreadCreate
// Func    : Create thread
// Paras   : name[in],         input thread name
//           entry[in],        input osiThreadEntry_t
//           argument[in],     input parameters
//           priority[in],     input thread priority
//           stack_size[in],   input stack size
//           event_count[in],  input event count
// Ret     : 0 - success, others - fail
// Desc    : 
//-----------------------------------------------------------------------------
void *Lib_osiThreadCreate(const char * name, osiThreadEntry_t entry, void * argument, unsigned long priority, 
                            unsigned long stack_size, unsigned long event_count);

//-----------------------------------------------------------------------------
// Name    : Lib_osiThreadCurrent
// Func    : Get current thread
// Paras   : 
// Ret     : return the pointer of current thread
// Desc    : 
//-----------------------------------------------------------------------------
void *Lib_osiThreadCurrent(void);

//-----------------------------------------------------------------------------
// Name    : Lib_osiThreadDelete
// Func    : Delete thread
// Paras   : handle[in],  input thread handle
// Ret     : 0 - success, others - fail
// Desc    : 
//-----------------------------------------------------------------------------
void Lib_osiThreadDelete(void *handle);

//-----------------------------------------------------------------------------
// Name    : Lib_osiMutexCreate
// Func    : Create Mutex
// Paras   : 
// Ret     : return the pointer of Mutex
// Desc    : 
//-----------------------------------------------------------------------------
void *Lib_osiMutexCreate(void);

//-----------------------------------------------------------------------------
// Name    : Lib_osiMutexDelete
// Func    : Delete Mutex
// Paras   : mutex[in],  input pointer of mutex
// Ret     : 
// Desc    : 
//-----------------------------------------------------------------------------
void Lib_osiMutexDelete(void *mutex);

//-----------------------------------------------------------------------------
// Name    : Lib_osiMutexLock
// Func    : Lock Mutex
// Paras   : mutex[in],  input pointer of mutex
// Ret     : 
// Desc    : 
//-----------------------------------------------------------------------------
void Lib_osiMutexLock(void *mutex);

//-----------------------------------------------------------------------------
// Name    : Lib_osiMutexTryLock
// Func    : Try to lock Mutex
// Paras   : mutex[in],    input pointer of mutex
//           timeout[in],  input Timeout
// Ret     : 1: success,0: fail
// Desc    : 
//-----------------------------------------------------------------------------
bool Lib_osiMutexTryLock(void *mutex, unsigned long timeout);

//-----------------------------------------------------------------------------
// Name    : Lib_osiMutexUnlock
// Func    : Unlock Mutex
// Paras   : mutex[in],  input pointer of mutex
// Ret     : 
// Desc    : 
//-----------------------------------------------------------------------------
void Lib_osiMutexUnlock(void *mutex);

//-----------------------------------------------------------------------------
// Name    : Lib_osiSemaphoreCreate
// Func    : Create Semaphore
// Paras   : max_count[in],  input maximum count
//           timeout[in],    input initial count
// Ret     : return the pointer of Semaphore
// Desc    : 
//-----------------------------------------------------------------------------
void *Lib_osiSemaphoreCreate(unsigned long max_count, unsigned long init_count);

//-----------------------------------------------------------------------------
// Name    : Lib_osiSemaphoreDelete
// Func    : Delete Semaphore
// Paras   : sem[in],  input Semaphore
// Ret     : 
// Desc    : 
//-----------------------------------------------------------------------------
void Lib_osiSemaphoreDelete(void *sem);

//-----------------------------------------------------------------------------
// Name    : Lib_osiSemaphoreDelete
// Func    : Release Semaphore
// Paras   : sem[in],  input Semaphore
// Ret     : 
// Desc    : 
//-----------------------------------------------------------------------------
void Lib_osiSemaphoreRelease(void *sem);

//-----------------------------------------------------------------------------
// Name    : Lib_osiSemaphoreAcquire
// Func    : Acquire Semaphore
// Paras   : semaphore[in],  input Semaphore
// Ret     : 1: success,0: fail
// Desc    : 
//-----------------------------------------------------------------------------
bool Lib_osiSemaphoreAcquire(void *semaphore);

//-----------------------------------------------------------------------------
// Name    : Lib_osiSemaphoreTryAcquire
// Func    : Try to acquire Semaphore
// Paras   : semaphore[in],  input Semaphore
//           timeout[in],  input Timeout
// Ret     : 1: success,0: fail
// Desc    : 
//-----------------------------------------------------------------------------
bool Lib_osiSemaphoreTryAcquire(void *semaphore, unsigned long timeout);

//-----------------------------------------------------------------------------
// Name    : Lib_osiCreatQueue
// Func    : Create Queue
// Paras   : count[in],     input counter
//           timeout[in],  input size
// Ret     : return queue pointer
// Desc    : 
//-----------------------------------------------------------------------------
void *Lib_osiCreatQueue(unsigned long count, unsigned long size);

//-----------------------------------------------------------------------------
// Name    : Lib_osiQueueSend
// Func    : Send queue data
// Paras   : handle[in],        input queue handle
//           item[in],          input item
//           tick_to_wait[in],  input waiting time
// Ret     : 0 - success, others - fail
// Desc    : 
//-----------------------------------------------------------------------------
int Lib_osiQueueSend(void *handle, void *item, unsigned long tick_to_wait);

//-----------------------------------------------------------------------------
// Name    : Lib_osiQueueReceive
// Func    : Receive queue data
// Paras   : handle[in],        input queue handle
//           item[in],          input item
//           tick_to_wait[in],  input waiting time
// Ret     : 0 - success, others - fail
// Desc    : 
//-----------------------------------------------------------------------------
int Lib_osiQueueReceive(void *handle, void *item, unsigned long tick_to_wait);

//-----------------------------------------------------------------------------
// Name    : Lib_osiTimerCreate
// Func    : Create Timer
// Paras   : ms[in],        input time,unit is millisecond
//           periodic[in],  input period
//           cb[in],        input osiCallback_t
//           ctx[in],       input pointer
// Ret     : return timer pointer
// Desc    : 
//-----------------------------------------------------------------------------
void *Lib_osiTimerCreate(unsigned long ms, bool periodic, osiCallback_t cb, void *ctx);

//-----------------------------------------------------------------------------
// Name    : Lib_osiTimerDelete
// Func    : Delete Timer
// Paras   : timer[in],  input timer
// Ret     : 0 - success, others - fail
// Desc    : 
//-----------------------------------------------------------------------------
int Lib_osiTimerDelete(void *timer);

//-----------------------------------------------------------------------------
// Name    : Lib_osiTimerStart
// Func    : Start Timer
// Paras   : timer[in],  input timer
// Ret     : 0 - success, others - fail
// Desc    : 
//-----------------------------------------------------------------------------
int Lib_osiTimerStart(void *timer);

//-----------------------------------------------------------------------------
// Name    : Lib_osiTimerStop
// Func    : Stop Timer
// Paras   : timer[in],  input timer
// Ret     : 0 - success, others - fail
// Desc    : 
//-----------------------------------------------------------------------------
int Lib_osiTimerStop(void *timer);

#endif