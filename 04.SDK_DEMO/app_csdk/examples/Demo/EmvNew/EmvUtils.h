/**********************************************************************************************************
 * Copyright(C)2026,
 * File Name     : EmvUtils.h
 * Version       : v1.0
 * Author        :
 * Description   : EMV kernel utils
 **********************************************************************************************************/

#ifndef __EMV_UTILS_H__
#define __EMV_UTILS_H__

#include <string.h>

#include "emv_kernel.h"

int app_rsa_public_key(unsigned char *n, unsigned int nlen, unsigned char *e, unsigned int elen, unsigned char *ibuf, unsigned int ilen, unsigned char *obuf);
int app_sha1(unsigned char *ibuf, unsigned int ilen, unsigned char *obuf);
int app_des_decrypt(unsigned char *key, unsigned int key_len, unsigned char *fill, unsigned char *ibuf, unsigned int ilen, unsigned char *obuf);
int app_des_encrypt(unsigned char *key, unsigned int key_len, unsigned char *fill, unsigned char *ibuf, unsigned int ilen, unsigned char *obuf);
int app_aes_decrypt(unsigned char *key, unsigned int key_len, unsigned char *ibuf, unsigned int ilen, unsigned char *obuf);
int app_aes_encrypt(unsigned char *key, unsigned int key_len, unsigned char *ibuf, unsigned int ilen, unsigned char *obuf);

void *app_memset(void *str, int c, int n);
void *app_memcpy(void *dest, const void *src, int n);
int app_memcmp(const void *str1, const void *str2, int n);
int app_strlen(const char *str);
void app_log(const char *str);

#endif
