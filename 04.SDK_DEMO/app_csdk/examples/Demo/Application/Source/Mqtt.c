/**********************************************************************************************************
 * Copyright(C)2021,
 * File Name     : Mqtt.c
 *  Version      :
 *  Author       :
 * Description   : Mqtt function implementation
 **********************************************************************************************************/

#include "Common.h"
#include "LibApi.h"
#include "Mqtt.h"
#include "Drv.h"
#include "key.h"
#include "LibSysapi.h"
#include "Communication.h"
#include "Power.h"
#include "TradeRecord.h"
#include "M_NetInterface.h"
#include "cJSON.h"
#include "MessageBox.h"
#include "EmvType.h"

#if defined(MQTT_SUPPORT)
// Network status of mqtt
enum
{
    MQTT_STEP_CLOSE_NET = 0,        // 0 -- Enter airplane mode
    MQTT_STEP_OPEN_NET,             // 1 -- Exit airplane mode
    MQTT_STEP_CHECK_NET,            // 2 -- Check check
    MQTT_STEP_DETECT_NET,           // 3 -- Checking network status
    MQTT_STEP_DISCONNECT,           // 4 -- MQTT disconnect
    MQTT_STEP_DISCONNECTED,         // 5 -- mqtt disconnected
    MQTT_STEP_CONNECT,              // 6 -- mqtt connect
    MQTT_STEP_CONNECTED,            // 7 -- mqtt connected
    MQTT_STEP_SUB,                  // 8 -- mqtt subscribe
    MQTT_STEP_SUBED,                // 9 -- mqtt subscribed
    MQTT_STEP_PING,                 // 10 -- mqtt ping result

    MQTT_STEP_END = 255
};

//Triplet information
#define PRODUCT_TRIPLE_INFO_FILE    "TripleInfo.bin"

ST_CUSTOMSERVER_PARAM g_stCustomServerParam = {0};
ST_MQTTSERVER_PARAM g_stMqttServerParam ={0};

static int Mqtt_ReadMsg(CBuff_t *msg_buff);

static bool g_MQTTTaskStatus = FALSE;

static Mqtt_Para_t s_mqtt_para =
{
    .ping_time = 30,            // Taking into account the network condition, the mqtt heartbeat packet period is 30s
    .clean_session = 1,
    .init_flag = FALSE,
};

static unsigned char step = MQTT_STEP_DETECT_NET;  // The status of the state machine

Mqtt_Result_t       g_mqtt_result;
Mqtt_Op_t           g_mqtt_op;

#if 1
int GetMqttInfo(Mqtt_Para_t *para)
{
    int ret = RET_ERR;
    iotx_mqtt_region_types_t region = IOTX_CLOUD_REGION_SHANGHAI;
    char *topicfmt="/%s/%s/user/get";
    iotx_dev_meta_info_t meta;
    iotx_sign_mqtt_t signout;
    memset(meta.product_key, 0, IOTX_PRODUCT_KEY_LEN + 1);
    memset(meta.device_name, 0, IOTX_DEVICE_NAME_LEN + 1);
    memset(meta.device_secret, 0, IOTX_DEVICE_SECRET_LEN + 1);

    if (ReadFile(PRODUCT_TRIPLE_INFO_FILE, 0, meta.product_key, IOTX_PRODUCT_KEY_LEN) <= 0)
    {
        TRACE_LOG("RET FALSE");
        return ret;
    }
    TRACE("GetMqttInfo product_key = %s", meta.product_key);
    if (ReadFile(PRODUCT_TRIPLE_INFO_FILE, 128, meta.device_name, IOTX_DEVICE_NAME_LEN) <= 0)
    {
        TRACE_LOG("RET FALSE");
        return ret;
    }
    TRACE("GetMqttInfo device_name = %s", meta.device_name);
    if (ReadFile(PRODUCT_TRIPLE_INFO_FILE, 256, meta.device_secret, IOTX_DEVICE_SECRET_LEN) <= 0)
    {
        TRACE_LOG("RET FALSE");
        ret = FALSE;
        return ret;
    }
    TRACE("GetMqttInfo device_secret = %s", meta.device_secret);

    sprintf(para->topic, topicfmt, meta.product_key, meta.device_name);  //MQTT subscribes to the unique element topic
    TRACE("GetMqttInfo topic = %s", para->topic);

    if (Lib_MqttIotSign(region, &meta, &signout) != 0)   //The device triplet information is read and stored in meta, and then OS is called to directly process the calculation of subsequent MQTT connection component elements
    {
        TRACE_LOG("RET FALSE");
        return ret;
    }

    //MQTT connects the five elements hostname，port，clientid，username，password
    TRACE_LOG("GetMqttInfo hostname = %s", signout.hostname);
    TRACE_LOG("GetMqttInfo port = %d", signout.port);
    TRACE_LOG("GetMqttInfo clientid = %s", signout.clientid);
    TRACE_LOG("GetMqttInfo username = %s", signout.username);
    TRACE_LOG("GetMqttInfo password = %s", signout.password);

    //Transfer the above five elements to PARA and call back to mqtt.c file for processing
    memset(para->server, 0, 128);
    if (strlen(signout.hostname) > 128)
    {
        TRACE_LOG("RET FALSE");
        return ret;
    }
    memcpy(para->server, signout.hostname, strlen(signout.hostname));
    para->port = signout.port;
    memset(para->client_id, 0, 128);
    if (strlen(signout.clientid) > 128)
    {
        TRACE_LOG("RET FALSE");
        return ret;
    }
    memcpy(para->client_id, signout.clientid, strlen(signout.clientid));
    memset(para->user, 0, 128);
    if (strlen(signout.username) > 128)
    {
        TRACE_LOG("RET FALSE");
        return ret;
    }
    memcpy(para->user, signout.username, strlen(signout.username));
    memset(para->password, 0, 128);
    if (strlen(signout.password) > 128)
    {
        TRACE_LOG("RET FALSE");
        return ret;
    }
    memcpy(para->password, signout.password, strlen(signout.password));

    para->init_flag = TRUE;
    return RET_OK;
}

