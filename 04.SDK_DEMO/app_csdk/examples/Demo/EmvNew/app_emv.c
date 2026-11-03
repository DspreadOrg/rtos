
#include "EmvDemo.h"
#include "key.h"
#include "Ui.h"
#include "Common.h"
#include <math.h>
#include <string.h>
#include "card.h"
#include "VAN_security.h"
#include "MessageBox.h"
#include "Idle.h"
#include "Trade.h"
#include "LanguageEN.h"
#include "Communication.h"
#include "CardTrade.h"
#include "api_lvgl.h"
#include "Card.h"

#include "EmvUtils.h"
#include "app_emv.h"

#define FILE_NAME_EMV_APP "emv_app.bin"
#define FILE_NAME_EMV_CAPK "emv_capk.bin"

#define EMV_APP_PAGE_SIZE (384)
#define EMV_CAPK_PAGE_SIZE (384)

static pu8 pTransactionPool = NULL;
static short s_emv_card_mode = -1;
static int trans_gac_time = 0;
static unsigned long EMV_L2_LastError = 0;
static bool glb_online_trans_result = false;
static bool emv_last_offline_pin_flag = false;
static EMV_KERNEL_PROCESS_STATUS glb_emv_kernel_process_status = PROCESS_STATUS_INIT;

int (*Trade_EMV_Dispmsg)(EmvKernelDisp type)         = NULL;
int (*Trade_EMV_AppSelect)(AidCandidate_t *pList, int listNum)       = NULL;
int (*Trade_SetKernelId)(unsigned char *aid,int aidLen,int *kernelId) = NULL;
int (*Trade_ConfirmCardInfo)(char *pan,int len)     = NULL;
int (*Trade_OnlineProcess)(EmvOnlineData_t* pOnlineData)   = NULL;
int (*Trade_PinEnter)(EMV_INPUT_PIN_TYPE pinType,char *pan,char *pinBlock) = NULL;
void (*Trade_AfterSelectApp)() = NULL;
void (*Trade_AfterReadRecord)() = NULL;

void tlv_batch_replace_(pu8 pTlvData, pu8 pTlvDstData)
{
    pu8 pChild = NULL;

    pChild = kernel_tlv_get_first_child(pTlvData);
    while(pChild)
    {
        kernel_tlv_replace(pTlvDstData,kernel_tlv_get_t(pChild),kernel_tlv_get_l(pChild),kernel_tlv_get_v(pChild));
        pChild = kernel_tlv_get_next_child(pChild);
    }
}

