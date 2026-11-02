
#ifndef __TRANS_CBK_H__
#define __TRANS_CBK_H__

typedef enum _pos_tag_t
{
    POS_TAG_START = 0,

    POS_TAG_HEAD = 0x7F00,
    APP_TAG_HEAD,
    POS_TAG_RES_EN_ONLINE_DATA,
    POS_TAG_RES_EN_ONLINE_RESULT,
    POS_TAG_RES_EN_BATCH_DATA,
    POS_TAG_RES_KN_ONLINE_DATA,
    POS_TAG_RES_KN_APP_DATA,

    APP_TAG_START = 0x9F8301,
	POS_TAG_TRANSACTION_CONTACTLESS_MODE,

    KERNEL_TAG_AID_LIST_TERMINAL                  = 0x9F8110,
    KERNEL_TAG_AID_LIST_CANDIDATE                 = 0x9F8111,
    KERNEL_TAG_AID_LIST_CANDIDATE_COUNTER         = 0x9F8112,
    KERNEL_TAG_AID_SELECT_RESULT_INDEX            = 0x9F8113,
    KERNEL_TAG_AID_LIST_DISPLAYNAME               = 0x9F8114,
    KERNEL_TAG_AID_SELECT_TRYAGAIN                = 0x9F8115,
	TAG_PBOC_ARITHMETIC_SUPPORT_SM                =  0xDF69,            //�����㷨֧��
	TAG_EMV_TRANS_REVERSAL                        =  0x9F8104,

	POS_TAG_END,
}Pos_Tag_t;


/*输出的AID侯选列表结构体*/
typedef struct _AidCandidate_t
{
	unsigned char _aid[16];		/*	AID	应用标识符*/
	unsigned char _aid_len;		/*	length of AID*/
	unsigned char _lable[20];           /* 应用标签
								50(ICC), ans, 1-16 bytes, we prepare 20 for some PBOC2 errors*/
	unsigned char _lable_len;		/*	length of lable*/
	unsigned char _preferred_name[20];  /* 应用优先名9F12(ICC), ans, 1-16 bytes */
	unsigned char _preferred_name_len;	/*	length of preferred name*/
	unsigned char _priority;            /* 应用优先权标识符87(ICC), b, 1 bytes */
	unsigned char  _enable;              /* indicate whether the candidate is enabled 
									0	去除
									1	可用*/
	unsigned char  _table_index;    /*  issuer code table index 9F11(ICC), n1, 1 byte */
	unsigned char _resv[2];		/*	reserve bytes	*/
	int   _file_offset;         		/* the offset of this AID	in the parameters file */
}AidCandidate_t;

typedef  struct  _EmvOnlineData_t
{
	char iccResponse[3];            /* 39域 */
	unsigned char ackdata[1024];       /* 55域 */
	int ackdatalen;      				/* 55域长度 */
}EmvOnlineData_t;

typedef enum
{
	EMV_DISP_READING_CARD= 0,
	EMV_DISP_SEE_PHONE ,
	EMV_DISP_NFC_RETAP,
	EMV_DISP_REMOVE_CARD,
	EMV_DISP_OFFLINE_PIN_BLOCKED,

}EmvKernelDisp;

typedef enum
{
    EMV_OFFLINEPIN_INPUT = 1,   /*Offline password*/
    EMV_LAST_OFFLINEPIN_INPUT = 2,   /*Last offline password*/
    EMV_ONLINEPIN_INPUT = 3,   /*Online password*/

}EMV_INPUT_PIN_TYPE;

