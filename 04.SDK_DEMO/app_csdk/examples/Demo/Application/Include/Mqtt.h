/**********************************************************************************************************
 * Copyright(C)2021,
 * File Name     : Mqtt.h
 *  Version      :
 *  Author       :
 * Description   : Mqtt function implementation
 **********************************************************************************************************/

#ifndef _MQTT_H
#define _MQTT_H

#include "Drv.h"
#include "Circle_Buffer.h"
#include "ParamManage.h"

#define MQTT_CLIENTID                   "clentid"
#define MQTT_USENAME                    "usename"
#define MQTT_PWD                        "pwd"
#define MQTT_TOPIC                      "totic"
#define MQTT_TOPIC_FORMAT               "%s"
#define MQTT_CLIENTID_FORMAT            "%s"

#define NET_NAME_MQTT                   "mqtt.test.com"
#define NET_NAME_MQTT_PORT              "1883"

//========================== mqtt======================================
#define NETWORK_MQTT_SIZE               128
#define NETWORK_MQTTCLIENTID_SIZE       128
#define NETWORK_MQTTUSENAME_SIZE        128
#define NETWORK_MQTTPWD_SIZE            256
#define NETWORK_MQTTTOPIC_SIZE          128

#define MQTT_KEEP_ONLINE_OUTTIME        (35 * 1000)

typedef struct
{
    unsigned char aucClientID[NETWORK_MQTTCLIENTID_SIZE];   //devicename
    unsigned char aucUserName[NETWORK_MQTTUSENAME_SIZE];    //username
    unsigned char aucPwd[NETWORK_MQTTPWD_SIZE];             //password
    unsigned char aucTopic1[NETWORK_MQTTTOPIC_SIZE];        //topic1
    unsigned char aucTopic2[NETWORK_MQTTTOPIC_SIZE];        //topic2
    unsigned char reserved[1024];                           //Reserved
}ST_CUSTOMSERVER_PARAM;

typedef struct
{
    unsigned char aucMqttHostName1[NETWORK_MQTT_SIZE];  //Host 1
    unsigned char aucPort1[NETWORK_PORT_SIZE+1];        //Host 1 port, save '\0'
    unsigned char aucMqttHostName2[NETWORK_MQTT_SIZE];  //Host 2
    unsigned char aucPort2[NETWORK_PORT_SIZE+1];        //Host 2 port, save '\0'
    unsigned char reserved[1024];                       //Reserved
}ST_MQTTSERVER_PARAM;

//mqtt Instruction execution states
typedef enum _MQTT_StateID_t
{
    MQTT_STATE_ID_DISCONNECT = 0,
    MQTT_STATE_ID_CONNECT,
    MQTT_STATE_ID_SUBSCRIBE,
    MQTT_STATE_ID_PUB,
    MQTT_STATE_ID_PING,
    MQTT_STATE_ID_UNSUB,

    MQTT_STATE_ID_END

} MQTT_STATE_ID;

typedef enum tagMqttParamId
{
    E_MQTT_ID_HOST_NAME_1,
    E_MQTT_ID_PORT_1,
    E_MQTT_ID_HOST_NAME_2,
    E_MQTT_ID_PORT_2,

    E_MQTT_ID_CLIENTID,
    E_MQTT_ID_USER_NAME,
    E_MQTT_ID_USER_PWD,
    E_MQTT_ID_TOPIC_1,
    E_MQTT_ID_TOPIC_2,

}E_MQTT_PARAM_ID;

typedef int (* Mqtt_Publish_CallBack)(char *buff, int len);

typedef enum
{
    IOTX_CLOUD_REGION_SHANGHAI,   /* Shanghai */
    IOTX_CLOUD_REGION_SINGAPORE,  /* Singapore */
    IOTX_CLOUD_REGION_JAPAN,      /* Japan */
    IOTX_CLOUD_REGION_USA_WEST,   /* America */
    IOTX_CLOUD_REGION_GERMANY,    /* Germany */
    IOTX_CLOUD_REGION_CUSTOM,     /* Custom setting */
    IOTX_CLOUD_DOMAIN_MAX         /* Maximum number of domain */

} iotx_mqtt_region_types_t;

#define IOTX_PRODUCT_KEY_LEN            (20)
#define IOTX_DEVICE_NAME_LEN            (32)
#define IOTX_DEVICE_SECRET_LEN          (64)
#define IOTX_DEVICE_ID_LEN              (64)
#define IOTX_PRODUCT_SECRET_LEN         (64)

typedef struct _iotx_dev_meta_info
{
    char product_key[IOTX_PRODUCT_KEY_LEN + 1];
    char product_secret[IOTX_PRODUCT_SECRET_LEN + 1];
    char device_name[IOTX_DEVICE_NAME_LEN + 1];
    char device_secret[IOTX_DEVICE_SECRET_LEN + 1];

} iotx_dev_meta_info_t;
#define DEV_SIGN_SOURCE_MAXLEN          (200)
#define DEV_SIGN_HOSTNAME_MAXLEN        (64)
#define DEV_SIGN_CLIENT_ID_MAXLEN       (200)
#define DEV_SIGN_USERNAME_MAXLEN        (64)
#define DEV_SIGN_PASSWORD_MAXLEN        (65)

typedef struct
{
    char hostname[DEV_SIGN_HOSTNAME_MAXLEN];
    unsigned short port;
    char clientid[DEV_SIGN_CLIENT_ID_MAXLEN];
    char username[DEV_SIGN_USERNAME_MAXLEN];
    char password[DEV_SIGN_PASSWORD_MAXLEN];

} iotx_sign_mqtt_t;

