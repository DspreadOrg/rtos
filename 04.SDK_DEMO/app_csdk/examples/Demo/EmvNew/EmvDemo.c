/**********************************************************************************************************
 * Copyright(C)2018,
 * File Name     : EmvDemo.c
 * Version       : v1.0
 * Author        :
 * Description   : EMV application sample apis
 **********************************************************************************************************/
/**************************** INCLUDE FILES BEGIN **************************/

#include "EmvDemo.h"
#include "key.h"
#include "Ui.h"
#include "Common.h"
#include <math.h>
#include "LibDukpt.h"
#include "card.h"
#include "VAN_security.h"
#include "MessageBox.h"
#include "Idle.h"
#include "Trade.h"
#include "LanguageEN.h"
#include "Communication.h"
#include "CardTrade.h"

#include "EmvUtils.h"

extern ST_MSC_TRACK astCiTrack[TRACK_NUM];

static unsigned char g_aucPAN[20];
static unsigned char g_ucPANLen;
static unsigned char aucMacPanAscii[30];

#define MSC_TMP_BUF_LEN 1000 //

ST_PARAMETER_CONFIG_LIST stParConlist;
ST_PARAMETER_TRANSACTION_PRESET stTrasactionSet;

unsigned char g_aucTradeMsgBuf[1024];
unsigned short g_usTradeMsgLen = 0;

static unsigned char g_aucPAN[20];
static unsigned char g_ucPANLen;
// static unsigned char aucMacPanAscii[30];

static u8 GetCardType(unsigned char *pucPan)
{
    int iBin = 0;
    unsigned char ucBuf[8] = {0};
    unsigned char ucCardType = 0;

    FUN_IN;
    memcpy(ucBuf, pucPan, 8);
    iBin = atoi((char *)ucBuf);
    TRACE("Bin = %d", iBin);

    if ((40000000 <= iBin) && (iBin <= 49999999))
    {
        ucCardType = CARD_VISA;
    }
    else if (((51000000 <= iBin) && (iBin <= 55999999)) ||
             ((22210000 <= iBin) && (iBin <= 27209999)) ||
             ((58276600 <= iBin) && (iBin <= 58276699)))
    {
        ucCardType = CARD_MASTERCARD;
    }
    else if (((37000000 <= iBin) && (iBin <= 37999999)) ||
             ((34000000 <= iBin) && (iBin <= 34999999)))
    {
        ucCardType = CARD_AMEX;
    }
    else if (((30000000 <= iBin) && (iBin <= 30599999)) ||
             ((30950000 <= iBin) && (iBin <= 30959999)) ||
             ((38000000 <= iBin) && (iBin <= 39999999)))
    {
        ucCardType = CARD_DINERS;
    }
    else if ((35280000 <= iBin) && (iBin <= 35899999))
    {
        ucCardType = CARD_JCB;
    }
    else if (((60110000 <= iBin) && (iBin <= 60110999)) ||
             ((60112000 <= iBin) && (iBin <= 60114999)) ||
             ((60117400 <= iBin) && (iBin <= 60117499)) ||
             ((60117700 <= iBin) && (iBin <= 60117999)) ||
             ((60118600 <= iBin) && (iBin <= 60119999)) ||
             ((62212600 <= iBin) && (iBin <= 62292599)) ||
             ((62400000 <= iBin) && (iBin <= 62699999)) ||
             ((62820000 <= iBin) && (iBin <= 62889999)) ||
             ((64400000 <= iBin) && (iBin <= 65999999)) ||
             ((6210940 <= iBin) && (iBin <= 6210949)))
    {
        ucCardType = CARD_DISCOVER;
    }
    return ucCardType;
}