#else
int GetMqttInfo(Mqtt_Para_t *para)
{
    //int len = 0;
    int ret = RET_ERR;

	//MQTT connects the five elements hostname，port，clientid，username，password
    TRACE_LOG("GetMqttInfo hostname = %s", g_stMqttServerParam.aucMqttHostName1);
    TRACE_LOG("GetMqttInfo port = %s", g_stMqttServerParam.aucPort1);
    TRACE_LOG("GetMqttInfo clientid = %s", g_stCustomServerParam.aucClientID);
    TRACE_LOG("GetMqttInfo username = %s", g_stCustomServerParam.aucUserName);
    TRACE_LOG("GetMqttInfo password = %s", g_stCustomServerParam.aucPwd);
    TRACE_LOG("GetMqttInfo topic = %s",  g_stCustomServerParam.aucTopic1);

	//Transfer the above five elements to PARA and call back to mqtt.c file for processing
    memset(para->server, 0, NETWORK_MQTT_SIZE);
    if(strlen((char*)g_stMqttServerParam.aucMqttHostName1) > NETWORK_MQTT_SIZE)
    {
        TRACE_LOG("RET FALSE");
        return ret;
    }
    memcpy(para->server, g_stMqttServerParam.aucMqttHostName1, strlen((char*)g_stMqttServerParam.aucMqttHostName1));

    para->port = atoi((const char *)g_stMqttServerParam.aucPort1);

    memset(para->client_id, 0, NETWORK_MQTTCLIENTID_SIZE);
    if(strlen((char*)g_stCustomServerParam.aucClientID) > NETWORK_MQTTCLIENTID_SIZE)
    {
        TRACE_LOG("RET FALSE");
        return ret;
    }
    memcpy(para->client_id, g_stCustomServerParam.aucClientID, strlen((char*)g_stCustomServerParam.aucClientID));

    memset(para->user, 0, NETWORK_MQTTUSENAME_SIZE);
    if(strlen((char*)g_stCustomServerParam.aucUserName) > NETWORK_MQTTUSENAME_SIZE)
    {
        TRACE_LOG("RET FALSE");
        return ret;
    }
    memcpy(para->user, g_stCustomServerParam.aucUserName, strlen((char*)g_stCustomServerParam.aucUserName));

    memset(para->password, 0, NETWORK_MQTTPWD_SIZE);
    if(strlen((char*)g_stCustomServerParam.aucPwd) > NETWORK_MQTTPWD_SIZE)
    {
        TRACE_LOG("RET FALSE");
        return ret;
    }
    memcpy(para->password, g_stCustomServerParam.aucPwd, strlen((char*)g_stCustomServerParam.aucPwd));

    memset(para->topic, 0, NETWORK_MQTTTOPIC_SIZE);
    if(strlen((char*)g_stCustomServerParam.aucTopic1) > NETWORK_MQTTTOPIC_SIZE)
    {
        TRACE_LOG("RET FALSE");
        return ret;
    }
    memcpy(para->topic, g_stCustomServerParam.aucTopic1, strlen((char*)g_stCustomServerParam.aucTopic1));

    para->init_flag = TRUE;
    return RET_OK;
}
#endif

//-----------------------------------------------------------------------------
// Name: CatchJson
// Function: Read the valid packet from the loop queue
// Parameters: msg_buff - Data stored in loop queue, ubff, len
// Return: Returns the number of bytes of a valid message
// Instructions:
//-----------------------------------------------------------------------------
int CatchJson(CBuff_t *msg_buff, unsigned char *buff, int len)
{
    int cur;
    int msg_len;
    signed char data;
    int count;
    int ret;

    if (msg_buff == NULL)
    {
        FUN_IN_ERR;
        return RET_ERR;
    }

    cur = 0;
    msg_len = CBuff_Length(msg_buff);
    //  { start
    while (cur < msg_len)
    {
        data = CBuff_Pick(msg_buff, cur);
        if (data == '{')
        {
            CBuff_Move(msg_buff, cur);
            break;
        }
        cur ++;
    }
    // no {
    if (cur >= msg_len)
    {
        FUN_IN_ERR;
        return RET_ERR;
    }

    // Matches {} inside parentheses
    count = 0;
    cur = 0;
    msg_len = CBuff_Length(msg_buff);
    while (cur < msg_len)
    {
        data = CBuff_Pick(msg_buff, cur);
        if (data == '{')
        {
            count++;
        }
        else if (data == '}')
        {
            count--;
            if (count == 0)
            {
                // match
                cur++;
                goto Lable_ret;
            }
        }
        cur++;

    }

    // Did not match
    FUN_IN_ERR;
    return RET_ERR;

Lable_ret:
    if (cur > len)
    {
        FUN_IN_ERR;
        return RET_ERR;
    }
    ret = CBuff_Read(msg_buff, buff, cur);
    return ret;

}

//Time difference
signed long g_SubTimeTemp = 0;