/*--------------------------------------
|   Function Name:
|       get_tlv_view
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
T_U8_VIEW get_tlv_view(u8 *pTlvBuf, u32 dwTag)
{
	u8 *pNode = kernel_tlv_find(pTlvBuf, dwTag);
	T_U8_VIEW uvResult = {NULL, 0};
	if(pNode)
	{
		uvResult.head = kernel_tlv_get_v(pNode);
		uvResult.len = kernel_tlv_get_l(pNode);
	}
	return uvResult;
}
/*--------------------------------------
|   Function Name:
|       set_tlv_view
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
void set_tlv_view(u8 *pTlvBuf, u32 dwTag, T_U8_VIEW uvVal)
{
	if(uvVal.head && uvVal.len)
	{
		if(kernel_tlv_find(pTlvBuf, dwTag))
			kernel_tlv_modify(pTlvBuf, dwTag, uvVal.len, uvVal.head);
		else
			kernel_tlv_add_child(pTlvBuf, dwTag, uvVal.len, uvVal.head);
	}
}

void transPoolSetTag(int tag,pu8 pValue,int Len)
{
    if(pTransactionPool != NULL)
        kernel_tlv_replace(pTransactionPool,tag,Len,pValue);
}

#if 0
static int _asc_2_bcd(unsigned char * ascstr, int asclen, unsigned char * bcdstr, int align)
{
	int i = 0;
	unsigned char ch;

	for (i = 0; i < (asclen + 1) / 2; i++)
		*(bcdstr + i) = 0x00;

	if ((asclen & 0x01) && align)
		align = 1;
	else
		align = 0;

	for (i = align; i < asclen + align; i ++) {
		ch = *(ascstr + i - align);
		if (ch >= 'a'&& ch <= 'f')
			ch -= ('a' - 0x0A);
		else if (ch >= 'A'&& ch <= 'F')
			ch -= ('A' - 0x0A);
		else if (ch >= '0' && ch <= '9')
			ch -= '0';
		else
			return -1;
		*(bcdstr + (i / 2)) |= (ch << (((i + 1) % 2) * 4));
	}

	return 0;
}
#endif

static u32 asc_to_bcd(pu8 src,u32 len,pu8 dest )
{
	u32 i;

	if(len%2){
		return 0;
	}
	else{}
	for(i=0;i<len/2;i++){
		if(src[i*2] >= '0' && src[i*2] <= '9'){
			dest[i] = src[i*2] - '0';
		}
		else if(src[i*2] >= 'A' && src[i*2] <= 'F'){
			dest[i] = src[i*2] - 'A' + 0x0A;
		}
		else if(src[i*2] >= 'a' && src[i*2] <= 'f'){
			dest[i] = src[i*2] - 'a' + 0x0A;
		}
		else{
			return 0;
		}
		dest[i] *= 16;
		if(src[i*2 + 1] >= '0' && src[i*2 + 1] <= '9'){
			dest[i] += src[i*2 + 1] - '0';
		}
		else if(src[i*2 + 1] >= 'A' && src[i*2 + 1] <= 'F'){
			dest[i] += src[i*2 + 1] - 'A' + 0x0A;
		}
		else if(src[i*2 + 1] >= 'a' && src[i*2 + 1] <= 'f'){
			dest[i] += src[i*2 + 1] - 'a' + 0x0A;
		}
		else{
			return 0;
		}
	}
	return i;
}

static u32 bcd_to_asc_sample(pu8 src,u32 len,pu8 dest )
{
	u32 i;

	for(i=0;i<len;i++){
		*(dest + 2*i) = ((*(src + i) & 0xF0) >> 4) + 0x30;
		*(dest + 2*i + 1) = (*(src + i) & 0x0F) + 0x30;
	}
	return len*2;
}

static u32 asc_to_bcd_sample(pu8 src,u32 len,pu8 dest )
{
	u32     i;
    u8      high;
    u8      low;

    high = 0;
    low = 0;
    for(i = 0; i < len; i++)
    {
        high = *(src + i) - 0x30;
        i++;
        if(i < len)
        {
	        low = *(src + i) - 0x30;
	    }
	    else
	    {
	        i--;                                                
	        low = 0x0f ;                                     
	    }
        *(dest + i/2) = (high << 4) | low;                    
    }
    return (len+1)/2;
}
#if 0
static u32 format_amt_ascn_to_bcd6(pu8 amt,u32 amtlen,pu8 amtOut )
{
	u8   amountasc[12];
    u32  var_i = 0;

	if(amtlen <=0){
		return 0;
	}
	else{}
    for(var_i = 0; var_i< amtlen;var_i++)
    {
      if(amt[var_i]>'9' || amt[var_i]<'0')
        break;
    }

    if(var_i > 12){
		return 0;
	}
	else{}
	memset(amountasc,'0',12);
	memcpy(amountasc + 12 - var_i,amt,var_i);
	asc_to_bcd_sample(amountasc,12,amtOut);
	return 6;
}
#endif
static int trans_cbk_fs_get_ics(unsigned char *cbk_buf, unsigned int cbk_buf_len)
{
    FUN_IN;

    int fd;
    unsigned char filebuf[512];
    unsigned int filebuf_len = 0;
    unsigned char temp_buf[1024];
    unsigned char *pchild = 0;

    fd = Lib_FileOpen(FILE_NAME_EMV_APP, LFS_O_RDONLY);
    if (fd < 0)
    {
        TRACE("Emv ics, fopen fail");
        return -1;
    }

    memset(filebuf, 0, sizeof(filebuf));
    filebuf_len = Lib_FileRead(fd, filebuf, EMV_APP_PAGE_SIZE);
    Lib_FileClose(fd);

    cbk_buf_len = kernel_tlv_new(cbk_buf, 0x7F00, 0, 0);

    kernel_tlv_new(temp_buf, 0x7F00, filebuf_len, filebuf);
    pchild = kernel_tlv_get_first_child(temp_buf);
    while (pchild)
    {
        unsigned int tag = 0;
        unsigned int len = 0;
        unsigned char *value = 0;

        tag = kernel_tlv_get_t(pchild);
        if (tag == 0)
            break;

        len = kernel_tlv_get_l(pchild);
        value = kernel_tlv_get_v(pchild);

        cbk_buf_len = kernel_tlv_add_child(cbk_buf, tag, len, value);

        pchild = kernel_tlv_get_next_child(pchild);
    }

    TRACE("Emv ics (%d)", cbk_buf_len);
    TRACEBUF(cbk_buf, cbk_buf_len);

    return 0;
}

static int trans_cbk_fs_get_aid_list(unsigned char *cbk_buf, unsigned int cbk_buf_len)
{
    FUN_IN;

    int fd;
    unsigned char tempBuf[512] = {0};
    unsigned char aidItem[16] = {0};
    unsigned int aidItemLen = 0;

    fd = Lib_FileOpen(FILE_NAME_EMV_APP, LFS_O_RDONLY);
    if (fd < 0)
    {
        TRACE("Emv aid list, fopen fail");
        return -1;
    }

    cbk_buf_len = kernel_tlv_new(cbk_buf, 0x7F00, 0, 0);

    Lib_FileSeek(fd, EMV_APP_PAGE_SIZE, SEEK_SET); // 跳过ICS

    while (Lib_FileRead(fd, tempBuf, EMV_APP_PAGE_SIZE) == EMV_APP_PAGE_SIZE)
    {
        FUN_IN;
        unsigned char *pNode = 0;
        unsigned char *pValue = 0;
        unsigned int valueLen = 0;

        // memset(tempBuf, 0, sizeof(tempBuf));
        memset(aidItem, 0, sizeof(aidItem));
        aidItemLen = 0;

        // fread(tempBuf, sizeof(char), EMV_APP_PAGE_SIZE, fp);

        // AID
        pNode = kernel_tlv_find(tempBuf, 0x9F06);
        if (pNode == 0)
        {
            continue;
        }
        pValue = kernel_tlv_get_v(pNode);
        valueLen = kernel_tlv_get_l(pNode);
        memcpy(aidItem + 2, pValue, valueLen);
        aidItemLen += valueLen;

        // kernel id
        if (memcmp(pValue, "\xA0\x00\x00\x00\x03", 5) == 0)
            aidItem[1] = KERNEL_ID_VISA;
        else if (memcmp(pValue, "\xA0\x00\x00\x00\x04", 5) == 0)
            aidItem[1] = KERNEL_ID_MASTERCARD;
        else if (memcmp(pValue, "\xA0\x00\x00\x02\x28", 5) == 0)
            aidItem[1] = KERNEL_ID_MASTERCARD;
        else if (memcmp(pValue, "\xA0\x00\x00\x00\x65", 5) == 0)
            aidItem[1] = KERNEL_ID_JCB;
        else if (memcmp(pValue, "\xA0\x00\x00\x00\x25", 5) == 0)
            aidItem[1] = KERNEL_ID_AMEX;
        else if (memcmp(pValue, "\xA0\x00\x00\x00\x52", 5) == 0)
            aidItem[1] = KERNEL_ID_D_PAS;
        else if (memcmp(pValue, "\xA0\x00\x00\x00\x24", 5) == 0)
            aidItem[1] = KERNEL_ID_D_PAS;
        else if (memcmp(pValue, "\xA0\x00\x00\x04\x94", 5) == 0)
            aidItem[1] = KERNEL_ID_D_PAS;
        else if (memcmp(pValue, "\xA0\x00\x00\x03\x33", 5) == 0)
            aidItem[1] = KERNEL_ID_CUP;
        else if (memcmp(pValue, "\xA0\x00\x00\x05\x24", 5) == 0)
            aidItem[1] = KERNEL_ID_RUPAY;
        else if (memcmp(pValue, "\xA0\x00\x00\x02\x77", 5) == 0)
            aidItem[1] = KERNEL_ID_INTERAC;
        else if (memcmp(pValue, "\xD9\x99\x99\x99\x99", 5) == 0)
            aidItem[1] = KERNEL_ID_PURE;
        else if (memcmp(pValue, "\xAA\xA0\x00\x00\x01", 5) == 0)
            aidItem[1] = KERNEL_ID_PURE;
        else if (memcmp(pValue, "\xA0\x00\x00\x06\x58", 5) == 0)
            aidItem[1] = KERNEL_ID_MIRPS;
        else if (memcmp(pValue, "\xA0\x00\x00\x01\x41", 5) == 0)
            aidItem[1] = KERNEL_ID_BANCOMAT;

        // ASI
        pNode = kernel_tlv_find(tempBuf, 0xDF01);
        if (pNode)
        {
            pValue = kernel_tlv_get_v(pNode);
            aidItem[0] = pValue[0];
        }
        else
        {
            aidItem[0] = 0;
        }

        aidItemLen += 2;

        cbk_buf_len = kernel_tlv_add_child(cbk_buf, 0x9F06, aidItemLen, aidItem);

        memset(tempBuf, 0, sizeof(tempBuf));
    }

    Lib_FileClose(fd);

    TRACE("Emv aid list (%d)", cbk_buf_len);
    TRACEBUF(cbk_buf, cbk_buf_len);

    return 0;
}

static int trans_cbk_fs_get_bctc_aid_list(unsigned char *cbk_buf,
                                   unsigned int cbk_buf_len)
{
    FUN_IN;
    int fd;
    unsigned char tempBuf[512] = {0};
    unsigned char aidItem[16] = {0};
    unsigned int aidItemLen = 0;

    fd = Lib_FileOpen(FILE_NAME_EMV_APP, LFS_O_RDONLY);
    if (fd < 0)
    {
        TRACE("Emv ic aid list, fopen fail");
        return -1;
    }

    cbk_buf_len = kernel_tlv_new(cbk_buf, 0x7F00, 0, 0);

    Lib_FileSeek(fd, EMV_APP_PAGE_SIZE, SEEK_SET); // 跳过ICS

    while (Lib_FileRead(fd, tempBuf, EMV_APP_PAGE_SIZE) == EMV_APP_PAGE_SIZE)
    {
        unsigned char *pNode = 0;
        unsigned char *pValue = 0;
        unsigned int valueLen = 0;

        // memset(tempBuf, 0, sizeof(tempBuf));
        memset(aidItem, 0, sizeof(aidItem));
        aidItemLen = 0;

        // AID
        pNode = kernel_tlv_find(tempBuf, 0x9F06);
        if (pNode == 0)
        {
            continue;
        }
        pValue = kernel_tlv_get_v(pNode);
        valueLen = kernel_tlv_get_l(pNode);
        memcpy(aidItem + 1, pValue, valueLen);
        aidItemLen += valueLen;

        // Length + ASI
        aidItem[0] = valueLen;
        pNode = kernel_tlv_find(tempBuf, 0xDF01);
        if (pNode)
        {
            if (kernel_tlv_get_v(pNode)[0] == 0)
            {
                aidItem[0] |= 0x80;
            }
        }

        aidItemLen += 1;

        cbk_buf_len = kernel_tlv_add_child(cbk_buf, 0x9F8724, aidItemLen, aidItem);
    }

    Lib_FileClose(fd);

    TRACE("Emv ic aid list (%d)", cbk_buf_len);
    TRACEBUF(cbk_buf, cbk_buf_len);

    return 0;
}

static int trans_cbk_fs_get_aid_param(unsigned char *cbk_buf, unsigned int cbk_buf_len)
{
    int fd;
    unsigned char tempBuf[512] = {0};
    unsigned char flag = 0;
    unsigned char tempTlvBuf[1024] = {0};
    unsigned char *pChild = 0;

    unsigned char *pNode = 0;
    unsigned char aid[16] = {0};
    unsigned int aidLen = 0;
    unsigned char trans_type = 0;

    pNode = kernel_tlv_find(cbk_buf, 0x9F06);
    if (pNode == 0)
    {
        return -1;
    }
    memset(aid, 0, sizeof(aid));
    aidLen = kernel_tlv_get_l(pNode);
    memcpy(aid, kernel_tlv_get_v(pNode), aidLen);

    transPoolSetTag(0x9F06,aid,aidLen);

    pNode = kernel_tlv_find(cbk_buf, 0x9C);
    if (pNode != 0)
    {
        trans_type = kernel_tlv_get_v(pNode)[0];
    }

    fd = Lib_FileOpen(FILE_NAME_EMV_APP, LFS_O_RDONLY);
    if (fd < 0)
    {
        TRACE("EMV aid param, fopen fail");
        return -1;
    }

    Lib_FileSeek(fd, EMV_APP_PAGE_SIZE, SEEK_SET); // 跳过ICS

    while (Lib_FileRead(fd, tempBuf, EMV_APP_PAGE_SIZE) == EMV_APP_PAGE_SIZE)
    {
        unsigned char *pNode = 0;
        unsigned char *pValue = 0;
        unsigned int valueLen = 0;

        pNode = kernel_tlv_find(tempBuf, 0x9F06);
        if (pNode == 0)
        {
            continue;
        }
        pValue = kernel_tlv_get_v(pNode);
        valueLen = kernel_tlv_get_l(pNode);

        if (memcmp(aid, pValue, valueLen) != 0)
        {
            memset(tempBuf, 0, sizeof(tempBuf));
            continue;
        }

        flag = 1;
        break;
    }

    Lib_FileClose(fd);

    if (flag == 0)
    {
        TRACE("EMV aid param, not find");
        return -1;
    }

    cbk_buf_len = kernel_tlv_new(cbk_buf, 0x7F00, 0, 0);

    kernel_tlv_new(tempTlvBuf, 0x7F00, EMV_APP_PAGE_SIZE, tempBuf);
    pChild = kernel_tlv_get_first_child(tempTlvBuf);
    while (pChild)
    {
        unsigned int tag = 0;
        unsigned int len = 0;
        unsigned char *value = 0;

        tag = kernel_tlv_get_t(pChild);
        if (tag == 0)
            break;

        len = kernel_tlv_get_l(pChild);
        value = kernel_tlv_get_v(pChild);

        if (tag == 0x7F11)
        {
            unsigned char *child_7F11 = 0;

            pNode = kernel_tlv_find(value, 0x9C);
            if (pNode == 0)
            {
                pChild = kernel_tlv_get_next_child(pChild);
                continue;
            }

            if (kernel_tlv_get_v(pNode)[0] != trans_type)
            {
                pChild = kernel_tlv_get_next_child(pChild);
                continue;
            }

            child_7F11 = kernel_tlv_get_first_child(pChild);
            while (child_7F11)
            {
                unsigned int child_tag = 0;
                unsigned int child_len = 0;
                unsigned char *child_val = 0;

                child_tag = kernel_tlv_get_t(child_7F11);
                child_len = kernel_tlv_get_l(child_7F11);
                child_val = kernel_tlv_get_v(child_7F11);

                cbk_buf_len = kernel_tlv_add_child(cbk_buf, child_tag, child_len, child_val);
                child_7F11 = kernel_tlv_get_next_child(child_7F11);
            }

            pChild = kernel_tlv_get_next_child(pChild);
            continue;
        }

        cbk_buf_len = kernel_tlv_add_child(cbk_buf, tag, len, value);

        pChild = kernel_tlv_get_next_child(pChild);
    }

    TRACE("EMV aid param (%d)", cbk_buf_len);
    TRACEBUF(cbk_buf, cbk_buf_len);

    return 0;
}

static int trans_cbk_fs_get_capk(unsigned char *cbk_buf, unsigned int cbk_buf_len)
{
    unsigned char *node = 0;
    unsigned char aid[16] = {0};
    unsigned int aidLen = 0;
    unsigned char index = 0;
    int fd;
    unsigned char tempBuf[512] = {0};
    unsigned char tlvPool[512] = {0};
    unsigned char flag = 0;

    TRACE("EMV get capk");
    TRACEBUF(cbk_buf, cbk_buf_len);

    node = kernel_tlv_find(cbk_buf, 0x9F06);
    if (node == 0)
    {
        return -1;
    }
    memset(aid, 0, sizeof(aid));
    aidLen = kernel_tlv_get_l(node);
    memcpy(aid, kernel_tlv_get_v(node), aidLen);

    TRACE("EMV capk, aid:");
    TRACEBUF(aid, aidLen);

    node = kernel_tlv_find(cbk_buf, 0x8F);
    if (node == 0)
    {
        return -1;
    }
    index = kernel_tlv_get_v(node)[0];
    TRACE("EMV capk, index = %d", index);

    fd = Lib_FileOpen(FILE_NAME_EMV_CAPK, LFS_O_RDONLY);
    if (fd < 0)
    {
        TRACE("EMV capk, fopen fail");
        return -1;
    }

    while (Lib_FileRead(fd, tempBuf, EMV_CAPK_PAGE_SIZE) == EMV_CAPK_PAGE_SIZE)
    {
        unsigned char *pNode = 0;
        unsigned char *pValue = 0;

        // memset(tempBuf, 0, sizeof(tempBuf));
        // fread(tempBuf, sizeof(char), EMV_CAPK_PAGE_SIZE, fp);
        memset(tlvPool, 0, sizeof(tlvPool));
        kernel_tlv_new(tlvPool, 0x7F00, EMV_CAPK_PAGE_SIZE, tempBuf);
        pNode = kernel_tlv_find(tlvPool, 0x9F06);
        if (pNode == 0)
        {
            continue;
        }
        pValue = kernel_tlv_get_v(pNode);
        kernel_tlv_get_l(pNode);
  
        if (memcmp(aid, pValue, 5) != 0)
        {
            continue;
        }
        TRACEBUF(pValue, 5);
        TRACEBUF(tlvPool,EMV_CAPK_PAGE_SIZE);
        pNode = kernel_tlv_find(tlvPool, 0x9F22);
        if (pNode == 0)
        {
            continue;
        }
        TRACE("EMV capk, check index = %d", kernel_tlv_get_v(pNode)[0]);
        if (kernel_tlv_get_v(pNode)[0] != index)
        {
            continue;
        }

        flag = 1;
        break;
    }

    Lib_FileClose(fd);

    if (flag == 0)
    {
        TRACE("EMV capk, not find");
        return -1;
    }

    cbk_buf_len = kernel_tlv_new(cbk_buf, 0x7F00, 0, 0);

    node = kernel_tlv_find(tlvPool, 0xDF02);
    if (node == 0)
    {
        return -1;
    }
    cbk_buf_len = kernel_tlv_add_child(cbk_buf, 0xDF8201, kernel_tlv_get_l(node),
                                       kernel_tlv_get_v(node));

    node = kernel_tlv_find(tlvPool, 0xDF04);
    if (node == 0)
    {
        return -1;
    }
    cbk_buf_len = kernel_tlv_add_child(cbk_buf, 0xDF8202, kernel_tlv_get_l(node),
                                       kernel_tlv_get_v(node));

    node = kernel_tlv_find(tlvPool, 0xDF05);
    if (node)
    {
    }

    TRACE("EMV capk (%d):", cbk_buf_len);
    TRACEBUF(cbk_buf, cbk_buf_len);

    return 0;
}

static int trans_cbk_fs_get_exception_file(unsigned char *cbk_buf,
                                    unsigned int cbk_buf_len)
{
    kernel_tlv_new(cbk_buf, 0x7F00, 0, 0);
    cbk_buf_len =
        kernel_tlv_add_child(cbk_buf, 0xDF05, 1, (unsigned char *)"\x00");
    return cbk_buf_len;
}

#if 0
static void power_on_display_processing(void)
{
    unsigned char cmd = CMD_DISPLAY_UIRD_MSG;
    unsigned char disp_buf[128];
    unsigned int disp_buf_len = 0;
    // unsigned char *node = 0;
    user_interface_request_data_t UIRD;

    memset(&UIRD, 0, sizeof(user_interface_request_data_t));
    UIRD.ucMessageIdentifier = CONTACTLESS_UIRD_MessageID_PROCESSING;
    UIRD.ucStatus = CONTACTLESS_UIRD_STATUS_NA;
    UIRD.ucHoldTime[0] = 0;
    UIRD.ucHoldTime[1] = 0;
    UIRD.ucHoldTime[2] = 0;

    kernel_tlv_new(disp_buf, 0x7F00, 0, 0);
    kernel_tlv_add_child(disp_buf, 0xDF00, 1, &cmd);
    disp_buf_len = kernel_tlv_add_child(disp_buf, 0xDF8116, sizeof(user_interface_request_data_t), (unsigned char *)&UIRD);

    app_user_cbk(NULL, disp_buf, disp_buf_len);
}
#endif

static void trans_cbk_power_on(void)
{
    // int ret = -1;
    // int cardtype = 0;
    Signal_Message SignalMessage;

    memset(&SignalMessage, 0x00, sizeof(Signal_Message));
    SignalMessage.syn_data = 0;
    SignalMessage.syn_data_len = 0;
#if 1
    // current flow already poweron for detect card, just check the mode here
    TRACE("Emv poweron s_emv_card_mode = %d", s_emv_card_mode);
    if (s_emv_card_mode == NFC_DETECETED)
    {
        SignalMessage.signal_type = CORRESPONDING_SIGNAL_RA;
        emv_kernel_customized((unsigned char *)&SignalMessage, sizeof(Signal_Message));
    }
    else if (s_emv_card_mode == SCI_DETECETED)
    {
        SignalMessage.signal_type = CORRESPONDING_SIGNAL_IC_CARD_ACTIVE;
        emv_kernel_customized((unsigned char *)&SignalMessage, sizeof(Signal_Message));
    }
    else
    {
        SignalMessage.signal_type = CORRESPONDING_SIGNAL_STOP;
        emv_kernel_customized((unsigned char *)&SignalMessage, sizeof(Signal_Message));
    }
#else
    if (g_card_type == 0x01)
    {
        g_card_type = 0;
        SignalMessage.signal_type = CORRESPONDING_SIGNAL_RA;
        emv_kernel_customized((unsigned char *)&SignalMessage, sizeof(Signal_Message));
        return;
    }
    /*
        ret = reader_enable();
        if (ret)
        {
            SignalMessage.signal_type = CORRESPONDING_SIGNAL_STOP;
            emv_kernel_ins_customized((unsigned char *)&SignalMessage, sizeof(Signal_Message));
            return;
        }

        ret = reader_card_wait(&cardtype);
        if (ret == 0)
        */
    {
        power_on_display_processing();
        switch (cardtype)
        {
        case CARD_NFC:
            SignalMessage.signal_type = CORRESPONDING_SIGNAL_RA;
            break;
        case CARD_IC:
            SignalMessage.signal_type = CORRESPONDING_SIGNAL_IC_CARD_ACTIVE;
            break;
        case CARD_MAG:
            SignalMessage.signal_type = CORRESPONDING_SIGNAL_MAG_CARD_ACTIVE;
            break;
        default:
            break;
        }
    }
    else if (ret == ERR_USER_CANCEL)
    {
        SignalMessage.signal_type = CORRESPONDING_SIGNAL_CANCEL;
    }
    else if (ret == ERR_TIME_OUT)
    {
        SignalMessage.signal_type = CORRESPONDING_SIGNAL_TIMEOUT;
    }
    else
    {
        SignalMessage.signal_type = CORRESPONDING_SIGNAL_STOP;
    }

    emv_kernel_customized((unsigned char *)&SignalMessage, sizeof(Signal_Message));
