/*
 * @File  gps.h
 * @Brief gps operation releated
 *
 * @Author: Neucrack
 * @Date: 2018-06-14 18:04:49
 * @Last Modified by: Neucrack
 * @Last Modified time: 2018-06-15 18:48:24
 */


#ifndef __GPS_H
#define __GPS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

#define true                            1
#define false                           0
/////////////////////////////////////////
////////////////config//////////////////
#define GPS_BUFFER_MAX_LENGTH           530  //max:agps gpd file pack need 512+8+3=523(Bytes)
#define GPS_TIME_OUT_CMD                1500
#define GPS_NMEA_FRAME_BUFFER_LENGTH    1024
#define GPS_DATA_BUFFER_MAX_LENGTH      2048

#define GPS_DEBUG 1

/////////////////////////////////////////

#if (GPS_DEBUG == 1)
#define GPS_DEBUG_I(...)  \
    do \
    { \
        TRACE(__VA_ARGS__); \
    } while(0)

#define GPS_DEBUG_MEM(a,b,c) \
    do \
    { \
        TRACEBUF(a,b); \
    } while(0)
#else
#define GPS_DEBUG_I(fmt,...)
#define GPS_DEBUG_MEM(a,b,c)
#endif

typedef enum
{
    GPS_CMD_SAVE_INFO_IN_FLASH   = 0,
    GPS_CMD_SET_UART_BAUDRATE    = 1,
    GPS_CMD_NMEA_OUTPUT_INTERVAL = 2,
    GPS_CMD_NMEA_OUTPUT_FREQ     = 3,
    GPS_CMD_SEARCH_MODE          = 4,
    GPS_CMD_GET_INFO             = 6,
    GPS_CMD_REBOOT               = 10,
    GPS_CMD_MAX
} GPS_CMD_t;

typedef enum
{
    GPS_REBOOT_MODE_HOT  = 0,
    GPS_REBOOT_MODE_WARM = 1,
    GPS_REBOOT_MODE_COLD = 2,
    GPS_REBOOT_MODE_FACTORY = 3,
    GPS_REBOOT_MODE_MAX
} GPS_Reboot_Mode_t;

typedef enum
{
    GPS_STANDBY_MODE_STOP  = 0,
    GPS_STANDBY_MODE_SLEEP = 1,
    GPS_STANDBY_MODE_MAX
} GPS_STANDBY_Mode_t;            // standby mode

typedef enum
{
    GPS_UART_BAUDRATE_4800 = 0,
    GPS_UART_BAUDRATE_9600 = 1,
    GPS_UART_BAUDRATE_19200 = 2,
    GPS_UART_BAUDRATE_38400 = 3,
    GPS_UART_BAUDRATE_57600 = 4,
    GPS_UART_BAUDRATE_115200 = 5,
    GPS_UART_BAUDRATE_MAX
} GPS_UART_Baudrate_t;            // standby mode

typedef enum
{
    GPS_SEARCH_MODE_GPS = 1,
    GPS_SEARCH_MODE_BDS = 2,
    GPS_SEARCH_MODE_GPS_BDS = 3,
    GPS_SEARCH_MODE_GLONASS = 4,
    GPS_SEARCH_MODE_GPS_GLONASS = 5,
    GPS_SEARCH_MODE_BDS_GLONASS = 6,
    GPS_SEARCH_MODE_GPS_BDS_GLONASS = 7,
    GPS_SEARCH_MODE_MAX
} GPS_SEARCH_Mode_t;            // standby mode

typedef enum
{
    GPS_LP_MODE_NORMAL         = 0, // normal run mode
    GPS_LP_MODE_LP             = 8, // lowpower mode, wake up by uart
    GPS_LP_MODE_SUPPER_LP      = 9, // super lowper track mode
    GPS_LP_MODE_MAX
} GPS_LP_Mode_t;                 // low power mode

typedef enum
{
    GPS_FORMAT_NMEA = 0,
    GPS_FORMAT_BINARY = 1,
    GPS_FORMAT_MAX
} GPS_Format_t;

typedef struct
{
    unsigned char gga;
    unsigned char gll;
    unsigned char gsa;
    unsigned char gsv;
    unsigned char rmc;
    unsigned char vtg;
    unsigned char zda;
    unsigned char ant;
    unsigned char dhv;
    unsigned char lps;
    unsigned char utc;
    unsigned char gst;
    unsigned char tim;
} GPS_NMEA_Output_Freq_t;

typedef enum
{
    GPS_FIX_MODE_NORMAL = 0,
    GPS_FIX_MODE_LOW_SPEED  = 1,
    GPS_FIX_MODE_HIGH_SPEED = 2,
    GPS_FIX_MODE_ELEVATION  = 3,
    GPS_FIX_MODE_MAX
} GPS_Fix_Mode_t;

typedef enum
{
    GPS_POSITION = 70,
    GPS_TIME = 71,
    GPS_MAX
} GPS_Info_Type_e;

extern int Lib_GpsInit(void);
extern int Lib_GpsOpen(void);
extern int Lib_GpsClose(void);
extern int Lib_GpsIsOpen(void);
extern int Lib_GpsReboot(GPS_Reboot_Mode_t mode);

#ifdef __cplusplus
}
#endif

#endif