// card transaction demo
short CardDemo(short cardMode, ST_TRANSDATA *pstTransData)
{
    short errCode;

    switch (cardMode)
    {
    case SCI_DETECETED: // Contact card
    case NFC_DETECETED: // Contacless card
        TRACE("Start EMV Demo!!!\r\n");
        errCode = EmvDemo(cardMode, pstTransData);
        break;

    case MSC_DETECETED:
        // Magnetic stripe cards
        TRACE("Start Mcc Demo!!!\r\n");
        errCode = MccDemo(pstTransData);
        break;

    default:
        // Invalid card
        errCode = 0x8001;
        break;
    }

    TRACE("errCode %x\r\n", errCode);
    return errCode;
}

static signed short searchChar(unsigned char *pBuf, unsigned char ch, signed short startpos)
{
    signed short pos = startpos;

    if (startpos < 0)
    {
        return -1;
    }

    while (*(pBuf + pos) != 0)
    {
        if (*(pBuf + pos) == ch)
        {
            return pos;
        }
        pos++;
    }
    return -1;
}

// service code
static u8 GetCardSevCode(unsigned char *pucTrackBuf, unsigned char *pucCardSevCode, unsigned char *pucCardSevCodeLen)
{
    signed short sOffSet1;
    unsigned char *pucTrack2Buf;
    unsigned char ucLen;

    pucTrack2Buf = pucTrackBuf;

    // Find the position of the equal sign
    sOffSet1 = searchChar(pucTrack2Buf, '=', 0);
    if (sOffSet1 < 0)
    {
        return FALSE;
    }

    //+1, starting position of validity period
    sOffSet1++;

    //+4, starting position of the service code
    sOffSet1 += 4;

    Lib_AscNum2bcd(pucTrack2Buf + sOffSet1, pucCardSevCode, 3, &ucLen, 0x0f);
    *pucCardSevCodeLen = ucLen;
    //    Printf_STR_BUF("Card Service Code: ", pucCardSevCode, *pucCardSevCodeLen);
    return TRUE;
}

static int app_user_online_request(unsigned char *buf, unsigned int len)
{
    unsigned char host_rsp[64];
    unsigned int host_rsp_len = 0;

    kernel_tlv_new(host_rsp, 0x7F00, 0, 0);
    host_rsp_len =
        kernel_tlv_add_child(host_rsp, 0x8A, 2, (unsigned char *)"\x30\x30");

    memcpy(buf, host_rsp, host_rsp_len);

    return 0;
}

static int app_user_nfc_disp(unsigned char *buf, unsigned int len)
{
    unsigned char *node = 0;
    unsigned char *value = 0;
    char cStr[32];

    user_interface_request_data_t UIRD;
    memset(&UIRD, 0, sizeof(UIRD));

    node = kernel_tlv_find(buf, 0xDF8116);
    if (node)
    {
        value = kernel_tlv_get_v(node);
        memcpy(&UIRD, (user_interface_request_data_t *)value, sizeof(user_interface_request_data_t));
    }

    TRACE("Emv app user nfc display id = %02XH", UIRD.ucMessageIdentifier);

    UIClearRect(0, 1, LCD_MAX_SEG, MAX_COM_LINES - 1);

    switch (UIRD.ucMessageIdentifier)
    {
    case CONTACTLESS_UIRD_MessageID_PRESENT_CARD:
        UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "insert/swipe/present card");
        break;
    case CONTACTLESS_UIRD_MessageID_PROCESSING:
        UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "Processing...");
        break;
    case CONTACTLESS_UIRD_MessageID_AUTHORISING_PLEASEWATI:
        UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "Authorising Please Wait...");
        break;
    case CONTACTLESS_UIRD_MessageID_ERROR_OTHER_CARD:
        UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "TERMINATE");
        // OsBeep(7, 500);
        break;
    case CONTACTLESS_UIRD_MessageID_DECLINED:
        UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "Decline");
        // OsBeep(7, 500);
        break;
    case CONTACTLESS_UIRD_MessageID_APPROVED:
        UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, "Approve");
        // OsBeep(7, 500);
        break;
    default:
        memset(cStr, 0, sizeof(cStr));
        sprintf(cStr, "Unknown %02XH", UIRD.ucMessageIdentifier);
        UITextAlign(ALIGN_M, 1, DISP_MODE_NORMAL, cStr);
        // OsBeep(7, 1000);
        break;
    }

    return 0;
}

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

