//-----------------------------------------------------------------------------
// @file    : LibcJson.h
// @brief   : cJson-related functions
// @details : 
// @author  : 
// @version :      
// @date    :      
// @history :                  
//-----------------------------------------------------------------------------
#ifndef _LIB_CJSON_H
#define _LIB_CJSON_H

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

typedef int cJSON_bool;

/* The cJSON structure: */
typedef struct cJSON
{
    /* next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem */
    struct cJSON *next;
    struct cJSON *prev;
    /* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */
    struct cJSON *child;

    /* The type of the item, as above. */
    int type;

    /* The item's string, if type==cJSON_String  and type == cJSON_Raw */
    char *valuestring;
    /* writing to valueint is DEPRECATED, use cJSON_SetNumberValue instead */
    int valueint;
    /* The item's number, if type==cJSON_Number */
    double valuedouble;

    /* The item's name string, if this item is the child of, or is in the list of subitems of an object. */
    char *string;
} cJSON;

cJSON * Lib_cJSON_CreateRaw(const char *raw);
cJSON * Lib_cJSON_CreateArray(void);
cJSON * Lib_cJSON_CreateObject(void);
cJSON * Lib_cJSON_AddNullToObject(cJSON * const object, const char * const name);
cJSON * Lib_cJSON_AddTrueToObject(cJSON * const object, const char * const name);
cJSON * Lib_cJSON_AddFalseToObject(cJSON * const object, const char * const name);
cJSON * Lib_cJSON_AddBoolToObject(cJSON * const object, const char * const name, const cJSON_bool boolean);
cJSON * Lib_cJSON_AddNumberToObject(cJSON * const object, const char * const name, const double number);
cJSON * Lib_cJSON_AddStringToObject(cJSON * const object, const char * const name, const char * const string);
cJSON * Lib_cJSON_AddRawToObject(cJSON * const object, const char * const name, const char * const raw);
cJSON * Lib_cJSON_AddObjectToObject(cJSON * const object, const char * const name);
cJSON * Lib_cJSON_AddArrayToObject(cJSON * const object, const char * const name);
cJSON_bool Lib_cJSON_AddItemToArray(cJSON *array, cJSON *item);
cJSON_bool Lib_cJSON_AddItemToObject(cJSON *object, const char *string, cJSON *item);
char * Lib_cJSON_Print(const cJSON *item);
cJSON_bool Lib_cJSON_ReplaceItemInObject(cJSON *object,const char *string,cJSON *newitem);
cJSON * Lib_cJSON_CreateNull(void);
cJSON * Lib_cJSON_CreateTrue(void);
cJSON * Lib_cJSON_CreateFalse(void);
cJSON * Lib_cJSON_CreateBool(cJSON_bool boolean);
cJSON * Lib_cJSON_CreateNumber(double num);
cJSON * Lib_cJSON_CreateString(const char *string);
void Lib_cJSON_Delete(cJSON *item);
void Lib_cJSON_free(void *object);
cJSON * Lib_cJSON_Parse(const char *value);
cJSON * Lib_cJSON_GetObjectItem(const cJSON * const object, const char * const string);
int Lib_cJSON_GetArraySize(const cJSON *array);
cJSON * Lib_cJSON_GetArrayItem(const cJSON *array, int index);
char * Lib_cJSON_PrintUnformatted(const cJSON *item);
void Lib_cJSON_DeleteItemFromArray(cJSON *array, int which);


#endif

