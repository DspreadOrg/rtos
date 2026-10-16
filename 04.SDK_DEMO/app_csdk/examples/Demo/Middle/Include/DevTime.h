/**********************************************************************************************************
 * Copyright(C)2011,
 * File Name     : DevTime.h
 *  Version      :
 *  Author       :
 * Description   : Device time management functiuon
 **********************************************************************************************************/

#ifndef __DEV_TIME_H__
#define __DEV_TIME_H__

#include "LibTypedef.h"

typedef struct
{
    const char *name;
    int timeZone;
} stWorldTimeZone;

short UISysTimeSet(int timeout);
void AutoSyncDateTimeFun(void);

#endif
