/**********************************************************************************************************
 * Copyright(C)2011,
 * File Name     : Communication.c
 *  Version      :
 *  Author       :
 * Description   : Implement network communication related functions
 **********************************************************************************************************/

#include "Communication.h"
#include "Language.h"
#include "ParamManage.h"
#include "Common.h"
#include "MessageBox.h"
#include "Input.h"
#include "Key.h"
#include "net_if.h"
#include "Language.h"
#include "M_NetInterface.h"
#include "api_lvgl.h"
#include "custom_Lvgl.h"

extern void *hDialog;

// Network mode, default gprs
unsigned char g_ucNetMode = NET_MODE_WIFI;

// g_serverOpt Only single channel communication Apis can be used
// For multi-channel communication scenarios, please use Ext class network interfaces
// Comment By ZhC 2024.09.14
// POSP Primary server Default. Communication function may fail when running multiple tasks.
static EN_SERVER_OPT g_serverOpt = SERVER_OPT_POSP_MAIN;

//-----------------------------------------------------------------------------
// Name    : UIWireNetInit
// Func    : Communication function initialization
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UIWireNetInit(void)
{
    static bool netInit = false;

    if (!netInit)
    {
        netInit = true;
        Net_Init();
    }

    return RET_OK;
}

//-----------------------------------------------------------------------------
// Name    : GetNetWorkMode
// Func    : Query the current network working mode
// Paras   : ucMode  - 4G mode or WiFi mode
// Ret     :
// Desc    : Check the network settings of the applications when startup and load
//           it into the status variable
//-----------------------------------------------------------------------------
unsigned char GetNetWorkMode(void)
{
	return g_ucNetMode;
}


//-----------------------------------------------------------------------------
// Name    : SetNetWorkMode
// Func    : Set network working mode
// Paras   : ucMode  - 4G mode or WiFi mode
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short SetNetWorkMode(unsigned char ucMode)
{
    if (RET_OK == TerminalParamSet(NET_WORK_MODE_BIT, &ucMode, 1))
    {
        FUN_IN;
        g_ucNetMode = ucMode;
        // Synchronize settings to SDK for subsequent network communication management
        Lib_SetInfo(E_NET_MODE, &g_ucNetMode, sizeof(g_ucNetMode));
        return RET_OK;
    }

    return RET_ERR;
}

//-----------------------------------------------------------------------------
// Name  : SetServerOpt
// Func  : Set server options
// Paras : serverOpt : server type
// Ret   :
// Desc  :
//-----------------------------------------------------------------------------
void SetServerOpt(EN_SERVER_OPT serverOpt)
{
    g_serverOpt = serverOpt;
}


//-----------------------------------------------------------------------------
// Name  : GetServerOpt
// Func  : Get current server Opt Idx number
// Paras :
// ret   : Server Opt Idx number
// desc  :
//-----------------------------------------------------------------------------
EN_SERVER_OPT GetServerOpt(void)
{
    return g_serverOpt;
}


#define update_check_offset(off, total, len) \
    {                                        \
        (off) += (len);                      \
        if ((off) >= (total))                \
        {                                    \
            TRACE("buffer overflow!\n");     \
            return RET_ERR;                   \
        }                                    \
    }


//-----------------------------------------------------------------------------
// Name    : UIWireNetDefaultTips
// Func    : get default prompts
// Paras   :
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
char *UIWireNetDefaultTips(void)
{
    EN_SERVER_OPT serverOpt = GetServerOpt();

    if ((serverOpt != SERVER_OPT_TMS_MAIN) && (serverOpt != SERVER_OPT_TMS_BACKUP) && (serverOpt != SERVER_OPT_TMS_MODULE))
    {
        return LOAD_DATA_GIF;
    }
    else
    {
        return NULL;
    }
}