int app_user_cbk(void *user_data, unsigned char *buf, unsigned int len)
{
    // int ret = 0;
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

        // TransView_vShowLine(2,EM_DTYPE_NORMAL,EM_ALIGN_CENTER,(char*)"pls input offline pin");
        // OsBeep(3, 500);
        // break;
        return -1;
    case CMD_CVM_ONLINE_PIN:
    case CMD_EMV_CALLBACK_CVM_ONLINE_PIN:
        // TransView_vShowLine(2,EM_DTYPE_NORMAL,EM_ALIGN_CENTER,(char*)"pls input online pin");
        // OsBeep(3, 500);
        //  memset(buf, 0, sizeof(buf));
        //  memcpy(buf, "\x7F\x00\x0A\x99\x08\xCE\x6E\xB6\xA5\x43\x65\x96\xF6", 13);
        // break;
        return -1;
    case CMD_EMV_CALLBACK_ONLINE_REQUEST: // BC
        app_user_online_request(buf, len);
        break;
    case CMD_NFC_ONLINE_REQUEST:
        app_user_online_request(buf, len);
        break;
    case CMD_DISPLAY_UIRD_MSG: // D7
        app_user_nfc_disp(buf, len);
        break;
    case CMD_DISPLAY_ICC_MSG: // C5
        app_user_icc_disp(buf, len);
        break;
    case CMD_EMV_CALLBACK_CARDHOLDER_CONFIRMATION:
        // app_user_cardholder_confirm(buf, len);
        break;
    default:
        break;
    }

    return 0;
}
#define FILE_NAME_EMV_APP "emv_app.bin"
#define FILE_NAME_EMV_CAPK "emv_capk.bin"

#define EMV_APP_PAGE_SIZE (384)
#define EMV_CAPK_PAGE_SIZE (384)

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

        pNode = kernel_tlv_find(tempBuf, 0x9F06);
        if (pNode == 0)
        {
            continue;
        }
        pValue = kernel_tlv_get_v(pNode);
        kernel_tlv_get_l(pNode);
        TRACEBUF(pValue, 5);
        if (memcmp(aid, pValue, 5) != 0)
        {
            continue;
        }

        pNode = kernel_tlv_find(tempBuf, 0x9F22);
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

    node = kernel_tlv_find(tempBuf, 0xDF02);
    if (node == 0)
    {
        return -1;
    }
    cbk_buf_len = kernel_tlv_add_child(cbk_buf, 0xDF8201, kernel_tlv_get_l(node),
                                       kernel_tlv_get_v(node));

    node = kernel_tlv_find(tempBuf, 0xDF04);
    if (node == 0)
    {
        return -1;
    }
    cbk_buf_len = kernel_tlv_add_child(cbk_buf, 0xDF8202, kernel_tlv_get_l(node),
                                       kernel_tlv_get_v(node));

    node = kernel_tlv_find(tempBuf, 0xDF05);
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

static short s_emv_card_mode = -1;
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

    ret = Lib_IccCommand(ICC_SLOT_EMV_IC, (unsigned char *)pValue, length,
                         aucResp, &usRespLen);

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
    case CMD_EMV_CALLBACK_GET_PAN_BEFORE_ODA_CUSTOMER:
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
    case CMD_OUT_SIGNAL_ENTRYPOINT: /* cless and quics */
        // trans_cbk_entrypoint_signal(cbk_buf, cbk_buf_len);
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
    default:
        app_user_cbk(NULL, cbk_buf, cbk_buf_len);
        break;
    }
    return ret;
}

void EmvKernelInit()
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
}

