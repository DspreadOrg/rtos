/**********************************************************************************************************
 * Copyright(C)2021, 
 * File Name     : TLV.h
 *  Version      :
 *  Author       :
 * Description   : TLV library APIs function
 **********************************************************************************************************/

#ifndef __TLV_H__
#define __TLV_H__

#include "EmvType.h"
#include "EmvError.h"

#define TAG_UNDEFINED           0xFFFFFF

typedef struct _TLV_LIST {
    // TLV LIST Buf total size
    uint32 size;
    // Actual usage length
    uint32 used;
    // TLV LIST data: T L V T L V ...
    uint8 *data;
} TLV_LIST;

typedef struct _TLV {
    uint32 Tag;
    uint32 Len;
    uint8 *pV;

    // The offset of TLV HEX data in TLV LIST
    uint32 offset;
    // Total length of TLV HEX data
    uint32 total;
} TLV;

// TLV LIST iteration
typedef struct _TLV_LIST_ITERATION {
    TLV_LIST *pTlvList;
    uint32 offset;
} TLV_LIST_ITERATION;


typedef struct _TL_LIST {
    // TL LIST Buf total size
    uint32 size;
    // Actual usage length
    uint32 used;
    // TL LIST data£ºT L T L ...
    uint8 *data;
} TL_LIST;

typedef struct _TL {
    uint32 Tag;
    uint32 Len;

    // The offset of TL HEX data in TLV LIST
    uint32 offset;
    // Total length of TL HEX data
    uint32 total;
} TL;

// TL LIST iteration
typedef struct _TL_LIST_ITERATION {
    TL_LIST *pTlList;
    uint32 offset;
} TL_LIST_ITERATION;


//------------------------------------------------------------------------------
// Function    : TlvAdd
// Description : Add, Add the specified TLV to the TLV List
// Param       : pTlvList       - IN/OUT  TLV LIST
//               pTlv           - IN      TLV structural
// Return      :
// Other       : If the TAG already exists, change its value
// Create      : 2016/5/25 by liaoxl
// Modify 1    : -/-/- by -
//------------------------------------------------------------------------------
ERR TlvAdd(TLV_LIST *pTlvList, const TLV *pTlv);

//------------------------------------------------------------------------------
// Function    : TlvAddList
// Description : Add, Add a specified TLV List substring to the TLV List
// Param       : pTlvList       - IN/OUT  TLV LIST
//               pSubList       - IN      TLV LIST
// Return      :
// Other       :
// Create      : 2016/5/25 by liaoxl
// Modify 1    : -/-/- by -
//------------------------------------------------------------------------------
ERR TlvAddList(TLV_LIST *pSrcList, const TLV_LIST *pSubList);

//------------------------------------------------------------------------------
// Function    : TlvDelete
// Description : Delete, delete the specified TLV in the TLV List
// Param       : pTlvList       - IN/OUT  TLV LIST
//               tagToDelete    - IN      TAG
// Return      :
// Other       :
// Create      : 2016/5/25 by liaoxl
// Modify 1    : -/-/- by -
//------------------------------------------------------------------------------
ERR TlvDelete(TLV_LIST *pTlvList, uint32 tagToDelete);

//------------------------------------------------------------------------------
// Function    : TlvModify
// Description : Modify, modify the specified TLV in the TLV List
// Param       : pTlvList       - IN/OUT  TLV LIST
//               pTlv           - IN      TLV structural
// Return      :
// Other       :
// Create      : 2016/5/25 by liaoxl
// Modify 1    : -/-/- by -
//------------------------------------------------------------------------------
ERR TlvModify(TLV_LIST *pTlvList, const TLV *pTlv);

//------------------------------------------------------------------------------
// Function    : TlvFind
// Description : Search, Find the specified TAG in the TLV List
// Param       : tagToFind      - IN  TAG to be searched
//               pTlvList       - IN  TLV LIST
//               pTlv           - OUT TLV structural morphology
// Return      :
// Other       :
// Create      : 2016/5/25 by liaoxl
// Modify 1    : -/-/- by -
//------------------------------------------------------------------------------
ERR TlvFind(const TLV_LIST *pTlvList, uint32 tagToFind, TLV *pTlv);