//-----------------------------------------------------------------------------
// Name    : UIWireHttpSend
// Func    : HTTP/https send Data Function
// Paras   : pUrl         - in     - URL address for data to be sent
//           pucBuf       - out    - Cache for storing received data
//           pusLen       - in/out - When input, it is the maximum length of the cache,
//                                   when output, it is the length of the received data
//           pucExtHeader - in       Format header information for HTTP sending data
//           Call         - The callback function for sending data, refer to Net_WriteCb
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UIWireHttpSend(int handle, char *pUrl, unsigned char *pucBuf, unsigned short usLen, char *pucExtHeader, int (*Call)(void))
{
    short sRC;
    unsigned int offset;
    unsigned char *pucPoint, *pucOut;
    unsigned char domainName[128];
    char *pcTmp = NULL, *pcTmp2 = NULL;
    unsigned short usDomainLen = 0;
    // The maximum send data length is 3200 bytes
    unsigned char aucPackBuffer[3200] = {0};
    unsigned short usPackLen;
    const char *pucMethod;

    if (pUrl == NULL)
    {
        return RET_ERR_PARAMETER;
    }

    pcTmp = strstr((char *)pUrl, "http://");
    if (pcTmp)
    {
        if (0 != Net_GetSocketMode())
        {
            return RET_ERR_PARAMETER;
        }

        pcTmp += strlen("http://");
    }
    else if (NULL != (pcTmp = strstr((char *)pUrl, "https://")))
    {
        if (1 != Net_GetSocketMode())
        {
            return RET_ERR_PARAMETER;
        }

        pcTmp += strlen("https://");
    }
    else
    {
        TRACE("no http:// nor https:// found in pUrl=%s\n", pUrl);
        return RET_ERR;
    }
    pcTmp2 = strstr((char *)pcTmp, "/");
    if (pcTmp2)
    {
        usDomainLen = (unsigned short)(pcTmp2 - pcTmp);
    }
    else
    {
        usDomainLen = strlen(pUrl) - (unsigned short)(pcTmp - pUrl);
    }

    if (usDomainLen > sizeof(domainName) - 1)
    {
        return RET_ERR;
    }
    memset(domainName, 0x00, sizeof(domainName));
    memcpy(domainName, pcTmp, usDomainLen);

    if (pucBuf == NULL)
    {
        //GET
        pucMethod = "GET ";
    }
    else
    {
        //POST
        pucMethod = "POST ";
    }
    pucPoint = aucPackBuffer;
    offset = 1; //strncat n plus a null chr

    strcpy((char *)pucPoint, pucMethod);
    offset += strlen(pucMethod);
    if (pcTmp2)
    {
        strncat((char *)pucPoint, (char *)(pcTmp2), sizeof(aucPackBuffer) - offset);
        update_check_offset(offset, sizeof(aucPackBuffer), strlen(pcTmp2));
    }
    else
    {
        strncat((char *)pucPoint, (char *)"/", sizeof(aucPackBuffer) - offset);
        update_check_offset(offset, sizeof(aucPackBuffer), 1);
    }
    strncat((char *)pucPoint, " HTTP/1.1\r\n", sizeof(aucPackBuffer) - offset);
    update_check_offset(offset, sizeof(aucPackBuffer), 10);

    strncat((char *)pucPoint, "Host: ", sizeof(aucPackBuffer) - offset);
    update_check_offset(offset, sizeof(aucPackBuffer), 6);
    strncat((char *)pucPoint, (char *)(domainName), sizeof(aucPackBuffer) - offset);
    update_check_offset(offset, sizeof(aucPackBuffer), strlen((const char *)domainName));
    strncat((char *)pucPoint, "\r\n", sizeof(aucPackBuffer) - offset);
    update_check_offset(offset, sizeof(aucPackBuffer), 2);

    if (pucBuf)
    {
        //POST
        sprintf((char *)domainName, "Content-Length:%d\r\n", usLen);
        strncat((char *)pucPoint, (char *)domainName, sizeof(aucPackBuffer) - offset);
        update_check_offset(offset, sizeof(aucPackBuffer), strlen((const char *)domainName));
        pcTmp = "Connection: Keep-Alive\r\n";
        strncat((char *)pucPoint, pcTmp, sizeof(aucPackBuffer) - offset);
        update_check_offset(offset, sizeof(aucPackBuffer), strlen(pcTmp));
        if (NULL == strstr(pucExtHeader, "User-Agent") && (!pucExtHeader))
        {
            pcTmp = "User-Agent: Esp8266_wroom02\r\n";
            strncat((char *)pucPoint, pcTmp, sizeof(aucPackBuffer) - offset);
            update_check_offset(offset, sizeof(aucPackBuffer), strlen(pcTmp));
        }
        if (!pucExtHeader)
        {
            pcTmp = "Content-Type: application/x-www-form-urlencoded\r\n";
            strncat((char *)pucPoint, pcTmp, sizeof(aucPackBuffer) - offset);
            update_check_offset(offset, sizeof(aucPackBuffer), strlen(pcTmp));
        }
    }

    if (pucExtHeader)
    {
        strncat((char *)pucPoint, (char *)pucExtHeader, sizeof(aucPackBuffer) - offset);
        update_check_offset(offset, sizeof(aucPackBuffer), strlen(pucExtHeader));
    }

    strncat((char *)pucPoint, "\r\n", sizeof(aucPackBuffer) - offset);
    update_check_offset(offset, sizeof(aucPackBuffer), 2);

    offset = strlen((char *)pucPoint);

    if (pucBuf)
    {
        //POST
        if (offset + usLen > sizeof(aucPackBuffer))
        {
            TRACE("short buffer, offset=%d, usLen = %d\n", offset, usLen);
            return RET_ERR;
        }
        memcpy(aucPackBuffer + offset, pucBuf, usLen);
        offset += usLen;
    }

    usPackLen = offset;

    pucOut = aucPackBuffer;

    sRC = UIWireNetTcpTxExt(handle, pucOut, usPackLen, NET_MAX_TX_TIME, Call);

    return sRC;
}