#endif
}

static void trans_cbk_power_off(void)
{
    FUN_IN;
    // OsMsrClose();
    // OsIccClose(0);
    // OsPiccClose();
}

static unsigned char aucResp[1024];
static unsigned short usRespLen = 0;
static int trans_cbk_nfc_apdu(unsigned char *cbk_buf, unsigned int cbk_buf_len)
{
    int ret = -1;
    unsigned char *pNode = 0;
    unsigned char *pValue = 0;
    unsigned int length = 0;

    Signal_Message SignalMessage;

    // memset(&ApduRsp,0,sizeof(ST_APDU_RSP));
    // memset(&ApduReq,0,sizeof(ST_APDU_REQ));

    pNode = kernel_tlv_find(cbk_buf, 0xDF01);
    length = kernel_tlv_get_l(pNode);
    pValue = kernel_tlv_get_v(pNode);

    TRACE("EMV NFC C-APDU(%d):", length);
    TRACEBUF(pValue, length);

    ret = Lib_PiccCommand((unsigned char *)pValue, length, aucResp, &usRespLen);
    TRACE("EMV NFC ret=%d, usRespLen=%d", ret, usRespLen);
    // TRACEBUF(aucResp, usRespLen);
    if (ret <= 0)
    {
        TRACE("EMV OsPiccIsoCommand Fail");
        aucResp[0] = PAYPASS_ERROR_INDICATION_L1_TIME_OUT_ERROR;
        SignalMessage.signal_type = CORRESPONDING_SIGNAL_L1RSP;
        SignalMessage.syn_data = aucResp;
        SignalMessage.syn_data_len = 1;
        emv_kernel_customized((unsigned char *)&SignalMessage, sizeof(Signal_Message));
        return ret;
    }

    // ApduRsp.DataOut[ApduRsp.LenOut] = ApduRsp.SWA;
    // ApduRsp.DataOut[ApduRsp.LenOut + 1] = ApduRsp.SWB;

    memset(&SignalMessage, 0x00, sizeof(Signal_Message));
    SignalMessage.signal_type = CORRESPONDING_SIGNAL_RA;
    SignalMessage.syn_data = aucResp;
    SignalMessage.syn_data_len = usRespLen;

    TRACE("EMV NFC R-APDU(%d):", SignalMessage.syn_data_len);
    TRACEBUF(SignalMessage.syn_data, SignalMessage.syn_data_len);

    emv_kernel_customized((unsigned char *)&SignalMessage, sizeof(Signal_Message));

    return 0;
}

static int trans_cbk_icc_apdu(unsigned char *cbk_buf, unsigned int cbk_buf_len)
{
    int ret;
    unsigned char *pNode = 0;
    unsigned char *pValue = 0;
    unsigned int length = 0;

    pNode = kernel_tlv_find(cbk_buf, 0xDF01);
    length = kernel_tlv_get_l(pNode);
    pValue = kernel_tlv_get_v(pNode);

    TRACE("EMV ICC C-APDU(%d):", length);
    TRACEBUF(pValue, length);
    unsigned char Lc = pValue[4];
    unsigned char Le = pValue[length - 1];
    // if((pValue[0] == 0x00 && pValue[1] == 0xA4 ) || (pValue[0] == 0x80 && pValue[1] == 0xA8) )
    if ((Lc != 0) && (Le == 0))
        ret = Lib_IccCommand(ICC_SLOT_EMV_IC, (unsigned char *)pValue, length-1,aucResp, &usRespLen);
    else
        ret = Lib_IccCommand(ICC_SLOT_EMV_IC, (unsigned char *)pValue, length,aucResp, &usRespLen); 

    TRACE("EMV ICC ret=%d, usRespLen=%d", ret, usRespLen);

    if ((ret > 0) && (usRespLen == 2) && (aucResp[0] == 0x61))
    {
        TRACEBUF(aucResp, usRespLen);
        unsigned char c_aucGetRspnd[5] = {0x00, 0xc0, 0x00, 0x00}; // 自动取响应命令
        c_aucGetRspnd[4] = aucResp[1];
        ret = Lib_IccCommand(ICC_SLOT_EMV_IC, (unsigned char *)c_aucGetRspnd, 5,
                             aucResp, &usRespLen);

        TRACE("EMV ICC1 ret=%d, usRespLen=%d", ret, usRespLen);
    }
    if ((ret > 0) && (usRespLen == 2) && (aucResp[0] == 0x6C))
    {
        TRACEBUF(aucResp, usRespLen);
        pValue[4] = aucResp[1]; // 卡片期望的正确 Le 值
        ret = Lib_IccCommand(ICC_SLOT_EMV_IC, (unsigned char *)pValue, length,
                             aucResp, &usRespLen);

        TRACE("EMV ICC2 ret=%d, usRespLen=%d", ret, usRespLen);
    }

    if (ret <= 0)
    {
        TRACE("EMV OsIccExchange Fail");
        return -1;
    }

    TRACE("EMV ICC R-APDU(%d):", usRespLen);
    TRACEBUF(aucResp, usRespLen);

    kernel_tlv_new(cbk_buf, 0x7F00, 0, 0);
    cbk_buf_len = kernel_tlv_add_child(cbk_buf, 0xDF01, usRespLen, aucResp);

    return 0;
}

static int trans_cbk_total_amount(unsigned char *cbk_buf, unsigned int cbk_buf_len)
{
    cbk_buf_len = kernel_tlv_replace(cbk_buf, 0x9F8219, 6,
                                     (unsigned char *)"\x00\x00\x00\x00\x00\x00");
    return 0;
}

static void trans_cbk_get_random(unsigned char *cbk_buf, unsigned int cbk_buf_len)
{
    unsigned char *pnode = 0;
    unsigned char random[128];
    unsigned int random_len = 4;

    pnode = kernel_tlv_find(cbk_buf, 0x9F8216);
    if (pnode)
    {
        random_len = kernel_tlv_get_v(pnode)[0];
    }

    Lib_AlgGetRandom(random, random_len);

    cbk_buf_len = kernel_tlv_add_child(cbk_buf, 0x9F37, random_len, random);
}

