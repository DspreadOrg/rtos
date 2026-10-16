/**********************************************************************************************************
 * Copyright(C)2011,
 * File Name     : Drv.h
 *  Version      :
 *  Author       :
 * Description   : Device configuration parameters
 **********************************************************************************************************/

#ifndef __DRV_H__
#define __DRV_H__

#include "LibApi.h"
#include "Ui.h"


//-----------------------------------------------------------------------------
// macro definition
//-----------------------------------------------------------------------------
// Device PN
#if defined(HW_D70R)
#define DEV_PN                      "D70R"

// Device peripheral configuration
#define HW_GPRS                     1   // 1 has gprs, 0 no gprs
#define HW_TPH                      1   // 1 has printer, 0 no printer
#define HW_WIFI                     1   // 1 has WiFi , 0  no WiFi
#define HW_QR_CAM                   1   // 1 has camera, 0 no camera

#elif defined(HW_A30R)
#define DEV_PN                      "A30R"

// Device peripheral configuration
#define HW_GPRS                     1   // 1 has gprs, 0 no gprs
#define HW_TPH                      0   // 1 has printer, 0 no printer
#define HW_WIFI                     1   // 1 has WiFi , 0  no WiFi
#define HW_QR_CAM                   0   // 1 has camera, 0 no camera

#elif defined(HW_A75R)
#define DEV_PN                      "A75R"

// Device peripheral configuration
#define HW_GPRS                     1   // 1 has gprs, 0 no gprs
#define HW_TPH                      1   // 1 has printer, 0 no printer
#define HW_WIFI                     1   // 1 has WiFi , 0  no WiFi
#define HW_QR_CAM                   1   // 1 has camera, 0 no camera


#elif
#error "PN undefined"
#endif

// Server parameters
#define HOST1_IP                    "120.55.38.45"
#define HOST1_PORT                  "29998"

#define HOST2_IP                    "120.55.38.45"
#define HOST2_PORT                  "29998"

// POSP service domain name+port (primary+backup)
#define NET_NAME_POSP               "posp.mdmistore.com"
#define NET_NAME_POSP_PORT          "29998"

#define NET_NAME_POSP_2             "posp.mdmistore.com"
#define NET_NAME_POSP_PORT_2        "29998"

// TMS server parameters
#define TMS_HOST1_IP                "101.37.30.113"
#define TMS_HOST1_PORT              "9999"

#define TMS_HOST2_IP                "101.37.30.113"
#define TMS_HOST2_PORT              "9999"

// TMS service domain name+port (primary+backup)
#define TMS_NET_NAME_POSP           "open.mdmistore.com"
#define TMS_NET_NAME_POSP_PORT      "9999"

#define TMS_NET_NAME_POSP_2         "app.test.com.cn"
#define TMS_NET_NAME_POSP_PORT_2    "9999"

//TMS server parameters for module upgrade
#define TMS_MODULE_DOMAIN_1         "open.mdmistore.com"
#define TMS_MODULE_DOMAIN_PORT_1    "9999"

// NTP server default
#define NTP_SERVER_DOMAIN           "time.windows.com"

#endif