int MsgProcess(CBuff_t *msg_buff, Mqtt_Result_t *result)
{
    unsigned char buf[1024];
    unsigned long localtime;
    ULONGLONG recvTime = 0;

    int len = 0;

    memset(buf, 0, sizeof(buf));
    len = sizeof(buf);
    len = CatchJson(msg_buff, buf, len);
    TRACE_LOG("CatchJsonBuf = %s, RetLen = %d, RealLen = %d", buf, len, strlen((char *)buf));
    if (len < 0)
    {
        FUN_IN_ERR;
        return RET_ERR;
    }

    cJSON *items = cJSON_Parse((char *)buf);
    if (items == NULL)
    {
        TRACE_LOG("cJSON_Parse items malloc fail!");
        return RET_ERR;
    }
    else
    {
        if (items->type != cJSON_Object)
        {
            cJSON_Delete(items);
            FUN_IN;
            return RET_ERR;
        }
    }

    // Timestamp calculation
    cJSON *time_items = cJSON_GetObjectItem(items, "timestamp");
    TRACE_LOG("time_items = 0x%08X", time_items);
    if (time_items != NULL)
    {
        unsigned char bcdTime[6] = {0};
        Lib_GetDateTime(bcdTime);
        Lib_RtcToUtcSec(bcdTime, sizeof(bcdTime), &localtime);
        TRACE_LOG("Localtime = %d", localtime);


        if (time_items->type == cJSON_Number)
        {
            TRACE_LOG("time_items->valuedouble = %f", time_items->valuedouble);
            time_items->valuedouble = time_items->valuedouble / 1000;
            TRACE_LOG("time_items->valuedouble = %f", time_items->valuedouble);
            g_SubTimeTemp = (signed long)(localtime - time_items->valuedouble);
            recvTime = time_items->valuedouble;
        }
        else if (time_items->type == cJSON_String)
        {
            TRACE_LOG("time_items->valuestring = %s", time_items->valuestring);
            recvTime = atoll(time_items->valuestring);
            TRACE_LOG("recvTimel = %lld", recvTime);
            recvTime = recvTime / 1000;
            TRACE_LOG("recvTime = %lld", recvTime);
            g_SubTimeTemp = (signed long)(localtime - (unsigned long)recvTime);
        }
        else
        {
            g_SubTimeTemp = 0;
            TRACE_LOG("timestamp fmt error!");
        }

        // Receive time data
        cJSON *bj_time = cJSON_GetObjectItem(items, "time");
        if (bj_time != NULL)
        {
            if (bj_time->type == cJSON_String)
            {
                TRACE_LOG("bj_time = %s", bj_time->valuestring);
            }
        }

        TRACE_LOG("delay time = %d s", g_SubTimeTemp);
    }

    // Normal transaction amount
    cJSON *money_items = cJSON_GetObjectItem(items, "requestData");

    if (money_items == NULL)
    {
        cJSON_Delete(items);
        FUN_IN;
        return RET_ERR;
    }
    TRACE_LOG("money_items->type = %d", money_items->type);
    if ((money_items->type != cJSON_String) || (money_items->valuestring == NULL) || (strlen(money_items->valuestring) == 0))
    {
        cJSON_Delete(items);
        FUN_IN;
        return RET_ERR;
    }


    // Convert to broadcast dollar numbers
    TRACE_LOG("money_items->valuestring = %s", money_items->valuestring);
    // Pass in more than 10 error, 8 integer + 2 decimal
    if (strlen(money_items->valuestring) > 10)
    {
        TRACE_LOG("price err: %s", money_items->valuestring);
        return RET_ERR;
    }
    result->money = atof(money_items->valuestring);
    TRACE_LOG("FmtMoney = %f", result->money);
    if (result->money <= 0)
    {
        cJSON_Delete(items);
        FUN_IN;
        return RET_ERR;
    }
    //result->money += 0.0049;
    TRACE_LOG("ResMoney = %f", result->money);

    cJSON_Delete(items);

    return RET_OK;
}

// mqtt interface
Mqtt_Op_t g_mqtt_op =
{
    GetMqttInfo,
    MsgProcess,
    NULL,
    NULL,
    NULL,
};

//======================================================================================================
//mqtt receives the packet cache and circulates the queue
static CBuff_t *s_mqtt_msg_buffer = NULL;
static void *s_MqttTaskQueue = NULL;

// Initialization of mqtt task
int Mqtt_Task_Init(void)
{
    if (s_mqtt_msg_buffer != NULL)
    {
        CBuff_Delete(s_mqtt_msg_buffer);
    }

    s_mqtt_msg_buffer = CBuff_New(1024 * 6);
    if (s_mqtt_msg_buffer == NULL)
    {
        FUN_IN_ERR;
        return RET_ERR;
    }

    s_MqttTaskQueue = Lib_osiCreatQueue(2, sizeof(unsigned long));

    return RET_OK;
}

static void CustomServerDefaultParam(void)
{
    char *topicfmt = MQTT_TOPIC_FORMAT;      //topic splicing mode
    char *clientidfmt = MQTT_CLIENTID_FORMAT;//clientid splicing mode
    //int iLen;

    //clientID
    sprintf((char*)g_stCustomServerParam.aucClientID, clientidfmt, MQTT_CLIENTID);

    //TOPIC
    sprintf((char*)g_stCustomServerParam.aucTopic1, topicfmt, MQTT_TOPIC);

    //USERNAME
    memcpy(g_stCustomServerParam.aucUserName, MQTT_USENAME, strlen(MQTT_USENAME));

    //Password
    memcpy(g_stCustomServerParam.aucPwd, MQTT_PWD, strlen(MQTT_PWD));

}