int trans_cbk(void *user_data, unsigned char *cbk_buf, unsigned int cbk_buf_len)
{
    FUN_IN;
    int ret = 0;
    unsigned char *node = 0;
    unsigned char cmd = 0;

    node = kernel_tlv_find(cbk_buf, 0xDF00);
    if (node == 0)
    {
        return -1;
    }
    cmd = kernel_tlv_get_v(node)[0];

    TRACE("EMV trans cbk, CMD = %02XH", cmd);
    TRACEBUF(cbk_buf, cbk_buf_len);
    switch (cmd)
    {
    case CMD_REQUEST_SAME_CARD_TOTAL_TRANS_AMOUNT:
        /* cless */
        break;
    case CMD_REQUEST_RUPAY_SERVICE_SHORTLISTING:
        /* cless */
        break;
    case CMD_REQUEST_RUPAY_SERVICE_GET_PRMACQ_KEY:
        /* cless */
        break;
    case CMD_REQUEST_DATAEXCHANGEBUFFER:
        /* not used */
        break;
    case CMD_REQUEST_TORN_BUFFER1:
        /* not used */
        break;
    case CMD_REQUEST_STORES_ONLINE_TRANSACTION_CONTEXT: /* cless */
        // trans_cbk_store_online_data(cbk_buf, cbk_buf_len);
        break;
    case CMD_REQUEST_GET_ONLINE_TRANSACTION_CONTEXT: /* cless */
        // trans_cbk_get_online_data(cbk_buf, cbk_buf_len);
        break;
    case CMD_REQUEST_STORES_RECOVERY_CONTEXT:
        /* cless */
        break;
    case CMD_REQUEST_GET_RECOVERY_CONTEXT:
        /* cless */
        break;
    case CMD_REQUEST_STORES_DELAY_AUTH_OUTCOME:
        /* cless */
        break;
    case CMD_AMEX_GET_Dynamic_Limit_Param: /* cless */
        // ret = trans_cbk_fs_get_drl(cbk_buf, cbk_buf_len);
        break;
    case CMD_EMV_CALLBACK_EXCHANGE_APDU: /* contact and cless->icc */
        ret = trans_cbk_icc_apdu(cbk_buf, cbk_buf_len);
        break;
    case CMD_EMV_CALLBACK_DISPLAY_UIRD_MSG:
        /* not used */
        break;
    case CMD_EMV_CALLBACK_GET_AID_PARAM: /* contact and cless->icc */
    case CMD_REQUEST_AID_PARAM:          /* cless and quics */
        trans_cbk_fs_get_aid_param(cbk_buf, cbk_buf_len);
        break;
    case CMD_EMV_CALLBACK_GET_PUBLIC_KEY: /* contact and cless->icc */
    case CMD_REQUEST_PKI:                 /* cless and quics */
        ret = trans_cbk_fs_get_capk(cbk_buf, cbk_buf_len);
        break;
    case CMD_EMV_CALLBACK_GET_RANDOM_NUMBER:            /* contact and cless->icc */
    case CMD_EMV_CALLBACK_GET_ANY_LENGTH_RANDOM_NUMBER: /* contact and cless->icc */
    case CMD_REQUEST_UNPREDICATABLE_NUMBER:             /* cless and quics */
        trans_cbk_get_random(cbk_buf, cbk_buf_len);
        break;
    case CMD_EMV_CALLBACK_GET_TOTAL_AMOUNT: /* contact and cless->icc */
        trans_cbk_total_amount(cbk_buf, cbk_buf_len);
        break;
    case CMD_EMV_CALLBACK_CHECK_EXCEPTION_FILE: /* contact and cless->icc */
    case CMD_REQUEST_EXCEPTION_FILE:            /* cless and quics */
        ret = trans_cbk_fs_get_exception_file(cbk_buf, cbk_buf_len);
        break;
    case CMD_EMV_CALLBACK_CHECK_IPK_REVOKE: /* contact and cless->icc */
    case CMD_REQUEST_PKI_IPK_REVO:          /* cless and quics */
        // ret = trans_cbk_fs_get_ipk_revoke(cbk_buf, cbk_buf_len);
        break;
    case CMD_EMV_CALLBACK_ISSUER_REFERAL:
        /* contact and cless->icc */
        break;
    case CMD_EMV_CALLBACK_PBOC_AUTHENTICATION:
        /* contact and cless->icc */
        break;
    case CMD_EMV_CALLBACK_TRANS_FORCE_ACCEPT:
        /* contact and cless->icc */
        break;

    case CMD_NOTHING_TO_DO:
        /* not used */
        break;
    case CMD_REQUEST_ICS: /* cless */
        trans_cbk_fs_get_ics(cbk_buf, cbk_buf_len);
        break;
    case CMD_REQUEST_AID_LIST: /* cless */
        trans_cbk_fs_get_aid_list(cbk_buf, cbk_buf_len);
        break;
    case CMD_REQUEST_POWER_ON: /* cless and quics */
        trans_cbk_power_on();
        break;
    case CMD_REQUEST_PDOL_DATA:
        /* not used */
        break;
    case CMD_TRANSACTION_APPROVED:
        /* not used */
        break;
    case CMD_TRANSACTION_ONLINE_REQUEST:
        /* not used */
        break;
    case CMD_TRANSACTION_ADVICE:
        /* not used */
        break;
    case CMD_TRANSACTION_DECLINED:
        /* not used */
        break;
    case CMD_TRANSACTION_TERMINATE:
        /* not used */
        break;
    case CMD_TRANSACTION_ITEM_REQUEST:
        /* not used */
        break;
    case CMD_TRANSACTION_FIELD_OFF: /* cless and quics */
        trans_cbk_power_off();
        break;
    case CMD_CARDHOLDER_CONFIRMATION:
        /* not used */
        break;
    case CMD_GET_TXN_LOG:
        /* not used */
        break;
    case CMD_GET_TOP_UP_LOG:
        /* not used */
        break;
    case CMD_GET_BALANCE:
        /* not used */
        break;
    case CMD_LED_BLINK:
        /* not used */
        break;
    case CMD_TRANSMIT_DEK:
        /* cless */
        break;
    case CMD_TRANSMIT_APDU_KERNEL: /* cless and quics */
        trans_cbk_nfc_apdu(cbk_buf, cbk_buf_len);
        break;
    case CMD_MIR_DATA_EXCHANGE: /* cless */
        // ret = trans_cbk_mir_data_exchange(cbk_buf, cbk_buf_len);
        break;
    case CMD_TIMER_START: /* cless */
        // trans_cbk_timer_start(cbk_buf, cbk_buf_len);
        break;
    case CMD_TIMER_STOP: /* cless */
        // trans_cbk_timer_stop(cbk_buf, cbk_buf_len);
        break;
    case CMD_REQUEST_TORN_BUFFER:
        /* cless */
        break;
    case CMD_WAIT_BY_APP_MS:
        /* cless */
    case CMD_CCC_NO_RES:
        /* cless */
        break;
    case CMD_SAVE_RRP_TIME:
        /* cless */
        break;
    case CMD_TRANSMIT_TORN:
        /* cless */
        break;
    case CMD_ICC_REQUEST_AID_LIST: /* icc */
        trans_cbk_fs_get_bctc_aid_list(cbk_buf, cbk_buf_len);
        break;
    case CMD_READER_MAG_DATA: /* not used */
        // trans_cbk_mag_data(cbk_buf, cbk_buf_len);
        break;

    case CMD_PRINT_MSG_DEBUG:

        break;
    case CMD_CVM_OFFLINE_PIN:              /* not used */
    case CMD_CVM_ONLINE_PIN:               /* cless */
    case CMD_EMV_CALLBACK_CVM_OFFLINE_PIN: /* contact and cless->icc */
    case CMD_EMV_CALLBACK_CVM_ONLINE_PIN:  /* contact and cless->icc */
    case CMD_EMV_CALLBACK_ONLINE_REQUEST:  /* contact and cless->icc */
    case CMD_DISPLAY_UIRD_MSG:             /* cless and quics */
    case CMD_DISPLAY_ICC_MSG:
    case CMD_EMV_CALLBACK_CARDHOLDER_CONFIRMATION: /* contact and cless->icc */
    case CMD_NFC_ONLINE_REQUEST:
    case CMD_OUT_SIGNAL_ENTRYPOINT: /* cless and quics */
    case CMD_EMV_CALLBACK_GET_PAN_BEFORE_ODA_CUSTOMER:
    default:
        app_user_cbk(NULL, cbk_buf, cbk_buf_len);
        break;
    }
    return ret;
}


static int disp_format_type(EmvKernelDisp type)
{
    int ret = 0;
    if(Trade_EMV_Dispmsg != NULL)
        ret = Trade_EMV_Dispmsg(type);
    return ret;    
}

static int disp_see_phone()
{
    return disp_format_type(EMV_DISP_SEE_PHONE);
}

static void disp_nfc_retap()
{
    disp_format_type(EMV_DISP_NFC_RETAP);
}

static void disp_remove_card()
{
    disp_format_type(EMV_DISP_REMOVE_CARD);
}

static void disp_reading_card()
{
    disp_format_type(EMV_DISP_READING_CARD);
}
#if 0
static void disp_offline_pin_blocked()
{
    disp_format_type(EMV_DISP_OFFLINE_PIN_BLOCKED);
}
#endif
static void ContactAddTagProcess(pu8 pTradFile)
{
    pu8 pNode = NULL;
    pu8 pTempBuffer = NULL;
    u32 tempBufferLen = 0;
    u32 ret;
    pTempBuffer = malloc(128);

    pNode = kernel_tlv_find(pTradFile,0x4F);
    if(!pNode)
    {
        ret = emv_kernel_get_contact_element(0x84,pTempBuffer,(pu32)&tempBufferLen);
        if(ret == 0)
        {
            kernel_tlv_replace(pTradFile,0x4F,tempBufferLen,pTempBuffer);
        }
    }

    free(pTempBuffer);
}

static int onlineResultProcess(unsigned char* pResData,EmvOnlineData_t* pOnlineData)
{
    int onlineDataLen = 0;
    pu8 pTempBuf = NULL;
    pTempBuf = malloc(256);
    memset(pTempBuf,0,256);

	TRACE("\r\nDspread: <iccResponse> = %s\r\n", pOnlineData->iccResponse);
    TRACE("\r\nDspread: ackdata[%d]\r\n", pOnlineData->ackdatalen);
    TRACEBUF(pOnlineData->ackdata,pOnlineData->ackdatalen);

    if(strlen(pOnlineData->iccResponse) > 0)
    {
        sprintf((char *)pTempBuf,"\x8A\x02%s",pOnlineData->iccResponse);
        onlineDataLen += 4;
    }
    memcpy(pTempBuf+onlineDataLen,pOnlineData->ackdata,pOnlineData->ackdatalen);
    onlineDataLen += pOnlineData->ackdatalen;

    kernel_tlv_new(pResData,0x7F00,onlineDataLen,pTempBuf);

    if( memcmp(pOnlineData->iccResponse,"00",2) == 0)
    {
        glb_online_trans_result = true;
    }
    else
    {
        glb_online_trans_result = false;
    }

    TRACE("\r\nDspread: <pResData>\r\n");
    TRACEBUF(pResData,kernel_tlv_get_node_l(pResData));

    free(pTempBuf);
    return onlineDataLen;
}
static int app_user_online_request(unsigned char *buf, unsigned int len)
{
    EmvOnlineData_t emvOnlineData;
    memset(&emvOnlineData,0,sizeof(EmvOnlineData_t));

    tlv_batch_replace_(buf,pTransactionPool);

    TRACE("pTransactionPool:");
    TRACEBUF(pTransactionPool,kernel_tlv_get_node_l(pTransactionPool));
    ContactAddTagProcess(pTransactionPool);
    trans_gac_time = 0x01;

    if(Trade_OnlineProcess)
    {
        Trade_OnlineProcess(&emvOnlineData);
        onlineResultProcess(buf,&emvOnlineData);  
    }

    trans_gac_time = 0x02;

    return 0;
}

static int app_user_nfc_disp(unsigned char *buf, unsigned int len)
{
    unsigned char *node = 0;
    unsigned char *value = 0;

    user_interface_request_data_t UIRD;
    memset(&UIRD, 0, sizeof(UIRD));

    node = kernel_tlv_find(buf, 0xDF8116);
    if (node)
    {
        value = kernel_tlv_get_v(node);
        memcpy(&UIRD, (user_interface_request_data_t *)value, sizeof(user_interface_request_data_t));
    }

    TRACE("Emv app user nfc display id = %02XH", UIRD.ucMessageIdentifier);

    switch (UIRD.ucMessageIdentifier)
    {
    case CONTACTLESS_UIRD_MessageID_PRESENT_CARD:
        break;
    case CONTACTLESS_UIRD_MessageID_PROCESSING:
        disp_reading_card();
        break;
    case CONTACTLESS_UIRD_MessageID_AUTHORISING_PLEASEWATI:
        break;
    case CONTACTLESS_UIRD_MessageID_ERROR_OTHER_CARD:
        break;
    case CONTACTLESS_UIRD_MessageID_DECLINED:
        break;
    case CONTACTLESS_UIRD_MessageID_APPROVED:
        break;
    default:
        break;
    }

    return 0;
}

static int nfc_try_again()
{
    int ret = -1;
    int key = 0;
    bool loop = true;
    Lib_PiccClose();
    Lib_PiccOpen();

    while(loop)
    {
        if (CARD_NONE < Lib_PiccInquiry( NULL, NULL, NULL, NULL))
        {
            TRACE("Lib_PiccInquiry2 OK");
            disp_remove_card();
            Lib_Beep(100);
        }
        else
        {
            loop = false;
        }
    }

    disp_nfc_retap();

    Lib_PiccClose();
    Lib_PiccOpen();

    while(1)
    {
        key = LvUiWaitKey(40);

		if (LVGL_OPER_TIMEOUT != key)
		{
			if (KB_CANCEL == key)
			{
				glb_emv_kernel_process_status = KERNEL_PROCESS_STATUS_NFC_RETAP_CANCLE; 
                ret = -1;
                break;
			}
			else
			{
				continue;
			}
		}
        else
        {
            glb_emv_kernel_process_status = KERNEL_PROCESS_STATUS_NFC_RETAP_TIMEOUT; 
            ret = -1;
            break;
        }

        if (CARD_NONE < Lib_PiccInquiry( NULL, NULL, NULL, NULL))
        {
            TRACE("Lib_PiccInquiry2 OK");
            Lib_Beep(50);
            s_emv_card_mode =  NFC_DETECETED;
            glb_emv_kernel_process_status = KERNEL_PROCESS_STATUS_NFC_RETAP_SUCCESS; 
            ret = 0;
            break;
        }
    }
    return ret;
}

