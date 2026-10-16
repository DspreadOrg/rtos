/**********************************************************************************************************
 * Copyright(C)2021,
 * File Name     : net_if.c
 *  Version      :
 *  Author       :
 * Description   : Encapsulating GPRS and WIFi network communication APIs
 **********************************************************************************************************/

#include "net_if.h"
#include "Language.h"
#include "Common.h"
#include "Communication.h"
#include "LibApi.h"
#include "Input.h"
#include "MessageBox.h"
#include "Key.h"
#include "M_NetInterface.h"
#include "LibGprs.h"
#include "api_lvgl.h"

extern void *hDialog;


// CA Certificate
#define CERT_TYPE_CA            0
// Client certificate and public key
#define CERT_TYPE_CLIENT        1
// Global variables, 0: using TCP; 1: Connect using SSL.This variable is only applicable to old network APIs
static unsigned char s_aucNetSocketMode = NET_MODE_TCP;

// 0 - Use demo code to implement, 1 - Using the API;
#define USE_NET_LIBAPI      1

#define IS_IP(ip) ((ip >= 0) && (ip <= 255) ? 1 : 0)

void Net_Init(void)
{
    unsigned char ucParam;

    Lib_WlsSetAirplaneMode(1);
    Lib_WifiNetworkDown();

    ucParam = GetNetWorkMode();

    if ((ucParam == NET_MODE_WIFI) || (Lib_GetDevState(ID_DEV_IS_ENTER_APP) == FALSE))
    {
        // WiFi module initialization
        Lib_WifiNetworkUp();
        Lib_WifiSetAutoConnectFlag(TRUE);
    }

    // The GPRS module initialization
    Lib_WlsSetAirplaneMode(0);
    Lib_WlsInit();

    // 4G APN adaptive registration network
    // 0: Non mandatory automatic adaptation
    Lib_WlsSetAdaptiveAPN(0);
}

static int Net_GetWifi2GprsFlag(void)
{
    if ((GetNetWorkMode() == NET_MODE_WIFI_PRIOR) && (Lib_WlsGetInitStatus() == RET_OK))
    {
        return RET_OK;
    }

    return RET_ERR;
}

short Net_SetSocketMode(unsigned char mode)
{
    if (mode > NET_MODE_SSL)
    {
        return RET_ERR;
    }
    else
    {
        s_aucNetSocketMode = mode;
    }
    return RET_OK;
}

short Net_GetSocketMode(void)
{
    return s_aucNetSocketMode;
}

//-----------------------------------------------------------------------------
// Name    : Net_OpenCb
// Func    : net open callback
// Paras   : timeout   : Data receiving timeout
//           ct        :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
int Net_OpenCb(int timeout, int ct)
{
    int ret;
    LVGL_RESULT result;

    result.img = FAILURE_IMG;
    result.title = TIP_FAILURE;
    result.fontColor = LV_COLOR_RED;

    if (GetNetWorkMode() == NET_MODE_GPRS)
    {
        // Query SIM card status
        ret = Lib_WlsCheckSim();
        if (ret == WLS_NOSIM)
        {
            TRACE_LOG("no sim card, ret = %d", ret);
            LvUiCloseDialog ( hDialog );
            hDialog = NULL;

            result.msg = TIP_NO_SIM_CARD;
            result.timeout = 10000;
            LvUiShowResult(&result);
            LvUilCls();
            return WLS_NOSIM;
        }
    }

    if (ct - timeout > 0)
    {
        // WiFi priority mode, does not prompt timeout
        if (Net_GetWifi2GprsFlag() == RET_OK)
        {
            LvUiCloseDialog ( hDialog );
            hDialog = NULL;

            result.msg = TIP_NET_WIFI_ABNORMAL;
            result.timeout = 10000;
            LvUiShowResult(&result);
            LvUilCls();
            return UI_SWITCH_GPRS;
        }
        else
        {
            LvUiCloseDialog(hDialog);
            hDialog = NULL;

            result.msg = TIP_NET_INIT_TIMEOUT;
            result.timeout = 10000;
            LvUiShowResult(&result);
            LvUilCls();
            return UI_INPUT_TIMEOUT;
        }
    }
    if (LvUiWaitKey(40) == KB_CANCEL)
    {
        return UI_INPUT_CANCEL;
    }

    return RET_ERR;
}