//-----------------------------------------------------------------------------
// Name: CustomServerParamInit
// Function: Initialization of customized server parameters
// Parameters:
// Return：
// Instructions:
//-----------------------------------------------------------------------------
short CustomServerParamInit(void)
{
    /*int curFileSize = -1;
    unsigned char aucBuf[NETWORK_MQTT_SIZE];
    int iLen;*/

    CustomServerDefaultParam();
    return 0;
}


static void MqttServerDefaultParam(void)
{
    memcpy(g_stMqttServerParam.aucMqttHostName1, NET_NAME_MQTT, strlen(NET_NAME_MQTT));
    memcpy(g_stMqttServerParam.aucPort1, NET_NAME_MQTT_PORT, strlen(NET_NAME_MQTT_PORT));
}


//-----------------------------------------------------------------------------
// Name: MqttServerParamInit
// Function: The MQTT server parameter information is initialized
// Parameters:
// Return：
// Instructions:
//-----------------------------------------------------------------------------
short MqttServerParamInit(void)
{
    //int curFileSize = -1;

    MqttServerDefaultParam();
    return 0;
}


//-----------------------------------------------------------------------------
// Name: Mqtt_KeepOnLine_Task
// Function: The mqtt connection task ensures that the mqtt server is connected
// Parameters:
// Return：
// Instructions:
//-----------------------------------------------------------------------------
int Mqtt_KeepOnLine_Task(void *arg)
{
    short ret;
    //static unsigned char step = MQTT_STEP_DETECT_NET;  // The status of the state machine
    static unsigned char net_mode;
    static unsigned long s_time = 0;                      // Overtime
    static unsigned char re_connect_count = 0;        // Retry times
    static unsigned char ucPowerOn_mqtt_connect = 0;  // Power-on MQTT connection times

#if !AUDIO_NET_CONNECT_REPLAY_EN
    static unsigned char ucMqttConnetedAudioPlaySta = FALSE;
#endif

#if OTHER_TOPIC_EN
    static unsigned char ucMqttSubNo = 0;  // Subscription serial number
    static int iMqttSubSendSta = RET_ERR;
#endif

    TRACE_LOG("mqtt ConnectSub Step = %d, s_time = %d", step, s_time);
    switch (step)
    {
        case MQTT_STEP_CLOSE_NET:               //0.Enter airplane mode
#if 0
            net_mode = GetNetWorkMode();
            if (net_mode == NET_MODE_GPRS)
            {
                Lib_WlsSetCFUN(0);
            }
#endif
            s_time = Lib_SysGetTimer();

            step = MQTT_STEP_OPEN_NET;
            break;

        case MQTT_STEP_OPEN_NET:                //1.Exit airplane mode
            if (IS_TIME_OUT(s_time, 1000))
            {
#if 0
                // Opening gprs is equivalent to relocating to the network
                net_mode = GetNetWorkMode();
                if (net_mode == NET_MODE_GPRS)
                {
                    Lib_WlsSetCFUN(1);
                    Lib_WlsInit();
                    TRACE("Lib_WlsInit()");
                }
#endif
                step = MQTT_STEP_DETECT_NET;
            }
            break;

        case MQTT_STEP_CHECK_NET:               //2.Check chec
        	step = MQTT_STEP_DETECT_NET;
            break;

        case MQTT_STEP_DETECT_NET:              //3.Checking network status
            net_mode = GetNetWorkMode();
            TRACE_LOG("net_mode = %d", net_mode);
            if (NET_MODE_GPRS == net_mode && 0 <= Lib_WlsGetInitStatus())
            {
                step = MQTT_STEP_DISCONNECT;
                re_connect_count = 0;
            }
            else if (NET_MODE_WIFI == net_mode && STA_AP_CONNECTED == Lib_WifiGetApStatus())
            {
                step = MQTT_STEP_DISCONNECT;
                re_connect_count = 0;
            }
            else if (IS_TIME_OUT(s_time, 40000))
            {
                // waiting timeout, relock-in
                FUN_IN;
                step = MQTT_STEP_CLOSE_NET;
            }
            break;

        case MQTT_STEP_DISCONNECT:          //4.Start to disconnect MQTT connection
            // disconnect
            ret = Mqtt_Disconnect();
            if (ret == RET_OK)
            {
                s_time = Lib_SysGetTimer();
                step = MQTT_STEP_DISCONNECTED;
            }
            break;

        case MQTT_STEP_DISCONNECTED:        //5.Processing disconnects MQTT results
            if (IS_TIME_OUT(s_time, 800))
            {
                step = MQTT_STEP_CONNECT;
                s_time = Lib_SysGetTimer();
            }
            break;

        case MQTT_STEP_CONNECT:             //6.Start connecting MQTT
            // connect

            TRACE_LOG("s_mqtt_para.server = %s, port:%d", s_mqtt_para.server, s_mqtt_para.port);
            ret = Mqtt_Connect(s_mqtt_para.server, s_mqtt_para.port,
                               s_mqtt_para.client_id, s_mqtt_para.user, s_mqtt_para.password, s_mqtt_para.ping_time, 1);
            if (ret == RET_OK)
            {
                step = MQTT_STEP_CONNECTED;
                re_connect_count++;
                s_time = Lib_SysGetTimer();
            }
            else if (IS_TIME_OUT(s_time, MQTT_KEEP_ONLINE_OUTTIME))      // The mqtt connection timed out and entered airplane mode
            {
                FUN_IN;
                step = MQTT_STEP_CLOSE_NET;
            }
            break;

        case MQTT_STEP_CONNECTED:           //7.Processing connection MQTT results
            // Whether the connection is successful
            ret = Mqtt_GetStates(MQTT_STATE_ID_CONNECT);
            if (ret == TRUE)
            {
                TRACE_LOG("Mqtt server connected successed");
                step = MQTT_STEP_SUB;
                re_connect_count = 0;
                s_mqtt_para.clean_session = 0;      // The session is cleared at startup and will not be cleared after reconnection

                //Boot mqtt connection twice, the second time is not clear session
                if (ucPowerOn_mqtt_connect < 1)
                {
                    ucPowerOn_mqtt_connect ++;
                    step = MQTT_STEP_DISCONNECT;
                }
                else
                {
                }
            }

            if (ret == FALSE)
            {
                if (re_connect_count > 3)
                {
                    // If the connection fails three times, send the disconnection command first, and then enter the flight mode to avoid that the requested resources are not released during the connection
                    Mqtt_Disconnect();
                    // Multiple failed reconnection. We're in airplane mode
                    TRACE_LOG("Mqtt server connected failed");
                    step = MQTT_STEP_CLOSE_NET;
                }
                else
                {
                    // reconnection
                    //step = MQTT_STEP_CONNECT;

                    // If the connection fails, go back to the disconnect step first to pair the connected commands with the disconnected commands.
                    //Otherwise, the underlying logic of the OS is confused and resources cannot be released, resulting in a crash OS
                    step = MQTT_STEP_DISCONNECT;
                }
            }
            else
            {
                if (IS_TIME_OUT(s_time, MQTT_KEEP_ONLINE_OUTTIME))
                {
                    //Mqtt_Disconnect();
                    TRACE_LOG("Mqtt server connected timeout");
                    step = MQTT_STEP_CLOSE_NET;
                }
            }

            break;

        case MQTT_STEP_SUB:                 //8.Start an MQTT subscription
            // subscription
            {
                ret = Mqtt_Subscribe(s_mqtt_para.topic);
                if (ret == RET_OK)
                {
                    step = MQTT_STEP_SUBED;
                }
            }
            break;

        case MQTT_STEP_SUBED:                   //9. Process MQTT subscription results
            ret = Lib_MqttGetState(MQTT_STATE_ID_SUBSCRIBE);
            TRACE_LOG("Mqtt MQTT_STATE_ID_SUBSCRIBE:%d", ret);
            if (ret == TRUE)
            {
                TRACE_LOG("Mqtt subscribe successed");
                step = MQTT_STEP_PING;
            }
            else if (ret == FALSE)
            {
                TRACE_LOG("Mqtt subscribe failed");
                step = MQTT_STEP_CHECK_NET;
            }
            break;

        case MQTT_STEP_PING:                //10.Check the MQTT ping status
            // Checking ping status
            if (Lib_MqttGetState(MQTT_STATE_ID_PING) == FALSE)
            {
                TRACE_LOG("MQTT_STATE_ID_PING = %d", Lib_MqttGetState(MQTT_STATE_ID_PING));
                step = MQTT_STEP_CHECK_NET;
            }
            net_mode = GetNetWorkMode();
            if (NET_MODE_GPRS == net_mode && (0 > Lib_WlsGetInitStatus()))
            {
                TRACE_LOG("mqtt NETGetWorkState = %d", net_mode);
                step = MQTT_STEP_CHECK_NET;
            }
            else if (NET_MODE_WIFI == net_mode && STA_AP_CONNECTED != Lib_WifiGetApStatus())
            {
                TRACE_LOG("mqtt NETGetWorkState = %d Ap_status=%d", net_mode, Lib_WifiGetApStatus());
                step = MQTT_STEP_CHECK_NET;
            }

            break;

        default:
            step = MQTT_STEP_CHECK_NET;
            break;

    }

    return RET_OK;
}