#if 0
static int app_user_icc_disp(unsigned char *buf, unsigned int len)
{
    unsigned char *node = 0;
    unsigned char disp_id = 0;

    node = kernel_tlv_find(buf, 0x9F8711);
    if (node == 0)
    {
        return -1;
    }

    disp_id = kernel_tlv_get_v(node)[0];

    TRACE("Emv app user icc display id = %02XH", disp_id);

    switch (disp_id)
    {
    case CONTACT_DISP_ID_OFFLINE_APPROVED:
        // TransView_vShowLine(2,EM_DTYPE_NORMAL,EM_ALIGN_CENTER,(char*)"offline
        // approve");
        break;
    case CONTACT_DISP_ID_OFFLINE_DECLINED:
        // TransView_vShowLine(2,EM_DTYPE_NORMAL,EM_ALIGN_CENTER,(char*)"offline
        // declined");
        break;
    case CONTACT_DISP_ID_ONLINE_APPROVED:
        // TransView_vShowLine(2,EM_DTYPE_NORMAL,EM_ALIGN_CENTER,(char*)"online
        // approve");
        break;
    case CONTACT_DISP_ID_ONLINE_DECLINED:
        // TransView_vShowLine(2,EM_DTYPE_NORMAL,EM_ALIGN_CENTER,(char*)"online
        // declined");
        break;
    case CONTACT_DISP_ID_UNABLEONLINE_OFFLINE_APPROVED:
        break;
    case CONTACT_DISP_ID_UNABLEONLINE_OFFLINE_DECLINED:
        break;
    case CONTACT_DISP_ID_TRY_ANOTHER_INTERFACE:
        break;
    case CONTACT_DISP_ID_TERMINATE:
        // TransView_vShowLine(2,EM_DTYPE_NORMAL,EM_ALIGN_CENTER,(char*)"terminate");
        break;
    case CONTACT_DISP_ID_NOT_ALLOWED:
        break;
    case CONTACT_DISP_ID_NOT_ACCEPTED:
        break;
    default:
        // TransView_vShowLine(2,EM_DTYPE_NORMAL,EM_ALIGN_CENTER,(char*)"display...");
        // OsBeep(7, 1000);
        break;
    }

    return 0;
}
#endif

static void app_user_cardholder_confirm(unsigned char* buf, unsigned int len)
{
    pu8 pTempBuffer = NULL;
    pu8 pNode = NULL;
    pu8 pChild = NULL;
    pu8 pValue = NULL;
    u32 counter = 0;
    //u32 tempBufferLen = 0;
    AidCandidate_t aidList[10];
    int index = -1;
   // pu8 pDataBuffer = NULL;
   // u32 dataBufferLen = 0;

    pTempBuffer = malloc(512);
    memset(pTempBuffer,0,512);
    #ifdef CFG_LIB_DEBUG
    OsLog(LOG_DEBUG,"Dspread: select app:");
#endif

    pNode = kernel_tlv_find(buf, 0x7F35);
    if(pNode == 0)
    {
        return;
    }
    
    pChild = kernel_tlv_get_first_child(pNode);
    while(pChild)
    {
        if(kernel_tlv_get_t(pChild) != 0x7F36)
        {
            continue;
        }
        memset(pTempBuffer,0,512);
        kernel_tlv_new(pTempBuffer,0x7F36,kernel_tlv_get_l(pChild),kernel_tlv_get_v(pChild));

        pValue = kernel_tlv_find(pTempBuffer,0x87);
        if(pValue) 
            aidList[counter]._priority = kernel_tlv_get_v(pValue)[0];

        pValue = kernel_tlv_find(pTempBuffer,0x9F12);
        if(pValue) 
        {
            memcpy(aidList[counter]._preferred_name,kernel_tlv_get_v(pValue),kernel_tlv_get_l(pValue));
            aidList[counter]._preferred_name_len = kernel_tlv_get_l(pValue);
        }
        else
            aidList[counter]._preferred_name_len = 0;

        pValue = kernel_tlv_find(pTempBuffer,0x50);
        if(pValue) 
        {
            memcpy(aidList[counter]._lable,kernel_tlv_get_v(pValue),kernel_tlv_get_l(pValue));
            aidList[counter]._lable_len = kernel_tlv_get_l(pValue);
        }
        else
            aidList[counter]._lable_len = 0;

        pValue = kernel_tlv_find(pTempBuffer,0x4F);
        if(pValue) 
        {
            memcpy(aidList[counter]._aid,kernel_tlv_get_v(pValue),kernel_tlv_get_l(pValue));
            aidList[counter]._aid_len = kernel_tlv_get_l(pValue);
        }
        else
            aidList[counter]._aid_len = 0;

        aidList[counter]._enable = 1;

        pValue = kernel_tlv_find(pTempBuffer,0x9F11);
        if(pValue && kernel_tlv_get_l(pValue) != 0)
        {
            aidList[counter]._table_index = kernel_tlv_get_v(pValue)[0];
        }
        else
        {
            aidList[counter]._table_index = 0;
        }

        counter++;
        pChild = kernel_tlv_get_next_child(pChild);

        if(counter >= 10)
            break;
    }
    if(Trade_EMV_AppSelect != NULL)
        index = Trade_EMV_AppSelect(aidList,counter);
        TRACE("AppSelect index = %d",index);
    if(index >= 0)
    {
        u8  tempData[5] = {0x00};
        tempData[0] = index;
        kernel_tlv_new(buf,0x7F00,0,0);
        kernel_tlv_add_child(buf, KERNEL_TAG_AID_SELECT_RESULT_INDEX, 1,tempData );
    }
    disp_reading_card();
    free(pTempBuffer);
}

static int getPan(unsigned char *buf,unsigned char *pan)
{
    unsigned char *node = 0;
    unsigned char *pValue = 0;
    unsigned int valueLen = 0;

    node = kernel_tlv_find(buf, 0x5A);
    if (node == NULL)
        node = kernel_tlv_find(buf, 0x57);
    pValue = kernel_tlv_get_v(node);
    valueLen = kernel_tlv_get_l(node);

    unsigned char aucCardNo[32];
    memset(aucCardNo, 0, sizeof(aucCardNo));
    int n = 0;
    if(pValue && valueLen > 0){
		memset(aucCardNo,0,sizeof(aucCardNo));
        Lib_Bcd2Asc(pValue, (char*)aucCardNo, valueLen);
        TRACE("aucCardNo len %d aucCardNo = %s", valueLen*2, aucCardNo);
		while(aucCardNo[n] >= '0' && aucCardNo[n] <= '9'){
			n++;
		}
		aucCardNo[n] = 0x00;
		if(n == 0){
			TRACE("get pan fail");
			return 0;
		}
		strcpy((char *)pan,(char *)aucCardNo);
        TRACE("Emv len %d CardNo = %s", n, pan);
		return n;
	}
    return 0;
}
static int app_user_input_online_pin( unsigned int cmd,unsigned char *buf)
{
    int ret = -1;
    //unsigned char *node = 0;
    //unsigned char *pValue = 0;
    //unsigned int valueLen = 0;
    char pin[32] = {0};
    char pan[32] = {0};

    if(emv_last_offline_pin_flag)
    {
        kernel_tlv_replace(pTransactionPool,0x9F17,1,(pu8)"\x00");
    }

    if(getPan(buf, (pu8)pan) == 0)
        return -1;

    if(Trade_PinEnter)
        ret = Trade_PinEnter(EMV_ONLINEPIN_INPUT,pan,pin);
    else
        ret = -1; 
    TRACE("---------------1---------------");
    if(cmd == CMD_CVM_ONLINE_PIN)  //nfc
    {
        TRACE("CMD_CVM_ONLINE_PIN");
        if(ret > 0)
        {
            //memset(buf, 0, sizeof(buf));
            kernel_tlv_new(buf,POS_TAG_HEAD,0,NULL);
            kernel_tlv_replace(buf,0x9F8156,ret,(unsigned char *)pin);
        }
        else if(ret == 0)
        {
            //memset(buf, 0, sizeof(buf));
            kernel_tlv_new(buf,POS_TAG_HEAD,0,NULL);
            kernel_tlv_replace(buf,0x9F8156,0,NULL);
            ret = 0;
        }
    }
    else if(cmd == CMD_EMV_CALLBACK_CVM_ONLINE_PIN) //chip
    {
        TRACE("CMD_EMV_CALLBACK_CVM_ONLINE_PIN");
        if(ret > 0)
        {
            //memset(buf, 0, sizeof(buf));
            kernel_tlv_new(buf,POS_TAG_HEAD,0,NULL);
            kernel_tlv_replace(buf,0x99,8,(unsigned char *)pin);
        }
        else if(ret == 0)
        {
            //memset(buf, 0, sizeof(buf));
            kernel_tlv_new(buf,POS_TAG_HEAD,0,NULL);
            kernel_tlv_replace(buf,0x99,0,NULL);
            ret = 0;
        }
    }

    return ret;
}
static int app_user_input_offline_pin(unsigned char *usr_data)
{
    int ret;
    //int pinLength;
    char pin[32] = {0};
    char pan[32] = {0};
    //int pinLenMin = 4;
    //int pinLenMax = 6;
    int counter = 0;
    pu8 node = NULL;

    node = kernel_tlv_find(usr_data, 0x9F821B);
    EMV_INPUT_PIN_TYPE pinType;

    if(node)
    {
        kernel_tlv_replace(pTransactionPool,0x9F17,kernel_tlv_get_l(node), kernel_tlv_get_v(node));
        counter = kernel_tlv_get_v(node)[0];
    }
    TRACE("offline pin counter %d",counter);
    if(counter == 1)
    {
        emv_last_offline_pin_flag = true;
        pinType = EMV_LAST_OFFLINEPIN_INPUT;
    }
    else    
        pinType = EMV_OFFLINEPIN_INPUT;

    if(Trade_PinEnter)
        ret = Trade_PinEnter(pinType,pan,pin);
    else
        ret = -1;    
    if(ret > 0)
    {
        //memset(usr_data, 0, sizeof(usr_data));
        kernel_tlv_new(usr_data,POS_TAG_HEAD,0,NULL);
        kernel_tlv_replace(usr_data,0x9F8155,strlen(pin),(unsigned char *)pin);
    }
    else if(ret == 0)  //by pass
    {
        //memset(usr_data, 0, sizeof(usr_data));
        kernel_tlv_new(usr_data,POS_TAG_HEAD,0,NULL);
        kernel_tlv_replace(usr_data,0x9F8155,0,NULL);
        ret = 0;
    }
    else
        ret = -1;    
    Lib_DelayMs(10);
#ifdef CFG_LIB_DEBUG
    OsLog(LOG_DEBUG,"Dspread: Ped get pin ret = %d",ret); 
#endif
    disp_reading_card();
    return ret;
}

extern void kernel_entry_point_customized(unsigned char*);
static int app_user_see_phone_process(unsigned char* buf, unsigned int len)
{
    int ret = 0;
    unsigned char* node = 0;
    unsigned char* value = 0;
    user_interface_request_data_t UIRD;
    memset(&UIRD, 0, sizeof(UIRD));

    node = kernel_tlv_find(buf, 0xDF8116);
	if (node)
	{
		value = kernel_tlv_get_v(node);
		memcpy(&UIRD, (user_interface_request_data_t*)value, sizeof(user_interface_request_data_t));
	}
    TRACE( "Dspread: app user nfc display id = %02XH", UIRD.ucMessageIdentifier);
    switch(UIRD.ucMessageIdentifier)
    {
    case CONTACTLESS_UIRD_MessageID_SEE_PHONE:
        Lib_Beep(300);
        //OsSleep(500);
        ret = disp_see_phone();
        if(ret != 0)  //setis 需求，see phone 后内核终止
        {
            Signal_Message SignalMessage;
            memset(&SignalMessage, 0x00, sizeof(Signal_Message));
            SignalMessage.signal_type = CORRESPONDING_SIGNAL_STOP;
            SignalMessage.syn_data = 0;
            SignalMessage.syn_data_len = 0;
            kernel_entry_point_customized((unsigned char*)&SignalMessage);

            return -1;
        }
        break;
    case CONTACTLESS_UIRD_MessageID_TRY_AGAIN:
        TRACE( "Dspread: CONTACTLESS_UIRD_MessageID_TRY_AGAIN id = %02XH", CONTACTLESS_UIRD_MessageID_TRY_AGAIN);
        disp_nfc_retap();
        Lib_DelayMs(10);
        break;
    default:
        break;
    }

    node = kernel_tlv_find(buf, 0xDF8129);
	if (node)
	{
		value = kernel_tlv_get_v(node);
		if(value[1] == 0x10)
        {
            ret = nfc_try_again();

            if(ret != 0)
            {
                Signal_Message SignalMessage;
                memset(&SignalMessage, 0x00, sizeof(Signal_Message));
                SignalMessage.signal_type = CORRESPONDING_SIGNAL_STOP;
                SignalMessage.syn_data = 0;
                SignalMessage.syn_data_len = 0;
                kernel_entry_point_customized((unsigned char*)&SignalMessage);
                return -1;
            }
            glb_emv_kernel_process_status = KERNEL_PROCESS_STATUS_NFC_RETAP_SUCCESS;
        }
	}
    disp_reading_card();
    return ret;
}