// The maximum received data length is 16KB bytes
unsigned char aucPackBuffer[16*1024] = {0};
//-----------------------------------------------------------------------------
// Name    : UIWireHttpRecv
// Func    : HTTP/https Receive Data Function
// Paras   : pucBuf    - out    - Cache for storing received data
//           pulLen    - in/out - When input, it is the maximum length of the cache,
//                                when output, it is the length of the received data
//           Call      - The callback function for receiving, refer to Net_ReadCb
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UIWireHttpRecv(int handle, unsigned char *pucBuf, unsigned long *pulLen, int (*Call)(int timeout, int ct))
{
    unsigned long ulRecvDataLen;
    char *pcEntityStart, *pcHeaderEnd, *pcPtr, *pcTmp;
    unsigned short usStatus;
    const char *pcTag1 = "Content-Length:";
    const char *pcTag2 = "Transfer-Encoding:";
    // HTTP message entity length
    unsigned int ulEntityLen = 0;
    unsigned char ucChunked = 0;
    // HTTP message header length
    //unsigned short ulHeadLen = 0;
    char ucbuf[32]={0};
    const char *pcTag3 = "\r\n\x30\r\n\r\n"; //match "\r\n0\r\n\r\n"
    unsigned int ulTimeout;
    unsigned short sRC;
    unsigned long ulLen;

    unsigned short aucPackBufferSize = sizeof(aucPackBuffer) - 1;
    memset(aucPackBuffer, 0x00, sizeof(aucPackBuffer));
    if ((NULL == pucBuf) || (0 == *pulLen))
    {
        return RET_ERR_PARAMETER;
    }

    ulRecvDataLen = sizeof(aucPackBuffer) - 1;
    if (0 > UIWireNetTcpRxExt(handle, aucPackBuffer, &ulRecvDataLen, NET_MAX_RX_TIME, PACKET_TYPE_RAW_DATA, Call))
    {
        FUN_IN_ERR;
        return RET_ERR;
    }
    aucPackBufferSize -= ulRecvDataLen;

reparse:
    TRACE_LOG("reparsereparse Start\n");
    // String terminator
    aucPackBuffer[ulRecvDataLen] = 0x00;
    pcPtr = (char *)aucPackBuffer;
    pcHeaderEnd = strstr(pcPtr, "\r\n\r\n");
    if (pcHeaderEnd == NULL)
    {
        TRACE("header end not found!\n");
        // TRACE("pcPtr is:\n\n%s\n\n", pcPtr);
        if (ulRecvDataLen >= sizeof(aucPackBuffer) - 1)
        {
            TRACE("short buffer!\n");
            return RET_ERR;
        }
        //block_recv_left_data_ext();
        {
            TRACE("try to receive left data\n");
            ulTimeout = Lib_SysGetTimer();
            do
            {
                ulLen = sizeof(aucPackBuffer) - ulRecvDataLen - 1;
                sRC = UIWireNetTcpRxExt(handle, aucPackBuffer + ulRecvDataLen, &ulLen, NET_MAX_RX_TIME, PACKET_TYPE_RAW_DATA, Call);
            } while (sRC < RET_OK && !IS_TIME_OUT(ulTimeout, 3000));
            if (sRC != RET_OK)
            {
                TRACE("failed to receive left data\n");
                return RET_ERR;
            }
            ulRecvDataLen += ulLen;
            goto reparse;
        }
    }

    memcpy(pucBuf, pcPtr, pcHeaderEnd + 4 - pcPtr);
    pcEntityStart = pcHeaderEnd + 4;

    TRACE("============== pcEntityStart:\n");
    Trace_LongData(pcEntityStart);

    pcTmp = strstr(pcPtr, " ");
    if (NULL == pcTmp)
    {
        return RET_ERR;
    }
    usStatus = strtol(pcTmp + 1, NULL, 10);
    TRACE("usStatus is %d\n", usStatus);
#if 1 // Comment out this code block during debugging
    if (usStatus < 200 || usStatus >= 300)
    {
        // HTTP response returns 4xx, 5xx represents error, 3xx represents redirection,
        // 1xx represents message, and only 2xx represents success, Only handle 2xx successful response entities
        TRACE("server returns error status:%d\n", usStatus);
        TRACE("pcPtr is:");
        //Trace_LongBuf(pcPtr, ulRecvDataLen);
        sprintf(ucbuf,"HTTP:%d",usStatus);

        MessageBox(ucbuf, MB_CENTER|MB_COLOR_RED, 2000);
        return RET_ERR;
    }
#endif
    //2xx OK
    pcPtr = strstr(pcPtr, "\r\n");
    pcPtr += 2;
    while (pcPtr < pcHeaderEnd)
    {
        pcTmp = pcPtr;
        while (*pcTmp == ' ')
            pcTmp++;
        if (strncasecmp((char *)pcTmp, pcTag1, strlen(pcTag1)) == 0)
        {
            pcTmp += strlen(pcTag1);
            ulEntityLen = strtol(pcTmp, NULL, 0);
        }
        if (strncasecmp((char *)pcTmp, pcTag2, strlen(pcTag2)) == 0)
        {
            pcTmp += strlen(pcTag2);
            while (*pcTmp == ' ')
                pcTmp++;
            if (strncasecmp((char *)pcTmp, "chunked", strlen("chunked")) == 0)
            {
                TRACE("chunked encoding\n");
                ucChunked = 1;
            }
        }
        pcPtr = strstr(pcPtr, "\r\n");
        pcPtr += 2;
    }

    if (ucChunked == 0)
    {
        // No chunked encoding, using content length
        if (ulEntityLen > 0)
        {
            if ((ulRecvDataLen - (unsigned short)((unsigned char *)pcEntityStart - aucPackBuffer)) != ulEntityLen)
            {
                TRACE("content-length is %d, actually %d received\n", ulEntityLen,
                      (ulRecvDataLen - ((unsigned int)pcEntityStart - (unsigned int)aucPackBuffer)));
                if (ulRecvDataLen >= sizeof(aucPackBuffer) - 1)
                {
                    TRACE("short buffer error!\n");
                    return RET_ERR;
                }
                //block_recv_left_data_ext();
                {
                    TRACE("try to receive left data\n");
                    ulTimeout = Lib_SysGetTimer();
                    do
                    {
                        ulLen = sizeof(aucPackBuffer) - ulRecvDataLen - 1;
                        sRC = UIWireNetTcpRxExt(handle,aucPackBuffer + ulRecvDataLen, &ulLen, NET_MAX_RX_TIME, PACKET_TYPE_RAW_DATA, Call);
                    } while (sRC < RET_OK && !IS_TIME_OUT(ulTimeout, 3000));
                    if (sRC != RET_OK)
                    {
                        TRACE("failed to receive left data\n");
                        return RET_ERR;
                    }
                    ulRecvDataLen += ulLen;
                    goto reparse;
                }
            }

            if (ulEntityLen > *pulLen)
            {
                TRACE("short buffer error, %d needed, %d provided\n", ulEntityLen, *pulLen);
                return RET_ERR;
            }
            memcpy(pucBuf, pcEntityStart, ulEntityLen);
            pucBuf[*pulLen] = 0;
            return RET_OK;
        }
        else
        {
            TRACE("ucChunked ==  0 and usEntityLen == 0!\n");

            if (strncasecmp((char *)(pcEntityStart + ulEntityLen - 7), pcTag3, strlen(pcTag3)) != 0)
            {
                FUN_IN;
                //block_recv_left_data_ext();
                {
                    TRACE("try to receive left data\n");
                    ulTimeout = Lib_SysGetTimer();
                    do
                    {
                        ulLen = sizeof(aucPackBuffer) - ulRecvDataLen - 1;
                        sRC = UIWireNetTcpRxExt(handle,aucPackBuffer + ulRecvDataLen, &ulLen, NET_MAX_RX_TIME, PACKET_TYPE_RAW_DATA, Call);
                    } while (sRC < RET_OK && !IS_TIME_OUT(ulTimeout, 3000));
                    if (sRC != RET_OK)
                    {
                        TRACE("failed to receive left data\n");
                        return RET_ERR;
                    }
                    ulRecvDataLen += ulLen;
                    goto reparse;
                }
            }

            if (ulEntityLen > *pulLen)
            {
                TRACE_LOG("short buffer error, %d needed, %d provided\n", ulEntityLen, *pulLen);
                return RET_ERR;
            }
            TRACE_LOG("usEntityLen = %d", ulEntityLen);
            ulEntityLen = ulRecvDataLen - (unsigned short)((unsigned char *)pcEntityStart - aucPackBuffer);
            if (ulEntityLen > *pulLen)
            {
                TRACE("short buffer error, %d needed, %d provided\n", ulEntityLen, *pulLen);
                return RET_ERR;
            }
            memcpy(pucBuf, pcEntityStart, ulEntityLen);
            *pulLen = ulEntityLen;
            pucBuf[*pulLen] = 0;
            return RET_OK;
        }
    }
    else
    {
        TRACE("Chunk type, receiving HTTP data usRecvDataLen:%d\n\n", ulRecvDataLen);
        // Chunk type, receiving HTTP header data
        ulEntityLen = ulRecvDataLen - (unsigned short)((unsigned char *)pcEntityStart - aucPackBuffer);
        char *pTemp = (char *)(pcEntityStart + ulEntityLen - 7);
        TRACE("1111 pcTag3\n");
        TRACEBUF((unsigned char *)pTemp, 7);
        if (strncasecmp((char *)(pcEntityStart + ulEntityLen - 7), pcTag3, strlen(pcTag3)) != 0)
        {
            FUN_IN;
            //block_recv_left_data_ext();
            {
                TRACE("try to receive left data\n");
                ulTimeout = Lib_SysGetTimer();
                do
                {
                    ulLen = sizeof(aucPackBuffer) - ulRecvDataLen - 1;
                    sRC = UIWireNetTcpRxExt(handle,aucPackBuffer + ulRecvDataLen, &ulLen, NET_MAX_RX_TIME, PACKET_TYPE_RAW_DATA, Call);
                } while (sRC < RET_OK && !IS_TIME_OUT(ulTimeout, 3000));

                if (sRC != RET_OK)
                {
                    TRACE("failed to receive left data\n");
                    return RET_ERR;
                }
                ulRecvDataLen += ulLen;
                goto reparse;
            }
        }

        if (ulEntityLen > *pulLen)
        {
            TRACE_LOG("short buffer error, %d needed, %d provided\n", ulEntityLen, *pulLen);
            return RET_ERR;
        }
        TRACE_LOG("usEntityLen = %d", ulEntityLen);

#if 0
        memcpy(pucBuf + ulHeadLen, pcEntityStart, ulEntityLen);
        *pulLen = ulHeadLen + ulEntityLen;
        pucBuf[*pulLen] = 0;
#else
        int ulLeftLen = ulEntityLen;
        unsigned short count = 0;
        unsigned int ulOffset = 0;
        pcPtr = pcEntityStart;
        while (1)
        {
            pcTmp = strstr(pcPtr, "\r\n");
            count = pcTmp-pcPtr;

            memset(ucbuf, 0, sizeof(ucbuf));
            memcpy(ucbuf, pcPtr, count);
            ulLen = strtol(ucbuf, NULL, 16);
            if (ulLen == 0 || ulLen >= *pulLen)
            break;

            pcPtr = pcTmp;
            pcPtr += 2;
            if (ulLen < ulLeftLen)
            {
                // Parse the first paragraph of the body from the data collected above
                memcpy(pucBuf + ulOffset, pcPtr, ulLen);
                ulLeftLen = ulLeftLen - ulLen;
                ulOffset += ulLen;
                pcPtr += ulLen;
                pcPtr += 2;
            }
            else
            {
                memcpy(pucBuf+ulOffset, pcPtr, ulLeftLen);
                ulOffset += ulLeftLen;
                break;
            }
        }

        *pulLen = ulOffset;
        pucBuf[*pulLen] = 0;

#endif


#if 0
        int fd_record = Lib_FileOpen ("packaget.bin", LFS_O_RDWR | LFS_O_CREAT);
        if ( fd_record <= 0 )
        {
            TRACE ( "Lib_FileOpen create fail,%d", fd_record );
            return RET_ERR;
        }
        int ret = Lib_FileWrite (fd_record, ( uchar* )pucBuf, *pulLen);
        if ( ret < 0 )
        {
            TRACE ( "ApiAddRec write fail,%d", ret );
            return RET_ERR;
        }
        ret = Lib_FileClose ( fd_record );
        if ( ret < 0 )
        {
            TRACE ( "ApiAddRec save fail,%d", ret );
            return RET_ERR;
        }
#endif

        return RET_OK;
    }
}