short EmvDemo(short cardMode, ST_TRANSDATA *pstTransData)
{
    FUN_IN;
    unsigned char trans_data[2048] = {0};
    unsigned int trans_data_len = 0;

    memcpy(trans_data, (unsigned char *)"\xFF\x00\x3D\x9F\x02\x06\x00\x00\x08\x88\x88\x00\x9F\x03\x06\x00\x00\x00\x00\x00\x00\x81\x04\x00\x00\x00\x00\x9F\x04\x04\x00\x00\x00\x00\x5F\x2A\x02\x01\x70\x9C\x01\x00\x9A\x03\x24\x01\x18\x9F\x21\x03\x01\x20\x47\x9F\x37\x04\x93\x64\xFA\x26\xDF\x30\x01\x01", 64); // DF30, card type: 1,nfc; 2,ic; 3,mag; 0,unknow
    trans_data_len = 64;

    EmvKernelInit();
    s_emv_card_mode = cardMode;
    int ret = emv_kernel_start(trans_data, trans_data_len);
    if (ret == -1)
    {
        ret = emv_kernel_error();
        TRACE("EMV kernel start fail, ret = %d", ret);
        return ret;
    }
    else
    {
        trans_data_len = ret;

        TRACE("EMV kernel  start end, trans len = %d", trans_data_len);
        TRACEBUF(trans_data, trans_data_len);
    }

    const char *str_version = emv_kernel_get_version();
    TRACE("EMV get version: %s", str_version);

    return RET_OK;
}

// valide data
u8 GetCardValidity(unsigned char *pucTrackBuf, unsigned char *pucCardValidity, unsigned char *pucCardValidityLen)
{
    signed short sOffSet1;
    unsigned char *pucTrack2Buf;
    unsigned char ucLen;

    pucTrack2Buf = pucTrackBuf;

    // Find the position of the equal sign
    sOffSet1 = searchChar(pucTrack2Buf, '=', 0);
    if (sOffSet1 < 0)
    {
        return FALSE;
    }

    //+1, Starting bit of validity period
    sOffSet1++;

    Lib_AscNum2bcd(pucTrack2Buf + sOffSet1, pucCardValidity, 4, &ucLen, 0x0f);
    *pucCardValidityLen = ucLen;
    // Printf_STR_BUF("Card validity period : ", pucCardValidity, *pucCardValidityLen);
    return TRUE;
}

// PAN
static void GetPANfromTrackData(unsigned char *pucTrackBuf, unsigned char *pucCardId, unsigned char *pucCardIdLen)
{
    signed short sOffSet1, sOffSet2;
    unsigned char *pucTrack2Buf;

    pucTrack2Buf = pucTrackBuf;

    // Find the location of the ';' semicolon
    sOffSet1 = searchChar(pucTrack2Buf, ';', 0);
    if (sOffSet1 < 0)
    {
        return;
    }

    // Find the position of the equal sign
    sOffSet2 = searchChar(pucTrack2Buf, '=', sOffSet1);
    if (sOffSet2 < 0)
    {
        return;
    }

    // +1, starting position of the main account
    sOffSet1++;
    // The length of the main account
    sOffSet2 -= sOffSet1;

    if (sOffSet2 < 13 || sOffSet2 > 19)
    {
        // The length of the main account cannot be less than 13 and cannot be greater than 19
        return;
    }

    // As calculating pinblock, pan is used, so the PAN character is stored in the global variable
    memcpy(g_aucPAN, pucTrack2Buf + sOffSet1, sOffSet2);
    g_ucPANLen = sOffSet2;

    memcpy(aucMacPanAscii + 1, g_aucPAN, g_ucPANLen);
    aucMacPanAscii[0] = g_ucPANLen;
    //    Printf_STR_BUF("aucMacPanAscii : ", aucMacPanAscii, aucMacPanAscii[0] + 1);

    // CUT PAN ,change to bcd, if PAN is not even, the last digit will be filled with an F
    Lib_AscNum2bcd(pucTrack2Buf + sOffSet1, pucCardId, sOffSet2, pucCardIdLen, 0x0f);
    // Printf_STR_BUF("Main account: ", pucCardId, *pucCardIdLen);
}