//-----------------------------------------------------------------------------
// Name: Mqtt_Msg_Task
// Function: The message is received, parsed, and broadcast
// Parameters:
// Return：
// Instructions:
//-----------------------------------------------------------------------------
int Mqtt_Msg_Task(void *arg)
{
    int ret;

    ret = Mqtt_ReadMsg(s_mqtt_msg_buffer);
    if (ret <= 0)
    {
        FUN_IN;
        return RET_ERR;
    }

    // Received message
    FUN_IN;

    // Clear broadcast of the deal
    memset(&g_mqtt_result, 0, sizeof(Mqtt_Result_t));

    // File parsing
    ret = g_mqtt_op.Msg_Process(s_mqtt_msg_buffer, &g_mqtt_result);
    if (ret != RET_OK)
    {
        FUN_IN;
        return RET_ERR;
    }

    return RET_OK;
}

void MqttScriTimeoutProcess(void)
{

}

//-----------------------------------------------------------------------------
// Name: Mqtt_GetConnectInfo_Task
// Function: Gets the information needed to connect to the MQTT service
// Parameters:
// Return：Return success flag
// Instructions:
//-----------------------------------------------------------------------------
int Mqtt_GetConnectInfo_Task(void *arg)
{
    // Once you get it, you don't need to get it again
    if (s_mqtt_para.init_flag == FALSE)
    {
        g_mqtt_op.Connect_Info(&s_mqtt_para);
    }

    return s_mqtt_para.init_flag;
}