//===================================================================================================================================
// The following are the APIS for multi-channel communication functions in UI level.
// It is recommended to use this new APIs, and the old Apis is not recommended to continue to be used.
// Update date: 2024.04.29
//===================================================================================================================================


//-----------------------------------------------------------------------------
// Name  : GetServerIpPort
// Func  : Get IP and port data
// Paras : pucOutIp:ip output cache buffer
//         pusOutPort:Port output cache buffer
// Ret   :
// Desc  :
//-----------------------------------------------------------------------------
short GetServerIpPort(EN_SERVER_OPT stServerOpt, char *pucOutIp, unsigned short *pusOutPort)
{
    unsigned char *server = NULL;
    unsigned char *port = NULL;

    switch(stServerOpt)
    {
    case SERVER_OPT_POSP_MAIN:
        if (g_stServerParam.ucUseDomainNameEn == '1')
        {
            server = g_stServerParam.aucDomain1;
            port = g_stServerParam.aucDomainPort1;
            memcpy(pucOutIp, server, strlen((char *)server));
            pucOutIp[strlen((char *)server)] = 0;
        }
        else
        {
            server = g_stServerParam.aucIP1;
            port = g_stServerParam.aucPort1;
            memcpy(pucOutIp, server, sizeof(g_stServerParam.aucIP1));
        }
        break;
    case SERVER_OPT_POSP_BACKUP:
        if (g_stServerParam.ucUseDomainNameEn == '1')
        {
            server = g_stServerParam.aucDomain2;
            port = g_stServerParam.aucDomainPort2;
            memcpy(pucOutIp, server, strlen((char *)server));
            pucOutIp[strlen((char *)server)] = 0;
        }
        else
        {
            server = g_stServerParam.aucIP2;
            port = g_stServerParam.aucPort2;
            memcpy(pucOutIp, server, sizeof(g_stServerParam.aucIP1));
        }
        break;

    case SERVER_OPT_TMS_MAIN:
        if (g_stTmsParam.stTmsServerParam.ucUseDomainNameEn == '1')
        {
            server = g_stTmsParam.stTmsServerParam.aucDomain1;
            port = g_stTmsParam.stTmsServerParam.aucDomainPort1;
            memcpy(pucOutIp, server, strlen((char *)server));
            pucOutIp[strlen((char *)server)] = 0;
        }
        else
        {
            server = g_stTmsParam.stTmsServerParam.aucIP1;
            port = g_stTmsParam.stTmsServerParam.aucPort1;
            memcpy(pucOutIp, server, sizeof(g_stTmsParam.stTmsServerParam.aucIP1));
        }
        break;

    case SERVER_OPT_TMS_BACKUP:
        if (g_stTmsParam.stTmsServerParam.ucUseDomainNameEn == '1')
        {
            server =  g_stTmsParam.stTmsServerParam.aucDomain2;
            port = g_stTmsParam.stTmsServerParam.aucDomainPort2;
            memcpy(pucOutIp, server, strlen((char *)server));
            pucOutIp[strlen((char *)server)] = 0;
        }
        else
        {
            server = g_stTmsParam.stTmsServerParam.aucIP2;
            port = g_stTmsParam.stTmsServerParam.aucPort2;
            memcpy(pucOutIp, server, sizeof(g_stTmsParam.stTmsServerParam.aucIP2));
        }
        break;

    case SERVER_OPT_TMS_MODULE:
        server = g_stTmsParam.stTmsModuleParam.aucModuleDomain1;
        port = g_stTmsParam.stTmsModuleParam.aucModuleDomainPort1;
        memcpy(pucOutIp, server, strlen((char *)server));
        pucOutIp[strlen((char *)server)] = 0;
        *pusOutPort = atoi((const char*)port);
        break;

    default:
        return RET_ERR_PARAMETER;
    }

    *pusOutPort = atoi((const char*)port);

    return RET_OK;
}