//0: The voice network is connected only once. If the voice network is disconnected, the voice network will not be connected again. 1: If the voice network is disconnected, the voice network will be connected again
#define AUDIO_NET_CONNECT_REPLAY_EN                         1
#define MQTT_DISSUB_AUDIO_PLAY_TIMES_MAX                    3                       // mqtt subscribs to the maximum number of disconnected voice broadcasts
#define MQTT_MSG_BUFF_MAX                                   128                     //Define the field length according to the tripartite requirements

#if OTHER_TOPIC_EN
#define   OTHER_TOPIC_NUM                                    2

#if (OTHER_TOPIC_NUM == 0)
#error "<<OTHER_TOPIC_NUM can not equ zero!>>"
#endif
#endif

// ---------------------------------------------------------------
typedef enum
{
    MQTT_MSG_TYPE_INVALID = 0,          // Invalid message
    MQTT_MSG_TYPE_PREVIEW,              // Advance report
    MQTT_MSG_TYPE_ACCOUNT,              // arrive
    MQTT_MSG_TYPE_TIMING,               // Timed push message

    MQTT_MSG_TYPE_END = 255

} Mqtt_Msg_t;

struct _Mqtt_Result_t
{
    double          money;              // Amount received
}_Mqtt_Result_t;
typedef struct _Mqtt_Result_t       Mqtt_Result_t;

struct _Mqtt_Para_t
{
    char server[128];
    unsigned short port;
    char client_id[128];
    char user[128];
    char password[256];
    char topic[128];
    int ping_time;
    unsigned char clean_session;          // Clear the session, =1, clear the message from the server, = 0, continue to receive the unsent message from the server
    unsigned char init_flag;                 // FALSE, not initialized, TRUE, initialized
};
typedef struct _Mqtt_Para_t Mqtt_Para_t;

struct _Mqtt_Op_t
{
    int (* Connect_Info)(Mqtt_Para_t *para);                            // connect Parameter information, mainly triplet information
    int (* Msg_Process)(CBuff_t *msg_buff, Mqtt_Result_t *result);      // The received server packet is parsed
    int (* Account_AudioPlay)(unsigned char *audio, int audio_len, Mqtt_Result_t *result);        // Play the amount required

    void (* PowerOff_SaveRecord)(void);         // Keep transaction record
    void (* PowerOn_ReadRecord)(void);          // Fetch transaction record

};
typedef struct _Mqtt_Op_t Mqtt_Op_t;

/**
 * @brief MQTT Quality of Service values.
 */
typedef enum MQTTQoS
{
    MQTTQoS0 = 0, /**< Delivery at most once. */
    MQTTQoS1 = 1, /**< Delivery at least once. */
    MQTTQoS2 = 2  /**< Delivery exactly once. */
}E_MQTTQoS;

/**
 * @brief MQTT Server info and CONNECT packet parameters.
 */
typedef struct tagCert
{
    char           *m_pcCaCert;     // CA
    unsigned long   m_ulCaCertLen;  // Length of CA
    char           *m_pcCliCert;    // Client certificate
    unsigned long   m_ulCliCertLen; // Length of Client certificate
    char           *m_pcCliKey;     // Client key
    unsigned long   m_ulCliKeyLen;  // Length of Client key
    int             m_iSecLevel;
}ST_TLSSOCK_CERT;

/**
 * @brief MQTT Server info and CONNECT packet parameters.
 */
typedef struct tagMqttConnectInfo
{
    const char     *m_pcServer;             // MQTT server host name.
    unsigned short  m_usPort;               // MQTT server port number.
    const char     *m_pcClientId;           // MQTT client identifier. Must be unique per client.
    const char     *m_pcUserName;           // MQTT user name. Set to NULL if not used.
    const char     *m_pcPassword;           // MQTT password. Set to NULL if not used.
    unsigned short  m_usKeepAliveSeconds;   // MQTT keep alive period.
    unsigned char   m_bCleanSession;        // Whether to establish a new, clean session or resume a previous session.
}ST_MQTT_CONNECT_INFO;

/**
 * @brief MQTT SUBSCRIBE packet parameters.
 */
typedef struct tagMqttSubscribeInfo
{
    E_MQTTQoS       m_eQos;                 // Quality of Service for subscription.
    const char     *m_pcTopicFilter;        // Topic filter to subscribe to.
    unsigned short  m_usTopicFilterLen;     // Length of subscription topic filter.
}ST_MQTT_SUB_INFO;

// SDK API: Support MQTTS
extern int Lib_MqttConnect(NETWORK_TYPE eType, ST_TLSSOCK_CERT *pstCert, ST_MQTT_CONNECT_INFO *pstMqttConnectInfo);
extern int Lib_MqttDisconnect(int fd);
extern int Lib_MqttSubscribe(int fd, ST_MQTT_SUB_INFO *pstSubscription);
extern int Lib_MqttSubscriptionList(int fd, ST_MQTT_SUB_INFO *subscription, unsigned long listCount);
extern int Lib_MqttUnsubscribe(int fd, ST_MQTT_SUB_INFO *subscription);
extern int Lib_MqttUnsubscriptionList(int fd, ST_MQTT_SUB_INFO *subscription, unsigned long listCount);
extern int Lib_MqttRecv(int fd, unsigned char *pucBuf, unsigned short *pusLen);
extern int Lib_MqttGetStates(int fd, MQTT_STATE_ID eId);

extern short MqttTaskThread(void);

#endif