//-----------------------------------------------------------------------------
// Name    : Track1Process
// Func    : 1th track data processing
// Paras   : pucSource[in],         Source data
//         : ucSourceLen[in],       Source data length
//         : pucDest[out],          Destination Data
//         : pucDestLen[out],       Destination Data length
//         : ucProcess[in],         Magnetic track data processing method
// Ret     : - 0 , success
//           - <0, failure
// Desc    :
//-----------------------------------------------------------------------------
void Track1Process(u8 *pucSource, u8 ucSourceLen, u8 *pucDest, u8 *pucDestLen, u8 ucProcess)
{
    u8 aucBuf[128], ucLen = 0;
    u8 ucKeyIndex;
    if (g_ucSubAlgFlag == ALGORITHM_DUKPT) // DUKPT
    {
        ucKeyIndex = DUKPT_WK_TDK_REQUIRE;
    }
    else
    {
        ucKeyIndex = stParConlist.aucResver[2];
    }

    TRACE("Data length of track 1 : %d\r\n", ucSourceLen);
    TRACEBUF(pucSource, ucSourceLen);

    // 1th track data processing
    switch (ucProcess)
    {
    case 1:
        // plaintext
        memcpy(pucDest, pucSource, ucSourceLen);
        *pucDestLen = ucSourceLen;
        return;
        // break;

    case 2:
        // padded with 00 to a multiple of 8
        memcpy(aucBuf, pucSource, ucSourceLen);
        ucLen = ucSourceLen;
        while (ucLen % 8)
        {
            aucBuf[ucLen++] = 0x00;
        }
        break;

    default:
        // plaintext
        memcpy(pucDest, pucSource, ucSourceLen);
        *pucDestLen = ucSourceLen;
        return;
        // break;
    }
    TRACE("Data length of track 1 after padding : %d", ucLen);
    TRACEBUF(aucBuf, ucLen);

    if (0 != DataEncryptProcess(aucBuf, pucDest, ucLen, ucKeyIndex))
    {
        memcpy(pucDest, aucBuf, ucLen);
    }
    *pucDestLen = ucLen;
    TRACE("Data length of track 1 after encryption : %d\r\n", *pucDestLen);
    TRACEBUF(pucDest, *pucDestLen);

    return;
}

short InputPIN(ST_TRANSDATA *pstTransData)
{
    unsigned char ucKeyDown = KB_NULL;
    unsigned char ascPin[12] = {0}, ascPinLen = 0;
    unsigned int uiTimeout = 0; //, ulWaitTime;
    unsigned char aucInput[13] = {0};

    FUN_IN;
    UIClearRect(0, 2, LCD_MAX_SEG, MAX_COM_LINES - 2);

    UITextAlign(ALIGN_L, 2, DISP_MODE_NORMAL, TIP_ENTER_PIN);

    uiTimeout = Lib_SysGetTimer() + 60000; // timeout 60S
    while (Lib_SysGetTimer() <= uiTimeout)
    {
        // IWDG_ReloadCounter();
        ucKeyDown = KbReadKey();
        if (ucKeyDown == KB_NULL)
        {
            continue;
        }
        BeepOn();
        TRACE("%s:KeyScan = %x\r\n", __func__, ucKeyDown);
        if ((KB_DIGIT_ZERO <= ucKeyDown) && (ucKeyDown <= KB_DIGIT_NINE))
        {
            if (ascPinLen < 12)
            {
                ascPin[ascPinLen++] = ucKeyDown;
                aucInput[ascPinLen - 1] = '*';
                aucInput[ascPinLen] = '\0';
                // SdkLcdDisStr(6 + ascPinLen, 2, 0, "*");
                UIText(14, 3, DISP_MODE_REVERSE, (void *)aucInput);
            }
            else
            {
                BeepOn();
                Lib_DelayMs(20);
                BeepOn();
            }
        }
        else if (ucKeyDown == KB_CLEAR)
        {
            if (ascPinLen > 0)
            {
                memset(ascPin, 0x00, sizeof(ascPin));
                ascPinLen = 0;
                aucInput[ascPinLen] = '\0';
                // SdkLcdDisStr(0, 2, 1, " ");
                UIText(14, 3, DISP_MODE_REVERSE, (void *)aucInput);
            }
            else
            {
                BeepOn();
                Lib_DelayMs(20);
                BeepOn();
            }
        }
        else if (ucKeyDown == KB_ENTER)
        {
            // cannot less than 4 digits
            if (ascPinLen < 4)
            {
                BeepOn();
                Lib_DelayMs(20);
                BeepOn();
                continue;
            }
            else
            {
                // SetPinblock(ascPin, ascPinLen, pstTransData);
                return RET_OK;
            }
        }
        else if (ucKeyDown == KB_CANCEL)
        {
            // cancel
            return UI_INPUT_CANCEL;
        }
        else
        {
            BeepOn();
        }
        TRACE("ASC PassWord : ");
        TRACE((void *)ascPin, ascPinLen);
    }

    return UI_INPUT_TIMEOUT;
}