//-----------------------------------------------------------------------------
// Name    : Net_ConnectCb
// Func    : Network connection callback function
// Paras   : timeout   : Data receiving timeout
//           ct        :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
int Net_ConnectCb(int timeout, int ct)
{
    LVGL_RESULT result;

    result.img = FAILURE_IMG;
    result.title = TIP_FAILURE;
    result.fontColor = LV_COLOR_RED;

    if (ct - timeout > 0)
    {
        // WiFi priority mode, does not prompt timeout
        if (Net_GetWifi2GprsFlag() == RET_OK)
        {
            LvUiCloseDialog(hDialog);
            hDialog = NULL;

            result.msg = TIP_NET_WIFI_ABNORMAL;
            result.timeout = 10000;
            LvUiShowResult(&result);
            LvUilCls();
            return UI_SWITCH_GPRS;
        }
        else
        {
            LvUiCloseDialog(hDialog);
            hDialog = NULL;

            result.msg = TIP_NET_CONNECT_TIMEOUT;
            result.timeout = 10000;
            LvUiShowResult(&result);
            LvUilCls();
            return UI_INPUT_TIMEOUT;
        }
    }
    if (LvUiWaitKey(40) == KB_CANCEL)
    {
        return UI_INPUT_CANCEL;
    }

    return RET_ERR;
}

//-----------------------------------------------------------------------------
// Name    : Net_ReadCb
// Func    : Default callback function for network data receiving interface
// Paras   : timeout   : Data receiving timeout
//           ct        :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
int Net_ReadCb(int timeout, int ct)
{
    LVGL_RESULT result;

    result.img = FAILURE_IMG;
    result.title = TIP_FAILURE;
    result.fontColor = LV_COLOR_RED;

    if (ct - timeout > 0)
    {
        result.msg = TIP_NET_READ_TIMEOUT;
        result.timeout = 10000;
        LvUiShowResult(&result);
        return UI_INPUT_TIMEOUT;
    }
    if (LvUiWaitKey(40) == KB_CANCEL)
    {
        return UI_INPUT_CANCEL;
    }

    return RET_ERR;
}

int Net_WriteCb(void)
{
	return 0;
}

//brief: net reset
int Net_Reset(int timeout)
{
    return Lib_NetResetEx(timeout);
}


//===================================================================================================================================
// The following are the APIS of netif for multi-channel communication functions
// It is recommended to use this new APIs, and the old Apis is not recommended to continue to be used.
// Update date: April 29, 2024
//===================================================================================================================================


//-----------------------------------------------------------------------------
// Name    : Net_OpenEx
// Func    : Create a socket number
// Paras   : handle    : Used to record the socket number of new connections
//           protocol  : The network protocol type of the new link
//           timeout   : Timeout value,in milliseconds
//           Call      : a callback function for Creating socket connections
// Ret     : RET_OK : Created successfully, Other, Created failed
// Desc    :
//-----------------------------------------------------------------------------
int Net_OpenEx(int* handle, NETWORK_TYPE protocol, int timeout, int (*Call)(int timeout, int ct))
{
    return Lib_NetOpenEx(handle, protocol, timeout, Call);
}


//-----------------------------------------------------------------------------
// Name    : Net_ConnectEx
// Func    : Establish a network connection with specified socket number
// Paras   : handle    : Network socket handle,
//           server    : server address
//           port      : server port
//           timeout   : network connection timeout
//           Call      : Callback function for network connection
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
int Net_ConnectEx(int handle, char *server, uint16_t port, int timeout, int (*Call)(int timeout, int ct))
{
    int ip_1, ip_2, ip_3, ip_4;
    int ret = -1;
    unsigned char ucIsIp = 0;

    // Check if it is an IP or a domain name
    ret = sscanf(server, "%d.%d.%d.%d", &ip_1, &ip_2, &ip_3, &ip_4);
    if (ret == 4)
    {
        if (IS_IP(ip_1) && IS_IP(ip_2) && IS_IP(ip_3) && IS_IP(ip_4))
        {
            ucIsIp = 1;
        }
    }
    // If it is IP, no SNI setting is required
    if (ucIsIp == 0)
    {
        // Set SNI, for multiple domain name server configuration
        ret = Lib_NetSetCfg(handle, TLSSOCK_CFG_TYPE_SNI, (uint32_t)server);
        if (ret != RET_OK)
        {
            FUN_IN_ERR;
            return RET_ERR;
        }
    }

    return Lib_NetConnectEx(handle, server, port, timeout, Call);
}


//-----------------------------------------------------------------------------
// Name    : Net_Status
// Func    : Query socket status
// Paras   : fd        : Network socket handle
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
int Net_StatusEx(int handle)
{
    return Lib_NetStatusEx(handle);
}


