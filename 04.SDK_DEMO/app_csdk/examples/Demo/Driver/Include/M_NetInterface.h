/**********************************************************************************************************
 * Copyright(C)2011, 
 * File Name     : M_NetInterface.h
 *  Version      :
 *  Author       :
 * Description   : Network related parameter definitions
 **********************************************************************************************************/

#ifndef __M_NET_INTERFACE__
#define __M_NET_INTERFACE__

//-----------------------------------------------------------------------------
// macro definition
//-----------------------------------------------------------------------------

//Signal minimum value, corresponding to the signal value when not connected to AP
#define WIFI_RSSI_MIN_VAL   (-65536)





// Network module task execution status status
typedef enum
{
    // Execution succeeded
    NET_EXEC_OK = 0,
    // Execution failed
    NET_EXEC_FAIL,
    // Execution timeout
    NET_EXEC_TIMEOUT,
    // Waiting for the completion of the previous task execution
    NET_EXEC_WAIT,
    // Waiting for the execution result of the current task
    NET_EXEC_PENDING,
} NET_EXEC_STATE;

#endif