typedef struct _EmvCallBack_t
{	
	//多应用选择 返回值>=0 返回选择的AID序列号 -1失败
	int (*EMV_AidSelect)(AidCandidate_t *pList, int listNum);
	//设置kernel id  return 0成功 非0 失败
	int (*EMV_SetKernelId)(unsigned char *aid,int aidLen,int *kernelId);
	//卡号确认 返回值0确认 -1失败
	int (*EMV_ConfirmCardInfo)(char *pan,int len);
	//请求pin cType：1脱机密码 2最后一次脱机密码 3联机密码  返回值-1输入失败  -2未输入BYPASS  -3中止交易和Timeout  >0输入密码长度
	int (*EMV_InputPasswd)(EMV_INPUT_PIN_TYPE cType,char *pan, char *pszPin);
	//身份确认 type 取值0-5,分别为:身份证/军官证/护照/入境证/临时身份证/其他 pcon：证件号；返回值1	执卡人身份确认  0身份确认失败
	int (*EMV_CertConfirm)(unsigned char type, unsigned char *pcon, unsigned char len);
	//联机  result:联机结果-1 联机失败 >=0   联机成功(有报文应答)
	int (*EMV_OnlineProcess)(EmvOnlineData_t* pOnlineData);
	//内核要求显示的信息
	int (*EMV_ProcessDisp)(EmvKernelDisp cType);  ///0  成功  1 内核退出
	void (*EMV_AfterSelectApp)();
	void (*EMV_AfterReadRecord)();
}EmvCallBack_t;

typedef	enum
{
	OFFLINEAPPROVED = 0,
	OFFLINEDECLINED,
	ONLINEAPPROVED,
	ONLINEDECLINED,
	UNABLEONLINE_OFFLINEAPPROVED,
	UNABLEONINE_OFFLINEDECLINED
}EMVTRANSRESULT;

typedef	enum
{
	PROCESS_STATUS_INIT = 0,
	KERNEL_PROCESS_STATUS_NFC_RETAP_SUCCESS,
	KERNEL_PROCESS_STATUS_NFC_RETAP_CANCLE,
	KERNEL_PROCESS_STATUS_NFC_RETAP_TIMEOUT,

}EMV_KERNEL_PROCESS_STATUS;

typedef struct{u8 *head; u32 len;} T_U8_VIEW;
#define UV_OK(uv)	((uv).head && (uv).len)

//内核交易类型
typedef enum
{
	EMV_L2_SALE 		= 0x00,  
	EMV_CASH			= 0x01,
	EMV_CASHBACK		= 0x09,
	EMV_INQUIRY			= 0x31,
	EMV_TRANFER			= 0x40,
	EMV_ADMIN			= 0x66,
	EMV_CASHDEPOSIT		= 0x21,
	EMV_PAYMENT			= 0x50,
	EMV_REFUND			= 0x20,
}EMVTransType;

//emv交易返回值
typedef enum
{
	APP_RC_START = -1,
	APP_RC_COMPLETED = 0,
	APP_RC_TERMINAL,
	APP_RC_CANCEL,
	APP_RC_EMV_DENAIL,
	APP_RC_EMV_GAC2_DENAIL,
	APP_RC_NFC_NOT_ALLOW,//不允许交易
	APP_RC_EMV_APP_BLOCK,//不允许交易
	APP_RC_EMV_APP_SEE_PHONE,
	APP_RC_EMV_TRANS_TRY_ANOTHER_INTERFACE,
	APP_RC_EMV_TRANS_GPO_NOT_SUPPORT,
	APP_RC_FALL_BACK,
	APP_RC_EMV_CARD_BLOCK,
	APP_RC_CARD_NOT_SUPPORT,

	APP_RC_NFC_RETAP_TIMEOUT,
	APP_RC_NFC_RETAP_CANCEL,
	APP_RC_NFC_TERMINAL,
	APP_RC_NFC_DOUBLETAP_DENAIL,
	APP_RC_NFC_MULTI_CARD,
	APP_RC_NFC_TRY_AGAIN,

	APP_RC_TRANS_REVERSEL,

	APP_RC_NUMS,
}EMV_L2_Return;

int app_user_cbk(void *user_data, unsigned char *buf, unsigned int len);
int Emv_SetCoreData(unsigned int tagname, unsigned char *pvalue, int valuelen);
unsigned char* Emv_GetCoreData(unsigned int tagname, int *pvallen);
int Emv_FetchData(unsigned int* tagname, int count, unsigned char* obuf, int olen);
int Emv_KernelInit(EmvCallBack_t t_callbackfun);
EMV_L2_Return Emv_Process(ST_TRANSDATA *emvTransParams);
int Emv_CheckContactlessDoubleTap(EmvOnlineData_t* pOnlineData);
int Emv_SetContactlessOnlineResult(EmvOnlineData_t* pOnlineData);

#endif