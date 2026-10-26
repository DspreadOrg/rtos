/*
********************************************************************************
*
*   File Name:
*       emv_kernel.h
*   Author:
*       jiaxin
*   Version:
*       V1.0
*   Description:
*
*
********************************************************************************
*/

#ifndef _EMV_KERNEL_H
#define _EMV_KERNEL_H

#ifdef __cplusplus
extern "C"
{
#endif//__cplusplus

/*-----------------------------------------------------------------------------
|   Includes
+----------------------------------------------------------------------------*/


#ifdef _EMV_KERNEL_C
#define GLOBAL
#else
#define GLOBAL  extern
#endif

/*-----------------------------------------------------------------------------
|   Macros
+----------------------------------------------------------------------------*/
#define EMV_KERNEL_VERSION     "1.2.6"


/*-----------------------------------------------------------------------------
|   Enumerations
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   Typedefs
+----------------------------------------------------------------------------*/
typedef int (*emv_kernel_callback_t)(void* user_data, unsigned char* buf, unsigned int len);

typedef struct _emv_kernel_alg_t
{
    int (*opt_rsa_public_key)(unsigned char* n, unsigned int nlen, unsigned char* e, unsigned int elen, unsigned char* ibuf, unsigned int ilen, unsigned char* obuf);
    int (*opt_sha1)(unsigned char* ibuf, unsigned int ilen, unsigned char* obuf);
    int (*opt_des_decrypt)(unsigned char* key, unsigned int key_len, unsigned char* fill, unsigned char* ibuf, unsigned int ilen, unsigned char* obuf);
    int (*opt_des_encrypt)(unsigned char* key, unsigned int key_len, unsigned char* fill, unsigned char* ibuf, unsigned int ilen, unsigned char* obuf);
    int (*opt_aes_decrypt)(unsigned char* key, unsigned int key_len, unsigned char* ibuf, unsigned int ilen, unsigned char* obuf);
    int (*opt_aes_encrypt)(unsigned char* key, unsigned int key_len, unsigned char* ibuf, unsigned int ilen, unsigned char* obuf);
}emv_kernel_alg_t;

typedef struct _emv_kernel_std_t
{
    void* (*opt_memset)(void *str, int c, int n);
    void* (*opt_memcpy)(void *dest, const void *src, int n);
    int (*opt_memcmp)(const void *str1, const void *str2, int n);
    int (*opt_strlen)(const char *str);
    void (*opt_log)(const char *str);
}emv_kernel_std_t;

/*-----------------------------------------------------------------------------
|   Variables
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   Constants
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   prototypes
+----------------------------------------------------------------------------*/
GLOBAL int emv_kernel_set_callback(emv_kernel_callback_t cb, void* user_data);
GLOBAL int emv_kernel_set_std(emv_kernel_std_t* emv_kernel_std);
GLOBAL int emv_kernel_set_alg(emv_kernel_alg_t* emv_kernel_alg);
GLOBAL int emv_kernel_start(unsigned char* buf, unsigned int size);
GLOBAL int emv_kernel_clean(unsigned char* buf, unsigned int size);
GLOBAL int emv_kernel_customized(unsigned char* buf, unsigned int size);
GLOBAL int emv_kernel_error(void);
GLOBAL int emv_kernel_get_contact_element(unsigned int tag, unsigned char* pdata, unsigned int* pdatalen);
GLOBAL int emv_kernel_set_contact_element(unsigned int tag, unsigned int length, unsigned char* pvalue);
GLOBAL int emv_kernel_get_contactless_element(unsigned int tag, unsigned char* pdata, unsigned int* pdatalen);
GLOBAL int emv_kernel_set_contactless_element(unsigned int tag, unsigned int length, unsigned char* pvalue);
GLOBAL const char* emv_kernel_get_version(void);

/*
*******************************************************************************
*   End of File
*******************************************************************************
*/

#undef GLOBAL

#include "outcome_list.h"
#include "tag_list.h"
#include "error_code_list.h"
#include "kernel_platform.h"
#include "kernel_include_version.h"

#ifdef __cplusplus
}
#endif//__cplusplus

#endif  /* #ifndef _EMV_KERNEL_H */