//------------------------------------------------------------------------------
// Function    : TlvRead
// Description : Read, read the specified TAG in the TLV List
// Param       : pTlvList       - IN  TLV LIST
//               tag            - TAG
//               bufSize        - Cache space size
//               pBuf           - TAG value
//               pValueLen      - TAG value length
// Return      :
// Other       :
// Create      : 2016/5/25 by liaoxl
// Modify 1    : -/-/- by -
//------------------------------------------------------------------------------
ERR TlvRead(const TLV_LIST *pTlvList, uint32 tag, uint8 *pBuf, uint32 bufSize, uint32 *pValueLen);

//------------------------------------------------------------------------------
// Function    : TlvWrite
// Description : Write, write the specified TAG in the TLV List
// Param       : pTlvList       - IN  TLV LIST
//               tag            - TAG
//               pBuf           - TAG value
//               Len            - TAG value length
// Return      :
// Other       :
// Create      : 2016/5/25 by liaoxl
// Modify 1    : -/-/- by -
//------------------------------------------------------------------------------
ERR TlvWrite(TLV_LIST *pTlvList, uint32 tag, uint8 *pBuf, uint32 Len);

//------------------------------------------------------------------------------
// Function    : TlvTHex2Int
// Description : Convert HEX's TAG to INT
// Param       : pHex       - IN  HEX
//               pTag       - OUT Obtained INT type TAG
//               pTagLen    - OUT the length of TAG
// Return      :
// Other       :
// Create      : 2016/5/25 by liaoxl
// Modify 1    : -/-/- by -
//------------------------------------------------------------------------------
ERR TlvTHex2Int(const uint8 *pHex, uint32 *pTag, uint32 *pTagLen);

//------------------------------------------------------------------------------
// Function    : TlvTInt2Hex
// Description : Convert INT type TAG to HEX
// Param       : pHex       - OUT HEX
//               hexSize    - IN  HEX length
//               tag        - IN  INT type TAG
//               pTagLen    - OUT The length of TAG
// Return      :
// Other       :
// Create      : 2016/5/25 by liaoxl
// Modify 1    : -/-/- by -
//------------------------------------------------------------------------------
ERR TlvTInt2Hex(uint8 *pHex, uint32 hexLen, uint32 tag, uint8 *pTagLen);

//------------------------------------------------------------------------------
// Function    : TlvLHex2Int
// Description : Convert HEX's LENGTH to INT
// Param       : pHex       - IN  HEX
//               pLength    - OUT Obtained INT type LENGTH
//               pLengthLen - OUT The length of LENGTH itself
// Return      :
// Other       :
// Create      : 2016/5/25 by liaoxl
// Modify 1    : -/-/- by -
//------------------------------------------------------------------------------
ERR TlvLHex2Int(const uint8 *pHex, uint32 *pLength, uint32 *pLengthLen);

//------------------------------------------------------------------------------
// Function    : TlvLInt2Hex
// Description : Convert INT type TAG to HEX
// Param       : pHex       - OUT HEX
//               hexSize    - IN  HEX length
//               length     - IN  INT type LENGTH
//               pLengthLen - OUT The length of LENGTH itself
// Return      :
// Other       :
// Create      : 2016/5/25 by liaoxl
// Modify 1    : -/-/- by -
//------------------------------------------------------------------------------
ERR TlvLInt2Hex(uint8 *pHex, uint32 hexSize, uint32 length, uint8 *pLengthLen);

//------------------------------------------------------------------------------
// Function    : TlvListCreate
// Description : Create a TLV-LIST structure using the given BUF
// Param       :
// Return      :
// Other       :
// Create      : 2016/5/25 by liaoxl
// Modify 1    : -/-/- by -
//------------------------------------------------------------------------------
ERR TlvListCreate(TLV_LIST *pTlvList, uint8 *pBuf, uint32 bufSize, uint32 usedDataLen);

//------------------------------------------------------------------------------
// Function    : TlvCreate
// Description : Create a TLV structure using the given T, L, and V values
// Param       :
// Return      :
// Other       :
// Create      : 2016/5/25 by liaoxl
// Modify 1    : -/-/- by -
//------------------------------------------------------------------------------
ERR TlvCreate(TLV *pTlv, uint32 t, uint32 l, uint8 *v);

//------------------------------------------------------------------------------
// Function    : TlvCreate
// Description : Create a TLV structure using the given TLV HEX
// Param       :
// Return      :
// Other       :
// Create      : 2016/5/25 by liaoxl
// Modify 1    : -/-/- by -
//------------------------------------------------------------------------------
ERR TlvCreateByHex(TLV *pTlv, uint8 *pTlvHex, uint32 tlvHexLen);