static int cardconfirm(unsigned char* buf, unsigned int len)
{
    int ret = 0;
    //unsigned char pinKsn[10] = {0};
    unsigned char Pan[32] = {0};
    // print_time("card info confirm");
    if(getPan(buf,Pan)  != 0)
    {
        TRACE("pan: %s",Pan);
        if(Trade_ConfirmCardInfo)
            ret = Trade_ConfirmCardInfo((char *)Pan,strlen((char *)Pan));
    }

    return ret;
}

static u32 intercept_pan_from_track2(pu8 track2dat, u32 track2length,pu8 cardNo )
{
	u16 i=0x0;

	while(track2dat[i] != 0x3d){
		if(i<track2length){
			cardNo[i]=track2dat[i];
 			i++;
		}
		else{
			break;
		}
	}
	return i;
}

static void ContactlessAddTagProcess(pu8 pTlvBuffer)
{
    pu8 pNode = NULL;
	pu8 pValue = NULL;
    int length = 0;
    pu8 pTempBuff = NULL;

    //append 9F34 for visa/union
	pNode = kernel_tlv_find(pTlvBuffer,0x9F06);
    if(pNode)
    {
        pValue = kernel_tlv_get_v(pNode);
        if(memcmp(pValue, "\xA0\x00\x00\x00\x03", 5) == 0 || memcmp(pValue, "\xA0\x00\x00\x03\x33", 5) == 0
            ||memcmp(pValue, "\xA0\x00\x00\x01\x52", 5) == 0)
        {
            pNode = kernel_tlv_find(pTransactionPool,0xDF8129);
            if(pNode)
            {
                pValue = kernel_tlv_get_v(pNode);
                if(pValue[3] == CONTACTLESS_OPS_CVM_ONLINE_PIN )
                {
                    kernel_tlv_replace(pTlvBuffer,0x9F34,3,(pu8 )"\x42\x03\x00");
                }
                else if(pValue[3] == CONTACTLESS_OPS_CVM_SIGNATURE )
                {
                    kernel_tlv_replace(pTlvBuffer,0x9F34,3,(pu8 )"\x1E\x03\x00");
                }
                else
                {
                    kernel_tlv_replace(pTlvBuffer,0x9F34,3,(pu8 )"\x1F\x03\x00");
                }
            }
            else
            {

            }
        }
        else
        {

        }
    }
    else
    {

    }

    //9B
    pNode = kernel_tlv_find(pTlvBuffer,0x9B);
    if(!pNode)
    {
        kernel_tlv_replace(pTlvBuffer,0x9B,2,(pu8 )"\x00\x00");
    }
    else
    {

    }

    //50
    pNode = kernel_tlv_find(pTlvBuffer,0x50);
    if(!pNode)
    {
        kernel_tlv_replace(pTlvBuffer,0x50,0,NULL);
    }
    else
    {

    }
    #if 0
    pNode = kernel_tlv_find(pTlvBuffer,0x9F06);
    if(!pNode)
    {
      if((pNode = kernel_tlv_find(glb_aid_data, 0x9F06))!= NULL)
      {
        if(kernel_tlv_find(pTlvBuffer,0x9F06))
        {
            kernel_tlv_modify(pTlvBuffer,0x9F06,kernel_tlv_get_l(pNode), kernel_tlv_get_v(pNode));
        }
        else
        {
            kernel_tlv_add_child(pTlvBuffer,0x9F06,kernel_tlv_get_l(pNode), kernel_tlv_get_v(pNode));
        }

      }
    }
    #endif
    pNode = kernel_tlv_find(pTlvBuffer,0x4F);
    if(pNode)
    {
        if(kernel_tlv_find(pTlvBuffer,0x84))
        {
            kernel_tlv_modify(pTlvBuffer,0x84,kernel_tlv_get_l(pNode),kernel_tlv_get_v(pNode));
        }
        else
        {
            kernel_tlv_add_child(pTlvBuffer,0x84,kernel_tlv_get_l(pNode),kernel_tlv_get_v(pNode));
        }

    }
    else
    {
      pNode = kernel_tlv_find(pTlvBuffer,0x84);
      if(pNode)
      {
        if(kernel_tlv_find(pTlvBuffer,0x4F))
        {
            kernel_tlv_modify(pTlvBuffer,0x4F,kernel_tlv_get_l(pNode),kernel_tlv_get_v(pNode));
        }
        else
        {
            kernel_tlv_add_child(pTlvBuffer,0x4F,kernel_tlv_get_l(pNode),kernel_tlv_get_v(pNode));
        }

      }
      else
      {
          pNode = kernel_tlv_find(pTlvBuffer,0x9F06);
          if(pNode)
          {
            if(kernel_tlv_find(pTlvBuffer,0x84))
            {
                kernel_tlv_modify(pTlvBuffer,0x84,kernel_tlv_get_l(pNode),kernel_tlv_get_v(pNode));
            }
            else
            {
                kernel_tlv_add_child(pTlvBuffer,0x84,kernel_tlv_get_l(pNode),kernel_tlv_get_v(pNode));
            }

            if(kernel_tlv_find(pTlvBuffer,0x4F))
            {
                kernel_tlv_modify(pTlvBuffer,0x4F,kernel_tlv_get_l(pNode),kernel_tlv_get_v(pNode));
            }
            else
            {
                kernel_tlv_add_child(pTlvBuffer,0x4F,kernel_tlv_get_l(pNode),kernel_tlv_get_v(pNode));
            }

          }
          else
          {}

      }
    }
    //9F09
    pNode = kernel_tlv_find(pTlvBuffer,0x9F09);
    if(!pNode)
    {
        kernel_tlv_replace(pTlvBuffer,0x9F09,0,NULL);
    }
    else
    {

    }

    //9F35
    pNode = kernel_tlv_find(pTlvBuffer,0x9F35);
    if(!pNode)
    {
        kernel_tlv_replace(pTlvBuffer,0x9F35,0,NULL);
    }
    else
    {

    }

    //9F53
    pNode = kernel_tlv_find(pTlvBuffer,0x9F53);
    if(!pNode)
    {
        kernel_tlv_replace(pTlvBuffer,0x9F53,0,NULL);
    }
    else
    {

    }

    //9F7C
    pNode = kernel_tlv_find(pTlvBuffer,0x9F7C);
    if(!pNode)
    {
        kernel_tlv_replace(pTlvBuffer,0x9F7C,0,NULL);
    }
    else
    {

    }

    //5F24
    pNode = kernel_tlv_find(pTlvBuffer,0x5F24);
    if(!pNode)
    {
        kernel_tlv_replace(pTlvBuffer,0x5F24,0,NULL);
    }
    else
    {

    }

    //5A
    pu8 pTempBuffer = NULL;
    pu8 pDataBuffer = NULL;
    u32 tempBufferLen = 0;
    u32 dataBufferLen = 0;

    pTempBuffer = malloc(64);
    pDataBuffer = malloc(64);
    memset(pTempBuffer, 0x00, 64);
    memset(pDataBuffer, 0x00, 64);
    pNode = kernel_tlv_find(pTlvBuffer,0x57);
    if(!pNode)
    {
        pNode = kernel_tlv_find(pTlvBuffer,0x9F6B);
    }

    if(pNode)
    {
        length = kernel_tlv_get_l(pNode);
        pValue = kernel_tlv_get_v(pNode);
        if((pValue[length - 1] & 0x0F) == 0x0F)
            tempBufferLen = bcd_to_asc_sample(pValue,length,pTempBuffer) - 1;
        else
            tempBufferLen = bcd_to_asc_sample(pValue,length,pTempBuffer);

        if(kernel_tlv_find(pTlvBuffer,0x5A) == NULL)
        {
            dataBufferLen = intercept_pan_from_track2(pTempBuffer,tempBufferLen,pDataBuffer);
            memset(pTempBuffer, 0xFF, 20);
            tempBufferLen = asc_to_bcd_sample(pDataBuffer,dataBufferLen,pTempBuffer);
            kernel_tlv_replace(pTlvBuffer,0x5A,tempBufferLen,pTempBuffer);
        }
    }

    free(pTempBuffer);
    free(pDataBuffer);
    //8A
    pNode = kernel_tlv_find(pTlvBuffer,0x8A);
    if(!pNode)
    {
        kernel_tlv_replace(pTlvBuffer,0x8A,0,NULL);
    }
    else
    {

    }

    //9F39
    pNode = kernel_tlv_find(pTlvBuffer,0x9F6B);
    if(!pNode)
    {
        kernel_tlv_replace(pTlvBuffer,0x9F39,1,(pu8)"\x07");
    }
    else
    {
        kernel_tlv_replace(pTlvBuffer,0x9F39,1,(pu8)"\x06");
    }


    //POS_TAG_TRANSACTION_CONTACTLESS_MODE
    u8 Mode[2] = {0x00, 0x00};
    pNode = NULL;
    pNode = kernel_tlv_find(pTlvBuffer,0x9F06);
    if(pNode)
    {
        pValue = kernel_tlv_get_v(pNode);
        if(memcmp(pValue, "\xA0\x00\x00\x00\x04", 5) == 0 )//master
        {
           pNode = NULL;
           pNode = kernel_tlv_find(pTlvBuffer,0x9F6B);
           if(pNode != NULL && kernel_tlv_get_l(pNode) !=0 )
           {
               Mode[0] = 0x01;
           }
           else
           {
           }
        }
        //VISA CUP
        else if(memcmp(pValue, "\xA0\x00\x00\x00\x03", 5) == 0 || memcmp(pValue, "\xA0\x00\x00\x03\x33", 5) == 0)
        {
          Mode[0]  = 0x00;
        }
        //AMEX
        else if(memcmp(pValue, "\xA0\x00\x00\x00\x25", 5) == 0 )
        {
           pNode = NULL;
           pNode = kernel_tlv_find(pTlvBuffer,0x9F8221);
           if(pNode != NULL && kernel_tlv_get_l(pNode) !=0 )
           {
               Mode[0]  = 0x01;
           }
           else
           {
           }
        }
        //Discover
        else if(memcmp(pValue, "\xA0\x00\x00\x01\x52", 5) == 0
            || memcmp(pValue, "\xA0\x00\x00\x03\x24", 5) == 0 )
        {
           pNode = NULL;
           pNode = kernel_tlv_find(pTlvBuffer,0x9F8221);
           if(pNode != NULL && kernel_tlv_get_l(pNode) !=0 )
           {
               Mode[0]  = 0x01;
           }
           else
           {
           }
        }
        //JCB
        else if(memcmp(pValue, "\xA0\x00\x00\x00\x65", 5) == 0 )
        {
           pNode = NULL;
           pNode = kernel_tlv_find(pTlvBuffer,0x9F8203);
           if(pNode != NULL && kernel_tlv_get_l(pNode) !=0 )
           {
               Mode[0]  = 0x01;
           }
           else
           {
           }
        }
        else
        {
        }
    }
    kernel_tlv_replace(pTlvBuffer,POS_TAG_TRANSACTION_CONTACTLESS_MODE,1,Mode);

    pTempBuff = malloc(32);
    kernel_tlv_delete(pTlvBuffer, TAG_EMV_IFD_SERIAL_NUMBER);
    // OsGetTermSn(pTempBuff);
    Lib_ReadSn(pTempBuff,32);
    kernel_tlv_replace(pTlvBuffer, TAG_EMV_IFD_SERIAL_NUMBER, strlen((char *)pTempBuff), &pTempBuff[0]);


    free(pTempBuff);

        TRACE( "pTransactionPool_1222225555 = ");
        TRACEBUF(pTlvBuffer, kernel_tlv_get_node_l(pTlvBuffer));
}