//-----------------------------------------------------------------------------
// Name    : Net_ReadEx
// Func    : Network receive data with specified socket number
// Paras   : handle    : Network socket handle,
//           buff      : Data receiving cache buffer
//           buff_size : Data receiving cache buffer size
//           timeout   : Data receiving timeout
//           Call      : Callback function for network receiving data
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
int Net_ReadEx(int handle, uint8_t *buff, int buff_size, int timeout, int (*Call)(int timeout, int ct))
{
    return Lib_NetReadEx(handle, buff, buff_size, timeout, Call);
}

//-----------------------------------------------------------------------------
// Name    : Net_WriteEx
// Func    : Network send data with specified socket number
// Paras   : handle    : Network socket handle,
//           buff      : sending data cache buffer
//           len       : sending data length
//           Call      : Callback function for network receiving data
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
int Net_WriteEx(int handle, uint8_t *buff, int len, int (*Call)(void))
{
    return Lib_NetWriteEx(handle, buff, len, Call);
}


//-----------------------------------------------------------------------------
// Name    : Net_CloseEx
// Func    : Close the specified socket connection
// Paras   : handle    : Network socket handle,
// Ret     :
// Desc    : fter calling this function, fd needs to be explicitly configured as invalid.
//-----------------------------------------------------------------------------
int Net_CloseEx(int handle)
{
    return Lib_NetCloseEx(handle);
}

//===================================================================================================================================
// Example of Multi Task Communication Thread
// Implement using UI layer network APIs
// This example code is used to demonstrate new network interfaces calls and multitasking task creation
//===================================================================================================================================
#define TEST_THREAD_RECV_SIZE      (3*1024)

char recv[TEST_THREAD_RECV_SIZE];

void Net_StartChannelThread(void *param)
{
    int handle;
    int ret = RET_ERR;
    unsigned long recv_size = TEST_THREAD_RECV_SIZE;

    const char data[182] = {
       0x00, 0x37,
        0x60, 0x00, 0x06, 0x00, 0x00, 0x60, 0x31, 0x00, 0x31, 0x01,
        0x00, 0x08, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC4, 0x00,
        0x10, 0x33, 0x30, 0x30, 0x31, 0x32, 0x33, 0x31, 0x30, 0x31,
        0x31, 0x37, 0x35, 0x33, 0x39, 0x39, 0x31, 0x36, 0x39, 0x35,
        0x33, 0x30, 0x31, 0x30, 0x00, 0x01, 0x00, 0x00, 0x11, 0x00,
        0x00, 0x00, 0x36, 0x30, 0x10,
    };

    TRACE_LOG("NetworkTaskThread start");

    for (;;)
    {
        // 10min
        Lib_DelayMs(60 * 1000 * 10);

        TRACE_LOG("NetworkTaskThread Start connect.\n");

        // passin the server serial number
        ret = UIWireNetTcpLinkExt(&handle, NETWORK_TCP, NET_MAX_TCPLINK_TIME, SERVER_OPT_POSP_MAIN, NULL);
        if (ret != RET_OK)
        {
            TRACE_LOG("NetworkTaskThread Connect failed.\n");
            goto RECYCLING;
        }

        TRACE_LOG("NetworkTaskThread Connect OK.\n");

        ret = UIWireNetTcpTxExt(handle, (unsigned char *)data, sizeof(data), NET_MAX_TX_TIME, NULL);
        if (ret < 0)
        {
            TRACE_LOG("NetworkTaskThread Send data failed.\n");
            goto RECYCLING;
        }

        TRACE_LOG("NetworkTaskThread data Send OK.\n");

        memset(recv, 0x00, sizeof(recv));
        recv_size = sizeof(recv);

        ret = UIWireNetTcpRxExt(handle, (unsigned char *)recv, &recv_size, NET_MAX_RX_TIME, PACKET_TYPE_RAW_DATA, NULL);
        if (ret != RET_OK)
        {
            TRACE_LOG("NetworkTaskThread Recv failed.\n");
            TRACEBUF((unsigned char *)recv, ret);
            goto RECYCLING;
        }

        TRACE_LOG("NetworkTaskThread Recv OK.\n");

RECYCLING:
        ret = UIWireNetCloseExt(&handle);
        if (ret == RET_OK)
        {
            TRACE_LOG("NetworkTaskThread Close OK.");
        }

        handle = -1;
    }
}


