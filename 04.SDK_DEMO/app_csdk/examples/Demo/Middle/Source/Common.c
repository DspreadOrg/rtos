/**********************************************************************************************************
 * Copyright(C)2011, 
 * File Name     : Common.c
 *  Version      :
 *  Author       :
 *  Date         :
 * Description   : Device General Algorithm apis
 **********************************************************************************************************/

#include "Common.h"
#include "ParamManage.h"
#include "Power.h"
#include "Input.h"
#include "Ui.h"

// MAX length of TRACE
#define TRACE_MAXLEN            1000


signed long long BcdToLongLong(unsigned char *pucBcd, unsigned char ucBcdLen)
{
    unsigned char i;
    signed long long llValue;

    llValue = 0;
    for(i=0;i<ucBcdLen;i++)
    {
        llValue *= 10;
        llValue += (pucBcd[i]>>4)&0x0F;
        llValue *= 10;
        llValue += (pucBcd[i]   )&0x0F;
    }

    return llValue;
}


unsigned char DateIsLegal(unsigned long year, unsigned char month, unsigned char day)
{
	unsigned char m_days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	if((year > 3000) || (year < 2001) || (month < 1) || (month > 12) || (day < 1) || (day > 31))
	{
        return FALSE;
	}
	if(((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
	{
		m_days[1]=29;
	}
	if(day > m_days[month-1])
	{
		return FALSE;
	}
	return TRUE;
}


//-----------------------------------------------------------------------------
// Name  : BcdSum2AscSum
// Func  : Convert BCD format amount to Asc amount (special customization)
// Paras :
// Ret   :
// Desc  : The amount input is 6 bytes, and the output format is amount x.xx (asc)
//-----------------------------------------------------------------------------
void BcdSum2AscSum(unsigned char *pucIn, char *pucOut)
{
	unsigned char i;
	char acTemp[20];

	Lib_Bcd2Asc(pucIn, acTemp, 6);

	// Fill in the amount whit '.'
	memmove(acTemp+11, acTemp+10, 2);
	acTemp[10] = '.';


	// Remove the top 0, and at most remove the top 9 zeros
	for (i=0; i<9; i++)
	{
		if ('0' != acTemp[i])
		{
			break;
		}
	}

	memcpy(pucOut, acTemp+i, 13-i);
	pucOut[13-i] = 0;
}

//-----------------------------------------------------------------------------
// Name  : IsLeapYear
// Func  : Determine if it is a leap year
// Paras :
// Ret   :
// Desc  :
//-----------------------------------------------------------------------------
short IsLeapYear(unsigned long ulYear)
{
	if (ulYear < 1600 || ulYear >=10000)
	{
		return -1;
	}
	else
	{
		// The condition for determining leap years
		if (((0 == ulYear%4) && (0 != ulYear%100)) || (0 == ulYear%400))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}


//-----------------------------------------------------------------------------
// Name  : DateCheck
// Func  : Verification date
// Paras :
// Ret   :
// Desc  :
//-----------------------------------------------------------------------------
short DateCheck(unsigned char ucMonth1, unsigned char ucDay1)
{
	unsigned char aucRtc[8];
	unsigned short usYear;
	unsigned char ucMonth,ucDay;

	ucMonth = (ucMonth1 >> 4)*10 + (ucMonth1&0x0F);
	ucDay = (ucDay1 >> 4)*10 + (ucDay1&0x0F);

	if (0 == ucDay || ucDay > 31)
	{
		return RET_ERR;
	}

	if (0 == ucMonth || ucMonth > 12)
	{
		return RET_ERR;
	}
	else if (4 == ucMonth || 6 == ucMonth|| 9 == ucMonth|| 11 == ucMonth)
	{
		if (ucDay == 31)
		{
			return RET_ERR;
		}
	}
	else if (2 == ucMonth)
	{
		aucRtc[0] = 0x20;
		Lib_GetDateTime(aucRtc+1);
		usYear = Lib_Bcd2Int(aucRtc, 2);

		if ( 0== IsLeapYear(usYear))
		{
			if (ucDay > 28)
			{
				return RET_ERR;
			}
		}
		else
		{
			if (ucDay > 29)
			{
				return RET_ERR;
			}
		}
	}
	return RET_OK;
}


//-----------------------------------------------------------------------------
// Name  : Strcspn
// Func  : Search for characters that appear in s2 in string s1.
// Paras :
// Ret   : Return the index value of the first character that appears in s1,
//         That is, the length of the substring that appears in s1 but not in s2.
// Desc  : Implement the strcspn function on your own
//-----------------------------------------------------------------------------
int Strcspn(const char * string,const char * control)
{
    const unsigned char * str = (const unsigned char *)string;
    const unsigned char * ctrl = (const unsigned char *)control;

    unsigned char map[32];
    int count = 0;

    //clear the bit map
    for(count = 0;count < 32;count++)
    {
        map[count] = 0;
    }
    //set bit in control map
    while(*ctrl)
    {
        map[*ctrl >> 3] |= (0x01 << (*ctrl & 7));
        ctrl++;
    }

    count = 0;
    map[0] |= 1;/*null chars not considered*/
    while(!(map[*str >> 3] & (0x01 << (*str & 7))))
    {
        count++;
        str++;
    }
    return count;
}

//-----------------------------------------------------------------------------
// Name  : iSplitStr
// Func  : Split String
// Paras : pcStr    : String before splitting
//         iStr1Len : Split length
//         pcStr1   : String split by length
//         pcStr    : The remaining string after splitting
// Ret   : Successfully returned 0
// Desc  :
//-----------------------------------------------------------------------------
int iSplitStr(const char *pcStr, int iStr1Len, char *pcStr1, char *pcStr2)
{
	int iPos, iAdd;
	int iLen = strlen(pcStr);

	// Find split location
	for (iPos = 0, iAdd = 0; iPos < iLen;)
	{
		if (iAdd >= iStr1Len)
		{
			break;
		}

		if (((unsigned char)pcStr[iPos]) >= 0x81)
		{
			if (((unsigned char)pcStr[iPos + 1]) < 0x40)
			{
				//18030 font lib
				iPos += 4;
				iAdd += 2;
				if (iAdd > iStr1Len)
				{
					iPos -= 4;
					break;
				}
			}
			else
			{
				//GBK Chinese character
				iPos += 2;
				iAdd += 2;
				if (iAdd > iStr1Len)
				{
					iPos -= 2;
					break;
				}
			}
		}
		else
		{
			// ASC character
			iPos++;
			iAdd += 1;
		}
	}

	if (iPos <= 0)
	{
		return -1;
	}

	// split string by length
	memcpy(pcStr1, pcStr, iPos);
	pcStr1[iPos] = 0;

	if (iPos < iLen)
	{
		// The remaining string after splitting
		memcpy(pcStr2, &pcStr[iPos], iLen - iPos);
		pcStr2[iLen - iPos] = 0;
	}

	return 0;
}


void itoaw (int n,char s[])
{
	int i,j,k=0;
	char w[40];
	i=0;
	do{
        // Take the next number
        w[i++]=n%10+'0';
	}
	while ((n/=10)>0); // Delete this number
	w[i]='\0';

	// The generated numbers are in reverse order, so need to be output in reverse order
	for(j=i-1;j>=0;j--)
	{
		s[k]= w[j];
		k++;
	}
}
static unsigned char s_ucTraceEnFlag = 0;

void ApiLibTrace(const char *fmt, ...)
{
    if (s_ucTraceEnFlag)
    {
        char acBuf[2048];
#if 0
#define TRACEBUF_LEN  4096
        static char *acBuf = NULL;
        if (acBuf == NULL)
        {
            acBuf = Lib_malloc(TRACEBUF_LEN);
            if (acBuf == NULL)
            {
                return;
            }
        }
        memset(acBuf, 0x00, TRACEBUF_LEN);
#endif
    	va_list args;
    	va_start(args, fmt);
    	vsprintf(acBuf, fmt, args);
    	va_end(args);

        Lib_Trace("%s", acBuf);
    }
}

void ApiLibTraceBuf(uint8_t *pBuf, uint16_t len)
{
    if (s_ucTraceEnFlag)
    {
        Lib_TraceBuf(pBuf, len);
    }
}

void SetTraceLogConfig(unsigned char ucFlag)
{
    s_ucTraceEnFlag = ucFlag;
    Lib_TraceLogConfig(ucFlag);
}

unsigned char GetTraceFlag(void)
{
    return s_ucTraceEnFlag;
}


short Trace_LongData(char *data)
{
    unsigned char aucPrnbuf[TRACE_MAXLEN+32] = {0};
    int i = 0;

    if(data == NULL)
    {
        return RET_ERR;
    }

    //TRACE ("Pack json body:");
    for (i = 0; i < strlen(data ) / TRACE_MAXLEN; i++)
    {
        BUFCLR(aucPrnbuf);
        memcpy(aucPrnbuf, data + (TRACE_MAXLEN*i), TRACE_MAXLEN);
        TRACE("%s",aucPrnbuf);
        Lib_DelayMs(1);
    }

    if (i * TRACE_MAXLEN < strlen(data))
    {
        BUFCLR(aucPrnbuf);
        memcpy(aucPrnbuf, data + (TRACE_MAXLEN*i), strlen(data) - (i * TRACE_MAXLEN) );
        TRACE("%s",aucPrnbuf);
    }

    return RET_OK;
}

short Trace_LongBuf(char *data, unsigned int len)
{
    int i = 0;
    unsigned int MAXLEN = 256;

    if(data == NULL)
    {
        return RET_ERR;
    }

    //TRACE ("Pack json body:");
    for (i = 0; i < strlen(data ) / MAXLEN; i++)
    {
        TRACEBUF((unsigned char *)(data + (MAXLEN*i)), MAXLEN);
        Lib_DelayMs(1);
    }

    if (i * MAXLEN < strlen(data))
    {
        TRACEBUF((unsigned char *)(data + (MAXLEN*i)), strlen(data) - (i * MAXLEN) );
    }
    return RET_OK;
}


void PubLongToChar(unsigned long ulDecData, int iHexlen, char *psHexData)
{
    int iCount;

    for (iCount = iHexlen-1; iCount >= 0; iCount--)
    {
        // Store the remainder in the low byte
        psHexData[iCount] = (char)(ulDecData%256);
        ulDecData /= 256;
    }
}


unsigned long PubCharToLong(const unsigned char *psHexData, int iHexLen, unsigned long *pulOut)
{
    int iCount;
    unsigned long ulTmp, ulValue;

    ulTmp = ulValue = 0;
    for (iCount = 0; iCount < iHexLen; iCount++)
    {
        ulValue = (unsigned long)psHexData[iCount] & 0xFF;
        ulTmp += (ulValue << 8 * (iHexLen - iCount - 1));
    }

    if (pulOut != NULL)
    {
        *pulOut = ulTmp;
    }
    return ulTmp;
}

void AmtAddThdsPt(char* amtstr)
{
	char tempBuf[20];
	char* prt1, *prt2;
	int AmtLength = 0, decPoint = 0, head = 0, counter = 0;

	FUN_IN;
	TRACE ( "AmtLength = %d, amtstr=%s",strlen ( amtstr ),amtstr );
	// ȥ�����3λ, ��С�������λ��С����
	memset ( tempBuf, 0, sizeof ( tempBuf ) );
	AmtLength = strlen ( amtstr ) - 3;
	if ( AmtLength > 3 )
	{
		prt1 = amtstr;
		prt2 = tempBuf;
		decPoint = AmtLength / 3;

		// ������������1, ȷ��ǧ��λ�ĸ���
		if ( 0 == AmtLength % 3 )
		{
			decPoint--;
		}
		else
		{
			// ȷ����1��ǧ��λ֮ǰ��λ��, ������
			head = AmtLength % 3;

			for ( counter = 0; counter < head; counter++ )
			{
				*prt2 = *prt1;
				prt1++;
				prt2++;
			}

			// ������ɺ��ǧ��λ, ָ���λ
			*prt2 = ',';
			prt2++;
			// ����ǧ��λ��, ������1
			decPoint--;
		}

		// ����ʣ�µ�ǧλ��
		while ( decPoint )
		{
			for ( counter = 0; counter < 3; counter++ )
			{
				*prt2 = *prt1;
				prt1++;
				prt2++;
			}

			*prt2 = ',';
			prt2++;
			decPoint--;
		}

		// ����ʣ�³���
		while ( *prt1 != '\0' )
		{
			*prt2 = *prt1;
			prt1++;
			prt2++;
		}

		*prt2 = '\0';

		// memset ( amtstr, 0, sizeof ( amtstr ) );
		strcpy(amtstr, tempBuf);
	}
	// end adding decimal point
}

unsigned long dfToInt(double dValue)
{
    unsigned long uTemp, uTemp1;
    unsigned long uReValue;

    uTemp1 = dValue * 100;

    uTemp = (uTemp1 + 5)/10;

    uReValue = (uTemp + 5)/10;

    return uReValue;
}