int app_user_cbk(void *user_data, unsigned char *buf, unsigned int len)
{
    //int ret = 0;
    unsigned char *node = 0;
    unsigned char cmd = 0;

    node = kernel_tlv_find(buf, 0xDF00);
    if (node == 0)
    {
        return -1;
    }
    cmd = kernel_tlv_get_v(node)[0];

    TRACE("Emv app user cbk, CMD = %02XH", cmd);
    TRACEBUF(buf, len);

    switch (cmd)
    {
    case CMD_CVM_OFFLINE_PIN:
    case CMD_EMV_CALLBACK_CVM_OFFLINE_PIN:
        TRACE("CMD_EMV_CALLBACK_CVM_OFFLINE_PIN");
        return app_user_input_offline_pin((unsigned char *)buf);
    case CMD_CVM_ONLINE_PIN:
    case CMD_EMV_CALLBACK_CVM_ONLINE_PIN:
        return app_user_input_online_pin(cmd,buf);
        break;
    case CMD_EMV_CALLBACK_ONLINE_REQUEST: // BC
        app_user_online_request(buf, len);
        break;
    case CMD_NFC_ONLINE_REQUEST:
        // app_user_online_request(buf, len);
        tlv_batch_replace_(buf,pTransactionPool);
        ContactlessAddTagProcess(pTransactionPool);
        //memset(buf, 0, sizeof(buf));
        kernel_tlv_new(buf,POS_TAG_HEAD,0,NULL);
        unsigned char value = 0x01;
        kernel_tlv_add_child(buf,TAG_NODE_CTL_ONLINE_REQUEST_NO_NEED_HANDLE,1,&value);
        //ret = 0;
        break;
    case CMD_DISPLAY_UIRD_MSG: // D7
        app_user_nfc_disp(buf, len);
        break;
    case CMD_DISPLAY_ICC_MSG: // C5
        // app_user_icc_disp(buf, len);
        break;
    case CMD_EMV_CALLBACK_CARDHOLDER_CONFIRMATION:
        TRACE("CMD_EMV_CALLBACK_CARDHOLDER_CONFIRMATION");
        app_user_cardholder_confirm(buf, len);
        break;
    case CMD_OUT_SIGNAL_ENTRYPOINT: /* cless and quics */
        app_user_see_phone_process(buf,len);
        break;
    case CMD_EMV_CALLBACK_GET_PAN_BEFORE_ODA_CUSTOMER:
        cardconfirm(buf,len);
        break;        
    default:
        break;
    }

    return 0;
}

int Emv_KernelInit(EmvCallBack_t t_callbackfun)
{
    FUN_IN;
    static emv_kernel_alg_t ins_opt_alg;
    static emv_kernel_std_t ins_opt_std;

    ins_opt_alg.opt_rsa_public_key = app_rsa_public_key;
    ins_opt_alg.opt_sha1 = app_sha1;
    ins_opt_alg.opt_des_decrypt = app_des_decrypt;
    ins_opt_alg.opt_des_encrypt = app_des_encrypt;
    ins_opt_alg.opt_aes_decrypt = app_aes_decrypt;
    ins_opt_alg.opt_aes_encrypt = app_aes_encrypt;

    ins_opt_std.opt_memset = app_memset;
    ins_opt_std.opt_memcpy = app_memcpy;
    ins_opt_std.opt_memcmp = app_memcmp;
    ins_opt_std.opt_strlen = app_strlen;
    ins_opt_std.opt_log = app_log;

    emv_kernel_set_callback(trans_cbk, 0);
    emv_kernel_set_std(&ins_opt_std);
    emv_kernel_set_alg(&ins_opt_alg);

    Trade_EMV_AppSelect = t_callbackfun.EMV_AidSelect;
    Trade_SetKernelId = t_callbackfun.EMV_SetKernelId;
    Trade_ConfirmCardInfo = t_callbackfun.EMV_ConfirmCardInfo;
    Trade_OnlineProcess = t_callbackfun.EMV_OnlineProcess;
    Trade_PinEnter = t_callbackfun.EMV_InputPasswd;
    Trade_EMV_Dispmsg = t_callbackfun.EMV_ProcessDisp;
    Trade_AfterReadRecord = t_callbackfun.EMV_AfterReadRecord;
    Trade_AfterSelectApp = t_callbackfun.EMV_AfterSelectApp;

    return 0;
}
void EMV_data_init(ST_TRANSDATA *emvTransParams,unsigned char *pTradebuf)
{
    unsigned char *pData = NULL,*pTemp = NULL;
    pData= malloc(64);
    memset(pData,0,64);
    pTemp = malloc(64);
    memset(pTemp,0,64);
    /*9C*/
    pTemp[0] = emvTransParams->trans_type;
    kernel_tlv_add_child(pTradebuf,0x9C,1,pTemp);

    if(memcmp(emvTransParams->trans_time,"\x00\x00\x00\x00\x00\x00\x00",6) == 0)
    {
        Lib_GetDateTime(pData);
        /**/
        kernel_tlv_replace(pTradebuf, TAG_EMV_TRANSACTION_DATE,3,pData);
        /**/
        kernel_tlv_replace(pTradebuf, TAG_EMV_TRANSACTION_TIME,3,pData+3);
    }
    else
    {
        /**/
        kernel_tlv_replace(pTradebuf, TAG_EMV_TRANSACTION_DATE,3,emvTransParams->trans_time);
        /**/
        kernel_tlv_replace(pTradebuf, TAG_EMV_TRANSACTION_TIME,3,emvTransParams->trans_time+3);
    }

    /*9F02*/
    memset(pTemp,0,64);
    asc_to_bcd(emvTransParams->trans_amount,12,pTemp);
    kernel_tlv_replace(pTradebuf, TAG_EMV_AMOUNT_AUTH_NUM, 6, pTemp);

    /*9F03*/
    kernel_tlv_replace(pTradebuf, TAG_EMV_AMOUNT_OTHER_NUM, 6, (pu8)"\x00\x00\x00\x00\x00\x00");

    // /*rand*/
    memset(pTemp,0,64);
    Lib_AlgGetRandom(pTemp,4);
    kernel_tlv_replace(pTradebuf, TAG_EMV_UNPREDICTABLE_NUMBER, 4, pTemp);

    // kernel_tlv_replace(pTradebuf, TAG_EMV_TRANSACTION_CURRENCY_CODE, 2, "\x01\x70");

    free(pData);
    free(pTemp);
}

static u32 Gac2FailProcess(pu8 pTradFile)
{
    u32 uiResult = APP_RC_EMV_DENAIL;
    //u8 ret_code = 0;
    pu8 pNode = NULL;
    //pu8 pValue = NULL;
    //int Length = 0;
    pNode = kernel_tlv_find(pTradFile,TAG_EMV_TRANS_REVERSAL);
    if(pNode)
    {
        if((kernel_tlv_get_v(pNode)[0] != 0x00) && (glb_online_trans_result == true))
        {

            uiResult = APP_RC_TRANS_REVERSEL;
        }
        else
            uiResult = APP_RC_EMV_DENAIL;
    }
    else
    {
        uiResult = APP_RC_EMV_DENAIL;
    }

    return uiResult;
}
static u32 ContactEmvProcessResultProcess(u32 kernelErrCode,pu8 pTradFile)
{
    u32 uiResult = APP_RC_EMV_DENAIL;
    u8 ret_code = 0;
    pu8 pNode = NULL;
    //pu8 pValue = NULL;
    //int Length = 0;

	if(kernelErrCode != 0x00000000){
        switch (kernelErrCode)
        {
        case ERROR_EMV_INPUT_PIN_TERMINAL: //pin input cancel
        case ERROR_EMV_CARDHOLDER_CONFIRMATION_RESULT_INDEX: // select app cancel
            return APP_RC_CANCEL;
            break;
        case ERROR_EMV_SELECT_APP_CANDIDATE_EMPTY:
            glb_online_trans_result = 0;
            return APP_RC_CARD_NOT_SUPPORT;
            break;    
        default:
            if(trans_gac_time == 0x02)
                uiResult = Gac2FailProcess(pTradFile);
            else
                uiResult = APP_RC_TERMINAL;   
            glb_online_trans_result = 0;
            return uiResult;
            break;
        }
    }
    else
    {
        pNode = kernel_tlv_find(pTradFile,0x9F8102);
        if(pNode)
        {
            ret_code = kernel_tlv_get_v(pNode)[1];
            if(ret_code == OFFLINEAPPROVED || ret_code == ONLINEAPPROVED)
            {
                uiResult = APP_RC_COMPLETED;
            }
            else if(ret_code == OFFLINEDECLINED || ret_code == UNABLEONINE_OFFLINEDECLINED)
            {
                uiResult = APP_RC_EMV_DENAIL;
            }
            else if(ret_code == ONLINEDECLINED)
            {
                pNode = kernel_tlv_find(pTradFile,TAG_EMV_TRANS_REVERSAL);
                if(pNode && (kernel_tlv_get_v(pNode)[0] != 0x00))
                {
                    uiResult = APP_RC_TRANS_REVERSEL;
                }
                else
                    uiResult = APP_RC_EMV_DENAIL;
            }
            else
            {
                uiResult = APP_RC_EMV_DENAIL;
            }
        }
    }

    return uiResult;
}

static u32 ContactlessEmvProcessResultProcess(u32 kernelErrCode,pu8 pTradFile)
{
    u32 uiResult = 0;
    pu8 pNode = NULL;
    unsigned char value[2] = {0};
    u32 switch_flag = 0;
    if(uiResult != 0x00000000)
    {

        switch(uiResult)
        {
            case 0xFF000302:
                uiResult = APP_RC_NFC_TRY_AGAIN;
                break;
            case ERROR_ENTRYPOINT_PPSE_RESPONSE_SW:
                uiResult = APP_RC_EMV_TRANS_TRY_ANOTHER_INTERFACE;
                break;     
            default:
                if(glb_emv_kernel_process_status == KERNEL_PROCESS_STATUS_NFC_RETAP_CANCLE)
                    uiResult = APP_RC_NFC_RETAP_CANCEL;
                else if(glb_emv_kernel_process_status == KERNEL_PROCESS_STATUS_NFC_RETAP_TIMEOUT)
                    uiResult = APP_RC_NFC_RETAP_TIMEOUT;
                else    
                    uiResult = APP_RC_NFC_TERMINAL;
                break;   
        }
    }
    else{
        pNode = kernel_tlv_find(pTradFile,0xDF8129);
        if(pNode)
        {
            value[0] = kernel_tlv_get_v(pNode)[0];
            switch(value[0])
            {
                case CONTACTLESS_OPS_STATUS_TRY_AGAIN:
                    uiResult = APP_RC_NFC_TRY_AGAIN;
                    break;
                case CONTACTLESS_OPS_STATUS_END_APPLICATION:
                    pNode = kernel_tlv_find(pTradFile,0x9C);
                    if(pNode && kernel_tlv_get_v(pNode)[0] == '\x20'){//refund
                            switch_flag = 1;
                            uiResult = APP_RC_COMPLETED;
                    }
                    if(!switch_flag){
                        uiResult = APP_RC_NFC_TERMINAL;
                        return uiResult;
                    }  
                    break;
                case CONTACTLESS_OPS_STATUS_APPROVED:
                case CONTACTLESS_OPS_STATUS_ONLINE_REQUEST:
                    uiResult = APP_RC_COMPLETED;
                    break;
                case CONTACTLESS_OPS_STATUS_DECLINED:
                    uiResult = APP_RC_EMV_DENAIL;
                    break;    
                case CONTACTLESS_OPS_STATUS_TRY_ANOTHER_INTERFACE:
                    uiResult = APP_RC_EMV_TRANS_TRY_ANOTHER_INTERFACE;
                    break;
                default:
                    uiResult = APP_RC_NFC_TERMINAL;
                    break;        
            }  
        }
        else
        {
            uiResult = APP_RC_NFC_TERMINAL;
        }    
    }
 
    return uiResult;
}
EMV_L2_Return Emv_Process(ST_TRANSDATA *pstTransData)
{
    int ret;
    u32 uiResult;
    int tradeBufferLen;
    pu8 pTradeBuffer = NULL;
    
    if(pstTransData == NULL)
        return APP_RC_TERMINAL;

    if(pTransactionPool != NULL)
    {
        free(pTransactionPool);
        pTransactionPool = NULL;
    }    
    glb_online_trans_result = false;
    glb_emv_kernel_process_status = PROCESS_STATUS_INIT;
    // Inslib_last_offline_pin_flag = false;

    pTransactionPool = malloc(2048);
    memset(pTransactionPool,0,2048);
    kernel_tlv_new(pTransactionPool,POS_TAG_HEAD,0,NULL);

    pTradeBuffer = malloc(2048);
    memset(pTradeBuffer,0,2048);
    tradeBufferLen = kernel_tlv_new(pTradeBuffer,0xFF00,0,NULL);

    s_emv_card_mode = pstTransData->cardDetectType;

    EMV_data_init(pstTransData,pTradeBuffer);
    tradeBufferLen = kernel_tlv_get_node_l(pTradeBuffer);
    disp_reading_card();
    TRACE("TradeBuffer:");
    TRACEBUF(pTradeBuffer, tradeBufferLen);
    ret = emv_kernel_start(pTradeBuffer,tradeBufferLen);

    if(ret >= 0)
    {
        uiResult = 0;
        EMV_L2_LastError = 0;
    }
    else
    {
        uiResult = emv_kernel_error();
        EMV_L2_LastError = uiResult;
    }
    tlv_batch_replace_(pTradeBuffer,pTransactionPool);

    TRACE("Kernel Result: %08x",uiResult);
    TRACEBUF(pTransactionPool,kernel_tlv_get_node_l(pTransactionPool));

    if(s_emv_card_mode == NFC_DETECETED)
        uiResult = ContactlessEmvProcessResultProcess(uiResult,pTransactionPool);
    else
        uiResult = ContactEmvProcessResultProcess(uiResult,pTransactionPool);    

    free(pTradeBuffer);
    
    return uiResult;
}