//-----------------------------------------------------------------------------
// Name: Mqtt_ReadMsg
// Function: Read mqtt data and put it in a loop queue
// Parameters: Length of data in the queue
// Return：
// Instructions:
//-----------------------------------------------------------------------------
static int Mqtt_ReadMsg(CBuff_t *msg_buff)
{
    unsigned char msg[1024 * 2];
    unsigned short msg_len;
    int ret;
    int len;
    static unsigned long ulClearCBuffTimeOut = 0;

    TRACE_LOG("mqtt Mqtt_ReadMsg");

    len = CBuff_Capacity(msg_buff);
    TRACE_LOG("CBuff capacity = %d", len);
    msg_len = sizeof(msg);
    if (len < msg_len)
    {
        msg_len = len;
    }
    memset(msg, 0, sizeof(msg));
    ret = Mqtt_Recv(msg, &msg_len);
    if (ret == RET_OK)
    {
        TRACE_LOG("WriteMqttMsg = %s, WriteMsgLen = %d", msg, msg_len);
        ret = CBuff_Write(msg_buff, msg, msg_len);
        ulClearCBuffTimeOut = Lib_SysGetTimer();
    }

    len = CBuff_Length(msg_buff);
    if (IS_TIME_OUT(ulClearCBuffTimeOut, 5000))
    {
        if (len > 0)
        {
            TRACE_LOG("RxErrMqtt ClearCBuff.");
            TRACEBUF((unsigned char*)msg_buff, len);
            CBuff_Clean(msg_buff);
            len = 0;
            ulClearCBuffTimeOut = 0;
        }
    }

    return len;

}

static int mqttFd = -1;
#define IS_IP(ip) ((ip >= 0) && (ip <= 255) ? 1 : 0)
static short Net_UrlToIpStr(char *server, char *Ip, char IpLen)
{
    int ip_1, ip_2, ip_3, ip_4;
    int ret = -1;

    if ((Ip == NULL) || (IpLen <= 0))
    {
        return RET_ERR;
    }

    // If IP format is: "112.1.1.102"
    ret = sscanf(server, "%d.%d.%d.%d", &ip_1, &ip_2, &ip_3, &ip_4);
    if (ret == 4)
    {
        if (IS_IP(ip_1) && IS_IP(ip_2) && IS_IP(ip_3) && IS_IP(ip_4))
        {
            // ip
            //goto Label;
            memset(Ip, 0, IpLen);
            memcpy(Ip, server, strlen(server));
            return RET_OK;
        }
    }

    memset(Ip, 0, IpLen);
    ret = Lib_WlsDnsResolve(server, Ip, IpLen);
    if (ret < 0)
    {
        TRACE_LOG("Net_UrlIpToAddrEx dns resolve failed, ret = %d", ret);
        return RET_ERR;
    }
    TRACE_LOG("Net_UrlIpToAddrEx ip_buff = %s", Ip);
    return RET_OK;
}

int Mqtt_GetStates(MQTT_STATE_ID eId)
{
    return Lib_MqttGetStates(mqttFd, eId);
}


//-----------------------------------------------------------------------------
// Name: Mqtt_Connect
// Function: MQTT connection
// Parameters:  server:     domain name of the MQTT server
//              port:       indicates the port number
//              cid:        indicates the device id
//              user:       indicates the user name
//              password:   indicates the password
//              ping_time:  indicates the automatic ping time
// Return：
// Instructions:
//-----------------------------------------------------------------------------
int Mqtt_Connect(char *server, unsigned short port, char *cid, char *user, char *password, int keepalive, int cleansession)
{
    int fd;
    char buff[48];
    int ret;
    ST_MQTT_CONNECT_INFO mqttConnectInfo = { 0 };
    char ip_buff[48];

    mqttConnectInfo.m_pcServer = server;
    if (GetNetWorkMode() == NET_MODE_GPRS)
    {
        ret = Net_UrlToIpStr(server, buff, sizeof(buff));
        TRACE_LOG("ret=%d:%s", ret, server);
        TRACEBUF((unsigned char*)ip_buff, sizeof(ip_buff));
        if (ret == RET_OK)
        {
            memcpy(ip_buff, buff, sizeof(buff));
            mqttConnectInfo.m_pcServer = ip_buff;
        }
        else
        {
            return RET_ERR;
        }
    }

    mqttConnectInfo.m_usPort = port;
    mqttConnectInfo.m_pcClientId = cid;
    mqttConnectInfo.m_pcUserName = user;
    mqttConnectInfo.m_pcPassword = password;
    mqttConnectInfo.m_usKeepAliveSeconds = keepalive;
    mqttConnectInfo.m_bCleanSession = cleansession;

    fd = Lib_MqttConnect(NETWORK_TCP, NULL, &mqttConnectInfo);
    TRACE_LOG("fd=%d", fd);

    if (fd < 0)
    {
        mqttFd = -1;
        return RET_ERR;
    }
    else
    {
        mqttFd = fd;
        return RET_OK;
    }
}

//-----------------------------------------------------------------------------
// Name: Mqtt_Disconnect
// Function: The MQTT disconnects
// Parameters:
// Return：
// Instructions:
//-----------------------------------------------------------------------------
int Mqtt_Disconnect(void)
{
    if (mqttFd < 0)
    {
        return RET_OK;;
    }

    return Lib_MqttDisconnect(mqttFd);
}

//-----------------------------------------------------------------------------
// name：Mqtt_Subscribe
// Function: MQTT Subscription
// Parameters:  topic: The topic
// Return：
// Instructions:
//-----------------------------------------------------------------------------
int Mqtt_Subscribe(char *topic)
{
    ST_MQTT_SUB_INFO subscription;

    subscription.m_eQos = MQTTQoS1;
    subscription.m_pcTopicFilter = topic;
    subscription.m_usTopicFilterLen = strlen(topic);

    return Lib_MqttSubscribe(mqttFd, &subscription);
}

