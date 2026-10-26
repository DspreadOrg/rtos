/*
********************************************************************************
*
*   File Name:
*       kernel_platform.h
*   Author:
*       jiaxin
*   Version:
*       V1.0
*   Description:
*
*
********************************************************************************
*/

#ifndef _KERNEL_PLATFORM_H
#define _KERNEL_PLATFORM_H

#ifdef __cplusplus
extern "C"
{
#endif//__cplusplus

/*-----------------------------------------------------------------------------
|   Includes
+----------------------------------------------------------------------------*/


#ifdef _KERNEL_PLATFORM_C
#define GLOBAL
#else
#define GLOBAL  extern
#endif

/*-----------------------------------------------------------------------------
|   Macros
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   Enumerations
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   Typedefs
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   Variables
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   Constants
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   prototypes
+----------------------------------------------------------------------------*/
GLOBAL void* kernel_memset(void* str, int c, int n);
GLOBAL void* kernel_memcpy(void* dest, const void* src, int n);
GLOBAL int kernel_memcmp(const void* str1, const void* str2, int n);
GLOBAL int kernel_strlen(const char* str);
GLOBAL int kernel_printf(const char* fmt, ...);

GLOBAL int kernel_rsa_public_key(unsigned char* n, unsigned int nlen, unsigned char* e, unsigned int elen, unsigned char* ibuf, unsigned int ilen, unsigned char* obuf);
GLOBAL int kernel_sha1(unsigned char* ibuf, unsigned int ilen, unsigned char* obuf);
GLOBAL int kernel_des_ecb_decrypt(unsigned char* key, unsigned int keylen, unsigned char* fill, unsigned char* ibuf, unsigned int ilen, unsigned char* obuf);
GLOBAL int kernel_des_ecb_encrypt(unsigned char* key, unsigned int keylen, unsigned char* fill, unsigned char* ibuf, unsigned int ilen, unsigned char* obuf);
GLOBAL int kernel_aes_decrypt(unsigned char* key, unsigned int keylen, unsigned char* ibuf, unsigned int ilen, unsigned char* obuf);
GLOBAL int kernel_aes_encrypt(unsigned char* key, unsigned int keylen, unsigned char* ibuf, unsigned int ilen, unsigned char* obuf);
GLOBAL int kernel_sm3_complete(unsigned char* ibuf, unsigned int ilen, unsigned char* obuf);
GLOBAL int kernel_sm2_verify(unsigned char* hash, unsigned int hashlen, unsigned char* sign, unsigned int signlen, unsigned char* key);

GLOBAL unsigned int kernel_tlv_new(unsigned char* tlv, unsigned int tag, unsigned int len, unsigned char* val);
GLOBAL unsigned int kernel_tlv_get_t(unsigned char* tlv);
GLOBAL unsigned int kernel_tlv_get_t_len(unsigned char* tlv);
GLOBAL unsigned int kernel_tlv_get_l(unsigned char* tlv);
GLOBAL unsigned int kernel_tlv_get_l_len(unsigned char* tlv);
GLOBAL unsigned char* kernel_tlv_get_v(unsigned char* tlv);
GLOBAL unsigned int kernel_tlv_get_node_l(unsigned char* tlv);
GLOBAL unsigned int kernel_tlv_add_child(unsigned char* tlv, unsigned int tag, unsigned int len, unsigned char* val);
GLOBAL unsigned char* kernel_tlv_find(unsigned char* tlv, unsigned int tag);
GLOBAL unsigned int kernel_tlv_delete(unsigned char* tlv, unsigned int tag);
GLOBAL unsigned int kernel_tlv_modify(unsigned char* tlv, unsigned int tag, unsigned int len, unsigned char* val);
GLOBAL unsigned int kernel_tlv_replace(unsigned char* tlv, unsigned int tag, unsigned int len, unsigned char* val);
GLOBAL unsigned int kernel_tlv_move(unsigned char* tlv, unsigned char* dest, unsigned int tag);
GLOBAL int kernel_tlv_is_composite(unsigned char* tlv);
GLOBAL unsigned char* kernel_tlv_get_first_child(unsigned char* tlv);
GLOBAL unsigned char* kernel_tlv_get_next_child(unsigned char* tlv);
GLOBAL unsigned int kernel_tlv_batch_move(unsigned char* tlv, unsigned char* dest);

/*
*******************************************************************************
*   End of File
*******************************************************************************
*/

#undef GLOBAL

#ifdef __cplusplus
}
#endif//__cplusplus

#endif  /* #ifndef _KERNEL_PLATFORM_H */