//-----------------------------------------------------------------------------
// Name    : UIWireNetTcpLinkExt
// Func    : Create TCP network connection function
// Paras   : handle    : Used to record the socket number of new connections
//           protocol  : The network protocol type of the new link
//           timeout   : Timeout value,in milliseconds
//           stSerOpt  : Connected business server number
//           Call      : The callback function for connection, refer to Net_ConnectCb
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UIWireNetTcpLinkExt(int* handle, NETWORK_TYPE protocol, int timeout, EN_SERVER_OPT stServerOpt, int (*Call)(int timeout, int ct))
{
    char  acDomainName[256];
    unsigned char ucReLinkCount = 0;
    unsigned short usIpPort;
    int ret;

    SetServerOpt(stServerOpt);

NET_TRY:
    if (Net_StatusEx(*handle) == CFW_SOCKET_CONNECTED)
    {
        FUN_IN;
        return RET_OK;
    }

    UIWireNetCloseExt(handle);
    ucReLinkCount = 0;
    while (1)
    {
        if (Call)
        {
            LVGL_DIALOG dialog = { 0 };

            LvUiCloseDialog(hDialog);
            hDialog = NULL;
            //LvUilCls();

            dialog.font = FONT_SIZE_NORMAL;
            dialog.title = "";
            dialog.msg = (char *)TIP_NET_INIT;
            dialog.gif = UIWireNetDefaultTips();
            dialog.timeout = NET_MAX_INIT_TIME / 1000;
            dialog.isCountDown = TRUE;

            LvUiDialog(&dialog, &hDialog);
        }
        ucReLinkCount++;
        // If multiple re-connection failures, restart the module and then exit
        if (ucReLinkCount > 3)
        {
            UIWireNetCloseExt(handle);
            break;
        }

        if (NULL == Call)
        {
            ret = Net_OpenEx(handle, protocol, NET_MAX_INIT_TIME, NULL);
        }
        else
        {
            ret = Net_OpenEx(handle, protocol, NET_MAX_INIT_TIME, Net_OpenCb);
        }

        TRACE_LOG("MultiChHandle = 0x%x, ret=%d\n", *handle, ret);
        // No SIM card or cancel, exit directly
        if ((ret == UI_INPUT_CANCEL) || (ret == WLS_NOSIM))
        {
            if (hDialog)
            {
                LvUiCloseDialog(hDialog);
            }
            hDialog = NULL;
            UIWireNetCloseExt(handle);
            return ret;
        }
        else if (ret == UI_INPUT_TIMEOUT)
        {
            if (hDialog)
            {
                LvUiCloseDialog(hDialog);
            }
            hDialog = NULL;
            // If initialization timeout, close Connection and exit
            //Net_Reset(NET_MAX_INIT_TIME);
            UIWireNetCloseExt(handle);
            break;
            //return ret;
        }
        else if (ret == UI_SWITCH_GPRS)
        {
            goto NET_TRY;
        }
        else if (ret < 0)
        {
            // Other errors, try again after restarting
            UIWireNetCloseExt(handle);
            continue;
        }
        if (Call)
        {
            LVGL_DIALOG dialog = { 0 };

            dialog.title = "";
            dialog.msg = (char *)TIP_NET_CONNECT;
            dialog.gif = UIWireNetDefaultTips();
            dialog.timeout = timeout / 1000;
            dialog.isCountDown = TRUE;

            LvUiRefreshDialog(hDialog, &dialog);
        }
        // Get server information
        memset(acDomainName, 0, sizeof(acDomainName));
        GetServerIpPort(stServerOpt, acDomainName, &usIpPort);
        TRACE_LOG("acDomainName=%s:%d", acDomainName, usIpPort);
        ret = Net_ConnectEx(*handle, acDomainName, usIpPort, timeout, Call);

        if (hDialog)
        {
            LvUiCloseDialog(hDialog);
        }
        hDialog = NULL;

        // User cancell
        if (ret == UI_INPUT_CANCEL)
        {
            UIWireNetCloseExt(handle);
            FUN_IN_ERR;
            return UI_INPUT_CANCEL;
        }
        else if (ret == UI_SWITCH_GPRS)
        {
            UIWireNetCloseExt(handle);
            goto NET_TRY;
        }
        else if (ret < 0 && (RET_ERR_PARAMETER != ret))
        {
            UIWireNetCloseExt(handle);
            FUN_IN_ERR;
            TRACE_LOG("ret = %d", ret);
            continue;
        }
        else
        {
            FUN_IN;
            TRACE_LOG("ret = %d", ret);
            return RET_OK;
        }
    }

    UIWireNetCloseExt(handle);
    FUN_IN_ERR;
    return RET_ERR;
}