//-----------------------------------------------------------------------------
// Name: Mqtt_Unsub
// Function: MQTT unsubscribe
// Parameters:  topic: The topic
// Return：
// Instructions:
//-----------------------------------------------------------------------------
int Mqtt_Unsub(char *topic)
{
    return RET_ERR;
}

//-----------------------------------------------------------------------------
// Name: Mqtt_Pub
// Function: MQTT Release
// Parameters:  topic:   indicates the topic
//              data:   data buff
//              len:    indicates the data length
// Return：
// Instructions:
//-----------------------------------------------------------------------------
int Mqtt_Pub(char *topic, unsigned char *data, int len)
{
    return RET_ERR;
}

//-----------------------------------------------------------------------------
// Name: Mqtt_Recv
// Function: MQTT receives messages for subscriptions
// Parameters:  pucBuf: indicates the receiving length
//              pusLen: Input receive buff length, output actual length
// Return：
// Instructions:
//-----------------------------------------------------------------------------
int Mqtt_Recv(unsigned char *pucBuf, unsigned short *pusLen)
{
    return Lib_MqttRecv(mqttFd, pucBuf, pusLen);
}

bool MqttGetTaskStatus(void)
{
    return g_MQTTTaskStatus;
}

void MqttSetTaskStatus(bool status)
{
    if (g_MQTTTaskStatus == status)
    {
        return;
    }

    g_MQTTTaskStatus = status;

    if (g_MQTTTaskStatus)
    {
        int s = status;
        Lib_osiQueueSend(s_MqttTaskQueue, (void *)&s, 0);
    }
}

//-----------------------------------------------------------------------------
// Name: MqttTask
// Function: MQTT duty inlets
// Parameters:
// Return：
// Instructions:
//-----------------------------------------------------------------------------
int MqttTask(void *arg)
{
    // 6.The three dimensional information is obtained, and only after the three dimensional information is successful, the remaining two tasks will be executed
    if (Mqtt_GetConnectInfo_Task(NULL) == TRUE)
    {
        // Maintain network status
        Mqtt_KeepOnLine_Task(NULL);

        // The mqtt packet is received. Procedure
        Mqtt_Msg_Task(NULL);

        // Subscription timeout processing
        MqttScriTimeoutProcess();
    }
    else
    {
        //TRACE("SubAppMain Get Connect Info error!");
    }

    return RET_OK;
}

short MqttTaskThread(void)
{
    Mqtt_Task_Init();

    CustomServerParamInit();
    MqttServerParamInit();

    while (1)
    {
        if (!MqttGetTaskStatus())
        {
            int s;
            Lib_osiQueueReceive(s_MqttTaskQueue, (void *)&s, -1);
        }

        if (Mqtt_GetStates(MQTT_STATE_ID_SUBSCRIBE))
            Lib_DelayMs(1000);
        else
            Lib_DelayMs(300);

        MqttTask(NULL);
    }
}

//-----------------------------------------------------------------------------
// Name    : UIOnlineMQTT
// Func    : MQTT
// Paras   : iTimeout[in], Timeout value, in milliseconds
// Ret     :
// Desc    :
//-----------------------------------------------------------------------------
short UIOnlineMQTT(int iTimeout)
{
    short sRC;
    char acBuf[256] = { 0 };
    unsigned char aucTimeBcd[6];
    unsigned char aucTimeStr[12];
    unsigned char aucDisBuf[] = "00:00:00";

    iotx_dev_meta_info_t meta;
    //iotx_sign_mqtt_t signout;
    memset(meta.product_key, 0, IOTX_PRODUCT_KEY_LEN + 1);
    memset(meta.device_name, 0, IOTX_DEVICE_NAME_LEN + 1);
    memset(meta.device_secret, 0, IOTX_DEVICE_SECRET_LEN + 1);

    if (ReadFile(PRODUCT_TRIPLE_INFO_FILE, 0, meta.product_key, IOTX_PRODUCT_KEY_LEN) <= 0)
    {
        TRACE_LOG("RET FALSE");
        return RET_ERR;
    }
    TRACE("product_key = %s", meta.product_key);
    if (ReadFile(PRODUCT_TRIPLE_INFO_FILE, 128, meta.device_name, IOTX_DEVICE_NAME_LEN) <= 0)
    {
        TRACE_LOG("RET FALSE");
        return RET_ERR;
    }
    TRACE("device_name = %s", meta.device_name);

    UIClearRect(0, 1, LCD_MAX_SEG, MAX_COM_LINES-1);
    DRAW_TITLE("Mqtt Info");

    while (1)
    {
        memset(acBuf, 0, sizeof(acBuf));
        sprintf((char *)acBuf, "ID = %s", meta.device_name);
        UITextAlign(ALIGN_L, 1, DISP_MODE_NORMAL, (char *)acBuf);

        memset(acBuf, 0, sizeof(acBuf));
        sprintf((char *)acBuf, "Key = %s", meta.product_key);
        UITextAlign(ALIGN_L, 2, DISP_MODE_NORMAL, (char *)acBuf);

        Lib_GetDateTime(aucTimeBcd);
        memset(aucTimeStr, 0x00, sizeof(aucTimeStr));
        Lib_Bcd2Asc(&aucTimeBcd[3], (char*)aucTimeStr, 3);

        aucDisBuf[0] = aucTimeStr[0];
        aucDisBuf[1] = aucTimeStr[1];

        aucDisBuf[3] = aucTimeStr[2];
        aucDisBuf[4] = aucTimeStr[3];

        aucDisBuf[6] = aucTimeStr[4];
        aucDisBuf[7] = aucTimeStr[5];

        memset(acBuf, 0, sizeof(acBuf));
        if (MqttGetTaskStatus())
        {
            if (Mqtt_GetStates(MQTT_STATE_ID_CONNECT) == TRUE)
            {
                sprintf((char *)acBuf, "%s  MqttStates:connect", aucDisBuf);
            }
            else
            {
                sprintf((char *)acBuf, "%s  MqttStates:disconnect", aucDisBuf);
            }
        }
        else
        {
            sprintf((char *)acBuf, "%s  MqttStates:stop", aucDisBuf);
        }
        UITextAlign(ALIGN_L, 3, DISP_MODE_NORMAL, acBuf);

        memset(acBuf, 0, sizeof(acBuf));
        sprintf((char *)acBuf, "%f", g_mqtt_result.money);
        UITextAlign(ALIGN_M, 5, DISP_MODE_NORMAL, acBuf);

        sRC = UIWaitKey(1000, 1);
        if ((UI_INPUT_CANCEL == sRC))
        {
            return sRC;
        }
    }

    return RET_OK;
}