//------------------------------------------------------------------------------
// Function    : TlvTlLen
// Description : The total length of T and L after the TLV structure is converted to HEX
// Param       :
// Return      :
// Other       :
// Create      : 2018/11/06 by lijingjing
// Modify 1    : -/-/- by -
//------------------------------------------------------------------------------
uint32 TlvTlLen(const TLV *pTlv);

//------------------------------------------------------------------------------
// Function    : TlvTotalLen
// Description : The total length of TLV structure after conversion to HEX
// Param       :
// Return      :
// Other       :
// Create      : 2018/11/06 by lijingjing
// Modify 1    : -/-/- by -
//------------------------------------------------------------------------------
uint32 TlvTotalLen(const TLV *pTlv);

//------------------------------------------------------------------------------
// Function    : Tlv2Hex
// Description : TLV structure converted to HEX
// Param       : pTlv       - TLV structural morphology
//               pHex       - HEX
//               hexSize    - The size of HEX
//               pHexLen    - OUT Actual size
// Return      :
// Other       :
// Create      : 2016/5/25 by liaoxl
// Modify 1    : -/-/- by -
//------------------------------------------------------------------------------
ERR Tlv2Hex(const TLV *pTlv, uint8 *pHex, uint32 hexSize, uint32 *pHexLen);



//------------------------------------------------------------------------------
// Function    : TlvListIterationInit
// Description : TLV LIST iteration initialization, TLV LIST iteration refers to traversing the TLVs in TLV LIST
// Param       : pTlvList   - IN TLV LIST
// Return      : TLV_LIST_ITERATION
// Other       :
// Create      :
// Modify 1    : -/-/- by -
//------------------------------------------------------------------------------
TLV_LIST_ITERATION TlvListIterationInit(const TLV_LIST *pTlvList);

//------------------------------------------------------------------------------
// Function    : TlvListIteration
// Description : TLV LIST iteration
// Param       : pTlvListIteration   - IN
//               pTlv    - OUT
// Return      : ERR_OK obtained a TLV, ERR-TLV-LIST-ITERATION-COMPLETE - obtained the last TLV, other - error
// Other       :
// Create      :
// Modify 1    : -/-/- by -
//------------------------------------------------------------------------------
ERR TlvListIteration(TLV_LIST_ITERATION *pTlvListIteration, TLV *pTlv);




//------------------------------------------------------------------------------
// Function    : GetValueAndLenFromLV
// Description : Extract specified V and L from the LV list structure
// Param       : LvBuf       - IN  LV List Array
//               LvLen       - IN  LV array length
//               No          - IN  LV Structure Index Number
//				 Len		 - OUT Specify index LV structure L value
//				 Value		 - OUT Specify index LV structure V value
// Return      :
// Other       :
// Create      : 2017/5/23 by xzr
// Modify 1    : -/-/- by -
//------------------------------------------------------------------------------
ERR GetValueAndLenFromLV(uint8 *LvBuf, uint32 LvLen, uint32 No, uint8 *Value, uint32 *Len);

//------------------------------------------------------------------------------
// Function    : SetValueAndLenToLV
// Description : Write specified V and L to the LV list structure
// Param       : LvBuf       - IN  LV List Array
//               LvLen       - IN  LV array length
//               No          - IN  LV Structure Index Number
//				 Len		 - OUT Specify index LV structure L value
//				 Value		 - OUT Specify index LV structure V value
// Return      :
// Other       :
// Create      : 2017/5/23 by xzr
// Modify 1    : -/-/- by -
//------------------------------------------------------------------------------
ERR SetValueAndLenToLV(uint8 *LvBuf, uint32 *LvLen, uint32 No, uint8 *Value, uint32 Len);

//------------------------------------------------------------------------------
// Function    : CombileTlvByDol
// Description : Combine DOL list and V list to form TLV list
// Param       : pTlvList    - IN/OUT  TLV List
//               pucDol      - IN  DOL list
//               ulDolLen    - IN  DOL list length
//               pucValue    - IN  Value set
//               ulValueLen  - IN  Value set length
// Return      :
// Other       :
// Create      : 2019/8/30 by Li Jingjing
// Modify 1    : -/-/- by -
//------------------------------------------------------------------------------
ERR CombileTlvByDol(TLV_LIST *pTlvList, uint8 *pucDol, uint32 ulDolLen, uint8 *pucValue, uint32 ulValueLen);

#endif