//-----------------------------------------------------------------------------
// Name    : UIWireNetTcpTx
// Func    : Network TCP send data function
// Paras   : handle    : Network socket number
//           data      : Cache buffer for sending data;
//           data_len  : The length of the sending data
//           timeout   : send data timeout value,in milliseconds
//           Call      : The callback function for sending data, refer to Net_WriteCb
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UIWireNetTcpTxExt(int handle, uint8_t *data, uint16_t data_len, int timeout, int (*Call)(void))
{
    int ret;
    if (Call)
    {
        LVGL_DIALOG dialog = { 0 };

        dialog.font = FONT_SIZE_NORMAL;
        dialog.title = "";
        dialog.msg = (char *)TIP_NET_SEND_DATA;
        dialog.gif = UIWireNetDefaultTips();
        dialog.timeout = NET_MAX_TX_TIME / 1000;
        dialog.isCountDown = TRUE;

        if (hDialog)
        {
            LvUiRefreshDialog(hDialog, &dialog);
        }
        else
        {
            LvUiDialog(&dialog, &hDialog);
        }

        LvUiWaitKey(40);
    }
    TRACE("TCP SEND:%d\n", data_len);
    TRACEBUF(data, data_len);
    ret = Net_WriteEx(handle, data, data_len, Call);
    TRACE_LOG("ret = %d",ret);
    if (hDialog)
    {
        LvUiCloseDialog(hDialog);
    }
    hDialog = NULL;
    if (ret < 0)
    {
        FUN_IN;
        //Net_CloseEx(handle);
        return RET_ERR;
    }
    return RET_OK;
}