short MccDemo(ST_TRANSDATA *pstTransData)
{
    unsigned char aucBuf[512];
    unsigned char ucLen = 0, ucTmpLen = 0;
    // unsigned char aucPanBuf[24];
    short errCode = 0;
    u8 aucTmpBuf[SDK_TRACK_NUM][MSC_TMP_BUF_LEN];
    ST_CARDINFO *pstCardInfo = NULL;
    short sRet = 0;
    FUN_IN;

    pstCardInfo = &pstTransData->stCardInfo;

    signed short sOffSet1 = 0, sOffSet2 = 0;

    if (!astCiTrack[SDK_TRACK2].ucTrackValid)
    {
        errCode = 0x8004;
        return errCode;
    }

    // DO_NEXT:
    TRACE("TRACK2 length = %d, TRACK3 length = %d\r\n", astCiTrack[SDK_TRACK2].ucTrackLen, astCiTrack[SDK_TRACK3].ucTrackLen);
    TRACE("TRACK2 = %s, TRACK3 = %s\r\n", astCiTrack[SDK_TRACK2].pucTrackBuf, astCiTrack[SDK_TRACK3].pucTrackBuf);

    pstCardInfo->eEntryMode = MODE_MSC;
    memset(aucBuf, 0, sizeof(aucBuf));
    // get PAN
    GetPANfromTrackData(astCiTrack[SDK_TRACK2].pucTrackBuf, aucBuf, &ucLen);
    // PanProcess(aucBuf, ucLen, aucPanBuf, &ucTmpLen, 0);
    Lib_Bcd2Asc(aucBuf, (char *)pstCardInfo->aucCardNo, ucLen);
    pstCardInfo->aucCardNo[ucLen * 2] = '\0';
    TRACE("CardNo: %s", pstCardInfo->aucCardNo);

    // remove padding F
    while (pstCardInfo->aucCardNo[strlen((const char *)pstCardInfo->aucCardNo) - 1] > '9')
    {
        pstCardInfo->aucCardNo[strlen((const char *)pstCardInfo->aucCardNo) - 1] = '\0';
    }
    pstCardInfo->ucCardLen = strlen((char *)pstCardInfo->aucCardNo);
    pstCardInfo->eCardType = GetCardType(pstCardInfo->aucCardNo);

    memset(aucBuf, 0, sizeof(aucBuf));

    // Patch: Malaysia GHL track data format is : track1 | track2 | track3
    if (0 == memcmp(stParConlist.aucName, "\x47\x48\x4C\x00", 4))
    {
        ucTmpLen = 0;

        // 1th track
        if (astCiTrack[SDK_TRACK1].ucTrackValid)
        {
            // Find the position of the %
            sOffSet1 = searchChar(astCiTrack[SDK_TRACK1].pucTrackBuf, '%', 0);
            // Find the position of the ?
            sOffSet2 = searchChar(astCiTrack[SDK_TRACK1].pucTrackBuf, '?', 0);
            if ((sOffSet1 >= 0) && (sOffSet2 >= 0) && (sOffSet2 > sOffSet1))
            {
                sOffSet1++;
                ucLen = sOffSet2 - sOffSet1;
                memcpy(aucTmpBuf[SDK_TRACK2] + ucTmpLen, astCiTrack[SDK_TRACK1].pucTrackBuf + sOffSet1, ucLen);
                ucTmpLen += ucLen;
            }
        }

        // Separator
        aucTmpBuf[SDK_TRACK2][ucTmpLen++] = '|';

        // 2th track
        memcpy(aucTmpBuf[SDK_TRACK2] + ucTmpLen, astCiTrack[SDK_TRACK2].pucTrackBuf + 1, astCiTrack[SDK_TRACK2].ucTrackLen - 2);
        ucTmpLen += astCiTrack[SDK_TRACK2].ucTrackLen - 2;

        // Separator
        aucTmpBuf[SDK_TRACK2][ucTmpLen++] = '|';

        // 3th track
        if (astCiTrack[SDK_TRACK3].ucTrackValid)
        {
            memcpy(aucTmpBuf[SDK_TRACK2] + ucTmpLen, astCiTrack[SDK_TRACK3].pucTrackBuf + 1, astCiTrack[SDK_TRACK3].ucTrackLen - 2);
            ucTmpLen += astCiTrack[SDK_TRACK3].ucTrackLen - 2;
        }
    }
#if 0
    else
    {
        // 2th track data, ucTrackLen includes; And ? length, ucTrackLen-2 has been removed; And ? , just leave only data
		Lib_AscNum2bcd(astCiTrack[SDK_TRACK2].pucTrackBuf + 1, aucTmpBuf[SDK_TRACK2], astCiTrack[SDK_TRACK2].ucTrackLen - 2, &ucTmpLen, 0x0f);
	}
	if (1 != TrackDataProcess(aucTmpBuf[SDK_TRACK2], ucTmpLen, aucTmpBuf[SDK_TRACK3], &ucLen, (u8)SDK_TRACK2, stParConlist.ucTrackPro))
	{
		errCode = 0x8025;
		g_usTradeMsgLen = 0;
		return errCode;
	}
	g_aucTradeMsgBuf[g_usTradeMsgLen] = 0x02;
	g_usTradeMsgLen++;
	g_aucTradeMsgBuf[g_usTradeMsgLen] = ucLen;
	g_usTradeMsgLen++;
	memcpy(g_aucTradeMsgBuf + g_usTradeMsgLen, aucTmpBuf[SDK_TRACK3], ucLen);
	g_usTradeMsgLen += ucLen;

    // 3th track
	if (astCiTrack[SDK_TRACK3].ucTrackValid)
	{
		Lib_AscNum2bcd(astCiTrack[SDK_TRACK3].pucTrackBuf + 1, aucTmpBuf[SDK_TRACK2], astCiTrack[SDK_TRACK3].ucTrackLen - 2, &ucTmpLen, 0x0f);
		if (1 != TrackDataProcess(aucTmpBuf[SDK_TRACK2], ucTmpLen, aucTmpBuf[SDK_TRACK3], &ucLen, (u8)SDK_TRACK3, stParConlist.ucTrackPro))
		{
			errCode = 0x8025;
			g_usTradeMsgLen = 0;
			return errCode;
		}
		g_aucTradeMsgBuf[g_usTradeMsgLen] = 0x03;
		g_usTradeMsgLen++;
		g_aucTradeMsgBuf[g_usTradeMsgLen] = ucLen;
		g_usTradeMsgLen++;
		memcpy(g_aucTradeMsgBuf + g_usTradeMsgLen, aucTmpBuf[SDK_TRACK3], ucLen);
		g_usTradeMsgLen += ucLen;
	}
	else
	{
		if (5 == stParConlist.ucTrackPro)
		{
            // After formatting, it needs to be returned
			// 56 bytes 0XFF
			memset(aucTmpBuf[SDK_TRACK2], 0xFF, sizeof(aucTmpBuf[SDK_TRACK2]));
			if (1 != TrackDataProcess(aucTmpBuf[SDK_TRACK2], 56, aucTmpBuf[SDK_TRACK3], &ucLen, (u8)SDK_TRACK3, stParConlist.ucTrackPro))
			{
				errCode = 0x8025;
				g_usTradeMsgLen = 0;
				return errCode;
			}
			g_aucTradeMsgBuf[g_usTradeMsgLen] = 0x03;
			g_usTradeMsgLen++;
			g_aucTradeMsgBuf[g_usTradeMsgLen] = ucLen;
			g_usTradeMsgLen++;
			memcpy(g_aucTradeMsgBuf + g_usTradeMsgLen, aucTmpBuf[SDK_TRACK3], ucLen);
			g_usTradeMsgLen += ucLen;
		}
	}
#endif
    else
    {
        memset(aucBuf, 0, sizeof(aucBuf));
        memcpy(aucBuf, astCiTrack[SDK_TRACK2].pucTrackBuf + 1, astCiTrack[SDK_TRACK2].ucTrackLen - 2);

        for (int i = 0; i < astCiTrack[SDK_TRACK2].ucTrackLen - 2; i++)
        {
            // '=' convert to D
            if (aucBuf[i] == 0x3D)
            {
                aucBuf[i] = 0x44;
            }
        }
    }
    memcpy(pstCardInfo->aucTrack2Data, aucBuf, astCiTrack[SDK_TRACK2].ucTrackLen - 2);
    pstCardInfo->ucTrack2Len = astCiTrack[SDK_TRACK2].ucTrackLen - 2;

    memset(aucBuf, 0, sizeof(aucBuf));
    // card valid date
    GetCardValidity(astCiTrack[SDK_TRACK2].pucTrackBuf, aucBuf, &ucLen);
    Lib_Bcd2Asc(aucBuf, (char *)pstCardInfo->aucExpiredDate, ucLen);
    TRACE("Card ExpiredDate: %s", pstCardInfo->aucExpiredDate);

    memset(aucBuf, 0, sizeof(aucBuf));
    // Get service code
    GetCardSevCode(astCiTrack[SDK_TRACK2].pucTrackBuf, aucBuf, &ucLen);
    Lib_Bcd2Asc(aucBuf, (char *)pstCardInfo->aucServiceCode, ucLen);
    TRACE("Card ServiceCode: %s", pstCardInfo->aucServiceCode);

    if (astCiTrack[SDK_TRACK1].ucTrackValid)
    {
        // 1th track
        //  Find the position of the %
        sOffSet1 = searchChar(astCiTrack[SDK_TRACK1].pucTrackBuf, '%', 0);
        if (sOffSet1 < 0)
        {
            errCode = 0x8025;
            g_usTradeMsgLen = 0;
            return errCode;
        }

        // Find the position of the ?
        sOffSet2 = searchChar(astCiTrack[SDK_TRACK1].pucTrackBuf, '?', sOffSet1);
        if (sOffSet2 < 0)
        {
            errCode = 0x8025;
            g_usTradeMsgLen = 0;
            return errCode;
        }
        TRACE("TRACK1 length = %d, sOffSet1 = %d, sOffSet2 = %d\r\n", astCiTrack[SDK_TRACK1].ucTrackLen, sOffSet1, sOffSet2);

        sOffSet1++;
        // 1th track data length
        sOffSet2 -= sOffSet1;
        Track1Process(astCiTrack[SDK_TRACK1].pucTrackBuf + sOffSet1, sOffSet2, aucTmpBuf[SDK_TRACK3], &ucLen, stParConlist.aucResver[1]);
    }

    sRet = InputPIN(pstTransData);

    return sRet;
}