int MqttTaskDeinit(void)
{
    // 暂停任务调度
    MqttSetTaskStatus(FALSE);
    Lib_DelayMs(1);

    // 断开当前MQTT连接
    Mqtt_Disconnect();

    // 参数初始化状态标志重置
    s_mqtt_para.init_flag = FALSE;

    // MQTT工作状态机重置
    step = MQTT_STEP_DETECT_NET;

    // 恢复任务调度
    MqttSetTaskStatus(TRUE);

    return RET_OK;
}

int MqttModifyConnectInfo(E_MQTT_PARAM_ID eMqttParamId, void *pvInfoBuf, unsigned long ulInfoLen)
{
    int iRet = RET_OK;

    if ((pvInfoBuf == NULL) || (ulInfoLen == 0))
    {
        return RET_ERR;
    }

    switch (eMqttParamId)
    {
        case E_MQTT_ID_HOST_NAME_1:
            if ((pvInfoBuf == NULL) || (ulInfoLen == 0) || (ulInfoLen > NETWORK_MQTT_SIZE-1))
            {
                return RET_ERR;
            }
            memset(g_stMqttServerParam.aucMqttHostName1, 0x00, NETWORK_MQTT_SIZE);
            memcpy(g_stMqttServerParam.aucMqttHostName1, pvInfoBuf, ulInfoLen);
            break;

        case E_MQTT_ID_PORT_1:
            if ((pvInfoBuf == NULL) || (ulInfoLen == 0) || (ulInfoLen > NETWORK_PORT_SIZE))
            {
                return RET_ERR;
            }
            memset(g_stMqttServerParam.aucPort1, 0x00, NETWORK_PORT_SIZE+1);
            memcpy(g_stMqttServerParam.aucPort1, pvInfoBuf, ulInfoLen);
            break;

        case E_MQTT_ID_CLIENTID:
            if ((pvInfoBuf == NULL) || (ulInfoLen == 0) || (ulInfoLen > NETWORK_MQTTCLIENTID_SIZE-1))
            {
                return RET_ERR;
            }
            memset(g_stCustomServerParam.aucClientID, 0x00, NETWORK_MQTTCLIENTID_SIZE);
            memcpy(g_stCustomServerParam.aucClientID, pvInfoBuf, ulInfoLen);
            break;

        case E_MQTT_ID_USER_NAME:
            // This parameter is empty
            if ((pvInfoBuf == NULL) && (ulInfoLen == 0))
            {
                memset(g_stCustomServerParam.aucUserName, 0x00, NETWORK_MQTTUSENAME_SIZE);
            }
            else if (ulInfoLen > NETWORK_MQTTUSENAME_SIZE-1)
            {
                return RET_ERR;
            }
            else
            {
                memset(g_stCustomServerParam.aucUserName, 0x00, NETWORK_MQTTUSENAME_SIZE);
                memcpy(g_stCustomServerParam.aucUserName, pvInfoBuf, ulInfoLen);
            }
            break;

        case E_MQTT_ID_USER_PWD:
            // This parameter is empty
            if ((pvInfoBuf == NULL) && (ulInfoLen == 0))
            {
                memset(g_stCustomServerParam.aucPwd, 0x00, NETWORK_MQTTPWD_SIZE);
            }
            else if (ulInfoLen > NETWORK_MQTTPWD_SIZE-1)
            {
                return RET_ERR;
            }
            else
            {
                memset(g_stCustomServerParam.aucPwd, 0x00, NETWORK_MQTTPWD_SIZE);
                memcpy(g_stCustomServerParam.aucPwd, pvInfoBuf, ulInfoLen);
            }
            break;

        case E_MQTT_ID_TOPIC_1:
            if ((pvInfoBuf == NULL) || (ulInfoLen == 0) || (ulInfoLen > NETWORK_MQTTTOPIC_SIZE-1))
            {
                return RET_ERR;
            }
            memset(g_stCustomServerParam.aucTopic1, 0x00, NETWORK_MQTTTOPIC_SIZE);
            memcpy(g_stCustomServerParam.aucTopic1, pvInfoBuf, ulInfoLen);
            break;

        default:
            iRet = RET_ERR;
            break;
    }

    return iRet;
}

short MqttTest(void)
{
    int iRet;

    iRet = MqttModifyConnectInfo(E_MQTT_ID_TOPIC_1, "terminal/123456/inward-alert", strlen("terminal/123456/inward-alert"));
    if (iRet < 0)
    {
        TRACE_LOG("Modify fail");
        return RET_ERR;
    }
    MqttTaskDeinit();

    return RET_OK;
}

#endif