static unsigned char tagValue[256] = {0};
unsigned char* Emv_GetCoreData(unsigned int tagname, int *pvallen)
{
    //u32 len=0;
    pu8 pNode=NULL;
    T_U8_VIEW uvTag;
    memset(tagValue,0,sizeof(tagValue));
    uvTag.head = tagValue;
    uvTag.len = 0;

    if(pTransactionPool == NULL)
        return 0;

    pNode = kernel_tlv_find(pTransactionPool,tagname);
	if(pNode != NULL && kernel_tlv_get_l(pNode) != 0)
    {
	    *pvallen=kernel_tlv_get_l(pNode);
        return kernel_tlv_get_v(pNode);
	}
    else
    {
        if(s_emv_card_mode == SCI_DETECETED)
        {
            if(emv_kernel_get_contact_element(tagname, uvTag.head, &uvTag.len)==0)
            {
                if(uvTag.len != 0)
                {
                    *pvallen = uvTag.len;
                    return uvTag.head;
                }
            }
            else
            {
                TRACE( " TAG %X not found \r\n", tagname);
            }
        }
        else  if(s_emv_card_mode == NFC_DETECETED)
        {
            if(emv_kernel_get_contactless_element(tagname, uvTag.head, &uvTag.len)==0)
            {
                if(uvTag.len != 0)
                {
                    *pvallen = uvTag.len;
                    return uvTag.head;
                }
            }
            else
            {
                TRACE( "TAG %X not found  \r\n", tagname);
            }
        }
    }
    *pvallen  = 0;
    return NULL;
}

int Emv_SetCoreData(unsigned int tagname, unsigned char *pvalue, int valuelen)
{
    if(s_emv_card_mode == SCI_DETECETED)
        return emv_kernel_set_contact_element(tagname,valuelen,pvalue);
    else if(s_emv_card_mode == NFC_DETECETED)
        return emv_kernel_set_contactless_element(tagname,valuelen,pvalue);    
    else
        return -1;    
}

//获取tagname[]里的一系列TLV数据,返回的数据格式为TLV
int Emv_FetchData(unsigned int* tagname, int count, unsigned char* obuf, int olen)
{
    //pu8 pNode = NULL;
    T_U8_VIEW uvTag;
    
    memset(tagValue,0,sizeof(tagValue));
    uvTag.head = tagValue;
    uvTag.len = 0;

    u8 *pTlvDe55 = malloc(256);
	if(pTlvDe55 == NULL)
	    return 0;
	memset(pTlvDe55,0,256);
	kernel_tlv_new(pTlvDe55, 0xFF00, 0, NULL);
	u8 i;
	for(i = 0; i < count; i++)
	{
		uvTag = get_tlv_view(pTransactionPool, tagname[i]);
		if(UV_OK(uvTag))
			set_tlv_view(pTlvDe55, tagname[i], uvTag);       
		else
        {
            memset(tagValue,0,sizeof(tagValue));
            uvTag.head = tagValue;
            uvTag.len = 0;
            if(s_emv_card_mode == SCI_DETECETED)
            {

                if(emv_kernel_get_contact_element(tagname[i], uvTag.head, &uvTag.len)==0)
                {
                    if(uvTag.len != 0)
                    {
                        set_tlv_view(pTlvDe55, tagname[i], uvTag);
                    }
                }
                else
                {
                    TRACE(" TAG %X NOT found in %s\r\n", tagname[i], __FUNCTION__);
                }
            }
            else  if(s_emv_card_mode == NFC_DETECETED)
            {
                if(emv_kernel_get_contactless_element(tagname[i], uvTag.head, &uvTag.len)==0)
                {
                  if(uvTag.len != 0)
                    {
                        set_tlv_view(pTlvDe55, tagname[i], uvTag);
                    }
                }
                else
                {
                    TRACE( " TAG %X not found in %s %d\r\n", tagname, __FUNCTION__,__LINE__);
                }
            }
        }         
	}

	T_U8_VIEW uvDe55 = {kernel_tlv_get_v(pTlvDe55), kernel_tlv_get_l(pTlvDe55)};

    if(uvDe55.len > olen)
    {
        free(pTlvDe55);
        pTlvDe55 =NULL;
        return 0;
    }

    memcpy(obuf,uvDe55.head,uvDe55.len);

	free(pTlvDe55);
    pTlvDe55 =NULL;
    return uvDe55.len;
}

int Emv_SetContactlessOnlineResult(EmvOnlineData_t* pOnlineData)
{
    FUN_IN;
    //u8 buff[32] = {0};
    u32 uiResult;
    int ret;
    pu8 onlineData = NULL;
    pu8 tempBuffer = NULL;
    int onlineDataLen = 0;
    //int tempBufferLen = 0;
    pu8 pNode = NULL;
    //pu8 pValue = NULL;
    EmvOnlineData_t insEmvOnlineData;
    if(pOnlineData)
    {
        memcpy(insEmvOnlineData.iccResponse,pOnlineData->iccResponse,strlen(pOnlineData->iccResponse));
        memcpy(insEmvOnlineData.ackdata,pOnlineData->ackdata,pOnlineData->ackdatalen);
    }
    else
    {
        return -1;
    }
    if(s_emv_card_mode == SCI_DETECETED)
        return 0;

    tempBuffer = malloc(1024);
    memset(tempBuffer,0,1024);
    onlineData = malloc(1024);
    memset(onlineData,0,1024);
    kernel_tlv_new(onlineData,POS_TAG_HEAD,0,NULL);
    TRACE("pTransactionPool:");
    TRACEBUF(pTransactionPool,kernel_tlv_get_node_l(pTransactionPool));
    pNode = kernel_tlv_find(pTransactionPool,0x9F06);
    if(pNode != NULL && kernel_tlv_get_l(pNode) > 0)
    {
        kernel_tlv_add_child(onlineData,0x9F06,kernel_tlv_get_l(pNode),kernel_tlv_get_v(pNode));
    }

    if(strlen(pOnlineData->iccResponse) > 0)
    {
        kernel_tlv_add_child(onlineData,0x8A,2,(unsigned char *)pOnlineData->iccResponse);
    }

    if(pOnlineData->ackdatalen > 0)
    {
       kernel_tlv_new(tempBuffer,0x7F00,pOnlineData->ackdatalen,pOnlineData->ackdata);
       tlv_batch_replace_(tempBuffer,onlineData);   
    }


    kernel_tlv_add_child(onlineData,0x9F872B,1,(pu8)"\x10");

    onlineDataLen = kernel_tlv_get_node_l(onlineData);
    TRACE( "double tap:");
    TRACEBUF(onlineData,onlineDataLen);

    disp_nfc_retap();
    ret = nfc_try_again();
    if(ret != 0)
    {
        return -1;
    }

    return 0;
    ret = emv_kernel_start(onlineData, onlineDataLen); 
    tlv_batch_replace_(onlineData,pTransactionPool);
    if(ret >= 0)
    {
        uiResult = 0;
        EMV_L2_LastError = 0;

    }
    else
    {
        uiResult = emv_kernel_error();
        EMV_L2_LastError = uiResult;
    }

    #ifdef CFG_LIB_DEBUG
    OsLog(LOG_ERROR, "double tap error code = %08XH", uiResult);
    onlineDataLen = kernel_tlv_get_node_l(onlineData);
    app_trace_value(onlineData,onlineDataLen);
    #endif

    if(uiResult != 0x00000000)
    {
        switch(uiResult)
        {
            case 0xFF000302:
                uiResult = APP_RC_NFC_TRY_AGAIN;
                break;
            case ERROR_ENTRYPOINT_PPSE_RESPONSE_SW:
                uiResult = APP_RC_EMV_TRANS_TRY_ANOTHER_INTERFACE;
                break;     
            default:
                if(glb_emv_kernel_process_status == KERNEL_PROCESS_STATUS_NFC_RETAP_CANCLE)
                    uiResult = APP_RC_NFC_RETAP_CANCEL;
                else if(glb_emv_kernel_process_status == KERNEL_PROCESS_STATUS_NFC_RETAP_TIMEOUT)
                    uiResult = APP_RC_NFC_RETAP_TIMEOUT;
                else    
                    uiResult = APP_RC_NFC_TERMINAL;
                break;   
        }
        goto EXIT;
    }
    /*判断交易结果*/
    pNode = kernel_tlv_find(onlineData,0xDF8129);
    if(pNode)
    {
        switch(kernel_tlv_get_v(pNode)[0])
        {
            case CONTACTLESS_OPS_STATUS_TRY_AGAIN:
                uiResult = APP_RC_NFC_TRY_AGAIN;
                break;
            case CONTACTLESS_OPS_STATUS_END_APPLICATION:
                uiResult = APP_RC_NFC_TERMINAL;
                break;
            case CONTACTLESS_OPS_STATUS_APPROVED:
            case CONTACTLESS_OPS_STATUS_ONLINE_REQUEST:
                uiResult = APP_RC_COMPLETED;
                break;
            case CONTACTLESS_OPS_STATUS_DECLINED:
                uiResult = APP_RC_EMV_DENAIL;
                break;    
            case CONTACTLESS_OPS_STATUS_TRY_ANOTHER_INTERFACE:
                uiResult = APP_RC_EMV_TRANS_TRY_ANOTHER_INTERFACE;
                break;
            default:
                uiResult = APP_RC_NFC_TERMINAL;
                break;        
        }  
    }
    else
    {
        uiResult = APP_RC_NFC_TERMINAL;
    }
EXIT:

    free(tempBuffer);
    free(onlineData);

    if(uiResult == APP_RC_COMPLETED)
    {
        ret = 0;
    }
    else
    {
        ret = -1;
    }
    return ret;
}


#define RUPAY_AID	"\xA0\x00\x00\x05\x24\x10\x10"   //EMVTAG_AID
/* RuPay CSU  */
static const unsigned char CSU_VALUE_1[4] = {0x80, 0x80, 0x80, 0x00};
static const unsigned char CSU_VALUE_2[4] = {0x80, 0x80, 0x40, 0x00};

int Emv_CheckContactlessDoubleTap(EmvOnlineData_t* pOnlineData)
{
    FUN_IN;
    int iRet = 0;
	int length =0;
	unsigned char *pNode =NULL;
	unsigned char IssuerData[128] = {0};
	unsigned char tlvData[512] = {0};

    TRACE( "pTransactionPool:");
    TRACEBUF(pTransactionPool,kernel_tlv_get_node_l(pTransactionPool));

	do
	{
		if(pOnlineData->ackdatalen == 0)
			break;
        pNode = kernel_tlv_find(pTransactionPool,0x9F06);
        if(pNode == NULL || kernel_tlv_get_l(pNode) == 0)
            break;
		if(memcmp(kernel_tlv_get_v(pNode),RUPAY_AID,7) != 0)
			break;	
		pNode = NULL;
		length = 0;

		kernel_tlv_new(tlvData,0x7F00,pOnlineData->ackdatalen,pOnlineData->ackdata);

        TRACE("tlvData:");
        TRACEBUF(tlvData,kernel_tlv_get_node_l(tlvData));

		pNode = kernel_tlv_find(tlvData,0x91);
		if(pNode == NULL)
			break;
		length = kernel_tlv_get_l(pNode);
		memcpy(IssuerData,kernel_tlv_get_v(pNode),length);
		for (size_t i = 0; i + 4 <= length; i++) 
		{
			uint8_t b3 = IssuerData[i + 2];
			if ((b3 & 0xC0) == 0x40 || (b3 & 0xC0) == 0x80) 
			{
				if (memcmp(&IssuerData[i], CSU_VALUE_1, 4) == 0 ||	memcmp(&IssuerData[i], CSU_VALUE_2, 4) == 0) 
				{
					iRet = 1;
					break;
				}
			}
		}

	} while (0);
	
	return iRet;
}