//-----------------------------------------------------------------------------
// Name    : UIWireNetTcpRxExt
// Func    : Network TCP receive data extend function with specified socket number
// Paras   : handle    : Network socket number
//           recv      : Cache buffer for storing data;
//           recv_size : The maximum length of the receiving cache buffer
//           timeout   : Receive timeout value,in milliseconds
//           PacketType: Format of data packets received from network
//           Call      : The callback function for receiving, refer to Net_ReadCb
// Ret     : 0: Received successfully, received all data, or received maximum buffer length
// Desc    :
//-----------------------------------------------------------------------------
short UIWireNetTcpRxExt(int handle, uint8_t *recv, unsigned long *recv_size, int timeout, uint8_t PacketType, int (*Call)(int timeout, int ct))
{
    int ret = -1;
    // Received data length
    int len = 0;
    int recvLen = 0;
    int total_num = 0;
    int buffLen = *recv_size;

    if (Call)
    {
        LVGL_DIALOG dialog = { 0 };

        dialog.font = FONT_SIZE_NORMAL;
        dialog.title = "";
        dialog.msg = (char *)TIP_NET_READ_DATA;
        dialog.gif = UIWireNetDefaultTips();
        dialog.timeout = NET_MAX_RX_TIME / 1000;
        dialog.isCountDown = TRUE;

        if (hDialog)
        {
            LvUiRefreshDialog(hDialog, &dialog);
        }
        else
        {
            LvUiDialog(&dialog, &hDialog);
        }

        LvUiWaitKey(40);
    }
    if (PACKET_TYPE_ISO8583 == PacketType)
    {
        // ISO8583 message reception

        ret = Net_ReadEx(handle, recv, *recv_size, timeout, Call);
        if(ret < 2)
        {
            if((ret != UI_INPUT_CANCEL)&&(ret != UI_INPUT_TIMEOUT))
            {
                ret = RET_ERR;
            }
            // Abnormal situation, close fd
            //close_flag = 1;
            goto RecvError;
        }
        len = ret;

        total_num = MAKEWORD(recv[0], recv[1]);
        total_num += 2;
        TRACE_LOG("Total Need  recv:%d data, total_num  = %d\n", len, total_num);

        while (total_num > len)
        {
            ret = Net_ReadEx(handle, recv + len, *recv_size - len, timeout, Call);
            if (ret < 0)
            {
                // Abnormal situation, close fd
                //close_flag = 1;
                goto RecvError;
            }
            len += ret;
        }

        *recv_size = len;
    }
    else if (PACKET_TYPE_RAW_DATA == PacketType)
    {
        // Network raw data reception
        // Loop receiving data
        while (1)
        {
            if (buffLen < SINGLE_RECV_LEN)
            {
                ret = Net_ReadEx(handle, recv + recvLen, buffLen, timeout, Call);
            }
            else
            {
                ret = Net_ReadEx(handle, recv + recvLen, SINGLE_RECV_LEN, timeout, Call);
            }
            if (ret < 0)
            {
                // Abnormal situation, close fd
                TRACE("UIWireNetTcpRxExt = %d",ret);
                //close_flag = 1;
                goto RecvError;
            }

            recvLen += ret;

            if (ret < SINGLE_RECV_LEN)
            {
                FUN_IN;
                TRACE("ret = %d",ret);
                // Data was read, but the length of the read is less than SINGLE_RECV_LEN bytes
                break;
            }

            buffLen -= SINGLE_RECV_LEN;

            if (buffLen == 0)
            {
                break;
            }
        }
        *recv_size = recvLen;
    }

    ret = RET_OK;

RecvError:

    if (hDialog)
    {
        LvUiCloseDialog(hDialog);
    }
    hDialog = NULL;
    TRACE("TCP RECV:%d\n", recvLen);
    TRACEBUF(recv, recvLen);
    return ret;
}


//-----------------------------------------------------------------------------
// Name    : UIWireNetCloseExt
// Func    : Close the connection with the specified serial number
// Paras   : handle   : Network socket number
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UIWireNetCloseExt(int *piHandle)
{
    int handle = *piHandle;
    int iRc = RET_ERR;

    if (handle <= 0)
    {
        return RET_ERR;
    }

    iRc = Net_CloseEx(handle);

    if (RET_OK == iRc)
    {
        // reset handle to invalid value
        *piHandle = NET_INVALID_HANDLE;
    }
    else
    {
        TRACE("Socekt Handle:%d close Failed.\n", handle);
    }

    return iRc;
}

