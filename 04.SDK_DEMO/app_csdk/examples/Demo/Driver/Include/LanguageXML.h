/**********************************************************************************************************
 * Copyright(C)2011,
 * Description: Factory language macro definition
 * Change Logs:
 * Date           Author         Notes
 * 2022-03-04     leeon          first version
 **********************************************************************************************************/

#ifndef _LANGUAGE_XML_H_
#define _LANGUAGE_XML_H_

#include "LanguageResource.h"
#include "LanguageProcess.h"

//
#define SELECT_LANG                             (char *)(Lib_XmlCvtIdToStr(ID_SELECT_LANG))
#define INITIALIZE_LANG                         (char *)(Lib_XmlCvtIdToStr(ID_INITIALIZE_LANG))
#define SETTING_DONE                            (char *)(Lib_XmlCvtIdToStr(ID_SETTING_DONE))
#define INITIALIZE_FAIL                         (char *)(Lib_XmlCvtIdToStr(ID_INITIALIZE_FAIL))
#define USE_DEFAULT_MENU                        (char *)(Lib_XmlCvtIdToStr(ID_USE_DEFAULT_MENU))
#define DEV_MODEL                               (char *)(Lib_XmlCvtIdToStr(ID_DEV_MODEL))
#define UPGRADE_SUCCESS                         (char *)(Lib_XmlCvtIdToStr(ID_UPGRADE_SUCCESS))
#define SYSTEM_INIT                             (char *)(Lib_XmlCvtIdToStr(ID_SYSTEM_INIT))

// main menu
#define MENU_MAIN                               (char *)(Lib_XmlCvtIdToStr(ID_MENU_MAIN))
#define MENU_PAY                                (char *)(Lib_XmlCvtIdToStr(ID_MENU_PAY))
#define MENU_RKI                                (char *)"RKI"//(Lib_XmlCvtIdToStr(ID_MENU_RKI)
#define MENU_MQTT                               (char *)"MQTT Info"

#define MENU_REFUND                             (char *)(Lib_XmlCvtIdToStr(ID_MENU_REFUND))
#define MENU_SCAN                               (char *)(Lib_XmlCvtIdToStr(ID_MENU_SCAN))
#define MENU_TEST                               (char *)(Lib_XmlCvtIdToStr(ID_MENU_TEST))
#define MENU_MANAGE                             (char *)(Lib_XmlCvtIdToStr(ID_MENU_MANAGE))
#define MENU_PRINT                              (char *)(Lib_XmlCvtIdToStr(ID_MENU_PRINT))
#define MENU_SETTING                            (char *)(Lib_XmlCvtIdToStr(ID_MENU_SETTING))
#define MENU_MORE                               (char *)(Lib_XmlCvtIdToStr(ID_MENU_MORE))
#define MENU_ESIGN                              (char *)"E-Sign"

// TEST MENU
#define MENU_TEST_TITLE                         (char *)(Lib_XmlCvtIdToStr(ID_MENU_TEST_TITLE))
#define MENU_ONLINE                             (char *)(Lib_XmlCvtIdToStr(ID_MENU_ONLINE))
#define MENU_ONLINESET                          (char *)(Lib_XmlCvtIdToStr(ID_MENU_ONLINESET))
#define MENU_CONNECTION_MODE                    (char *)(Lib_XmlCvtIdToStr(ID_MENU_CONNECTION_MODE))

// Test submenu
#define MENU_TEST_SUBTITLE                      (char *)(Lib_XmlCvtIdToStr(ID_MENU_TEST_SUBTITLE))
#define MENU_NET_MODE_SETTING                   (char *)(Lib_XmlCvtIdToStr(ID_MENU_NET_MODE_SETTING))
#define MENU_APN_DNS_SETTING                    (char *)(Lib_XmlCvtIdToStr(ID_MENU_APN_DNS_SETTING))
#define MENU_OPERATOR_LIST                      (char *)(Lib_XmlCvtIdToStr(ID_MENU_OPERATOR_LIST))
#define MENU_TCP                                (char *)(Lib_XmlCvtIdToStr(ID_MENU_TCP))
#define MENU_SSL                                (char *)(Lib_XmlCvtIdToStr(ID_MENU_SSL))

// Echo Test Menu
#define MENU_NETWORK_TEST                       (char *)(Lib_XmlCvtIdToStr(ID_MENU_NETWORK_TEST))
#define MENU_SINGLE_ECHO                        (char *)(Lib_XmlCvtIdToStr(ID_MENU_SINGLE_ECHO))
#define MENU_SHORT_LINK                         (char *)(Lib_XmlCvtIdToStr(ID_MENU_SHORT_LINK))
#define MENU_LONG_LINK                          (char *)(Lib_XmlCvtIdToStr(ID_MENU_LONG_LINK))
#define MENU_RESTART_LINK                       (char *)(Lib_XmlCvtIdToStr(ID_MENU_RESTART_LINK))
#define MENU_COMBINATION                        (char *)(Lib_XmlCvtIdToStr(ID_MENU_COMBINATION))

// GPRS Setting
#define MENU_SET_GPRS_MODE                      (char *)(Lib_XmlCvtIdToStr(ID_MENU_SET_GPRS_MODE))
#define MENU_GPRS                               (char *)(Lib_XmlCvtIdToStr(ID_MENU_GPRS))
#define MENU_WIFI                               (char *)(Lib_XmlCvtIdToStr(ID_MENU_WIFI))
#define MENU_WIFI_PRIOR                         (char *)(Lib_XmlCvtIdToStr(ID_MENU_WIFI_PRIOR))
#define MENU_MODE                               (char *)(Lib_XmlCvtIdToStr(ID_MENU_MODE))

// Wifi Setting
#define MENU_NETWORK_SETTING                    (char *)(Lib_XmlCvtIdToStr(ID_MENU_NETWORK_SETTING))
#define MENU_SELECT_MODE                        (char *)(Lib_XmlCvtIdToStr(ID_MENU_SELECT_MODE))
#define MENU_CONNECT_WIFI                       (char *)(Lib_XmlCvtIdToStr(ID_MENU_CONNECT_WIFI))

// management menu
#define MENU_MANAGE_TITLE                       (char *)(Lib_XmlCvtIdToStr(ID_MENU_MANAGE_TITLE))
#define MENU_LOG_IN                             (char *)(Lib_XmlCvtIdToStr(ID_MENU_LOG_IN))
#define MENU_LOG_OUT                            (char *)(Lib_XmlCvtIdToStr(ID_MENU_LOG_OUT))
#define MENU_PAY_QUERY                          (char *)(Lib_XmlCvtIdToStr(ID_MENU_PAY_QUERY))
#define MENU_DEV_INFO                           (char *)(Lib_XmlCvtIdToStr(ID_MENU_DEV_INFO))
#define MENU_VER_INFO                           (char *)(Lib_XmlCvtIdToStr(ID_MENU_VER_INFO))
#define MENU_FW_OTA                             (char *)(Lib_XmlCvtIdToStr(ID_MENU_FW_OTA))
#define MENU_TMS_UPGRADE                        (char *)(Lib_XmlCvtIdToStr(ID_MENU_TMS_UPGRADE))

// PRINT MENU
#define MENU_PRINT_TITILE                       (char *)(Lib_XmlCvtIdToStr(ID_MENU_PRINT_TITILE))
#define MENU_PRINT_RECEIPT                      (char *)(Lib_XmlCvtIdToStr(ID_MENU_PRINT_RECEIPT))
#define MENU_CALIB_SCREEN                       (char *)(Lib_XmlCvtIdToStr(ID_MENU_CALIB_SCREEN))
#define MENU_PRINT_ESIG                         (char *)(Lib_XmlCvtIdToStr(ID_MENU_PRINT_ESIG))
#define MENU_ESIGN_TITILE                       (char *)"E-Sign"


// Query transaction history menu
#define MENU_QUERY_TITLE                        (char *)(Lib_XmlCvtIdToStr(ID_MENU_QUERY_TITLE))
#define MENU_QUERY_DETAIL                       (char *)(Lib_XmlCvtIdToStr(ID_MENU_QUERY_DETAIL))
#define MENU_QUERY_TOTAL                        (char *)(Lib_XmlCvtIdToStr(ID_MENU_QUERY_TOTAL))

// OTA menu
#define MENU_OS_UPDATE                          (char *)(Lib_XmlCvtIdToStr(ID_MENU_OS_UPDATE))
#define MENU_WIFI_UPDATE                        (char *)(Lib_XmlCvtIdToStr(ID_MENU_WIFI_UPDATE))
#define MENU_SE_UPDATE                          (char *)(Lib_XmlCvtIdToStr(ID_MENU_SE_UPDATE))
#define MENU_FILE_UPDATE                        (char *)(Lib_XmlCvtIdToStr(ID_MENU_FILE_UPDATE))
#define MENU_APP_FW_UPDATE                      (char *)(Lib_XmlCvtIdToStr(ID_MENU_APP_FW_UPDATE))
#define MENU_FIRMWARE_OTA                       (char *)(Lib_XmlCvtIdToStr(ID_MENU_FIRMWARE_OTA))
#define MENU_OS_FOTA_UPGRADE                    (char *)(Lib_XmlCvtIdToStr(ID_MENU_OS_FOTA_UPGRADE))
#define MENU_APP_FW_UPGRADE                     (char *)(Lib_XmlCvtIdToStr(ID_MENU_APP_FW_UPGRADE))

// Settings menu
#define MENU_SET_TITLE                          (char *)(Lib_XmlCvtIdToStr(ID_MENU_SET_TITLE))
#define MENU_NET_SETTING                        (char *)(Lib_XmlCvtIdToStr(ID_MENU_NET_SETTING))
#define MENU_BRIGHTNESS                         (char *)(Lib_XmlCvtIdToStr(ID_MENU_BRIGHTNESS))
#define MENU_SYSTIME_SETTING                    (char *)(Lib_XmlCvtIdToStr(ID_MENU_SYSTIME_SETTING))
#define MENU_BEEP                               (char *)(Lib_XmlCvtIdToStr(ID_MENU_BEEP))
#define MENU_THEME_COLOR                        (char *)(Lib_XmlCvtIdToStr(ID_MENU_THEME_COLOR))
#define MENU_LANGUAGE_SETTING                   (char *)(Lib_XmlCvtIdToStr(ID_MENU_LANGUAGE_SETTING))
#define MENU_LAYER_DEMO                         (char *)(Lib_XmlCvtIdToStr(ID_MENU_LAYER_DEMO))
#define MENU_POWER_MANAGE                       (char *)(Lib_XmlCvtIdToStr(ID_MENU_POWER_MANAGE))
#define MENU_BACKLIGHT_TIME                     (char *)(Lib_XmlCvtIdToStr(ID_MENU_BACKLIGHT_TIME))
#define MENU_SLEEP_TIME                         (char *)(Lib_XmlCvtIdToStr(ID_MENU_SLEEP_TIME))
#define MENU_POWEROFF_TIME                      (char *)(Lib_XmlCvtIdToStr(ID_MENU_POWEROFF_TIME))

// system time setting
#define MENU_DATE_AND_TIME                      (char *)(Lib_XmlCvtIdToStr(ID_MENU_DATE_AND_TIME))
#define MENU_TIME_ZONE                          (char *)(Lib_XmlCvtIdToStr(ID_MENU_TIME_ZONE))
#define MENU_NTP                                (char *)(Lib_XmlCvtIdToStr(ID_MENU_NTP))

//NTP
#define MENU_NTP_SERVER                         (char *)(Lib_XmlCvtIdToStr(ID_MENU_NTP_SERVER))
#define MENU_NTP_SYNC                           (char *)(Lib_XmlCvtIdToStr(ID_MENU_NTP_SYNC))
#define MENU_NTP_AUTO_SYNC                      (char *)(Lib_XmlCvtIdToStr(ID_MENU_NTP_AUTO_SYNC))

#define MENU_NTP_SYNC_SWITCH                    (char *)(Lib_XmlCvtIdToStr(ID_MENU_NTP_SYNC_SWITCH))
#define MENU_NTP_SYNC_SETTING                   (char *)(Lib_XmlCvtIdToStr(ID_MENU_NTP_SYNC_SETTING))

// POSP Setting
#define MENU_POSP                               (char *)(Lib_XmlCvtIdToStr(ID_MENU_POSP))
#define MENU_POSP_MODE                          (char *)(Lib_XmlCvtIdToStr(ID_MENU_POSP_MODE))
#define MENU_TMS                                (char *)(Lib_XmlCvtIdToStr(ID_MENU_TMS))
#define MENU_TMS_MODE                           (char *)(Lib_XmlCvtIdToStr(ID_MENU_TMS_MODE))
#define MENU_NETWORK                            (char *)(Lib_XmlCvtIdToStr(ID_MENU_NETWORK))

// TMS server setting
#define MENU_TMS_APP_SERVER                     (char *)(Lib_XmlCvtIdToStr(ID_MENU_TMS_APP_SERVER))
#define MENU_TMS_APP_SERVER_MODE                (char *)(Lib_XmlCvtIdToStr(ID_MENU_TMS_APP_SERVER_MODE))
#define MENU_TMS_OTHER_SERVER                   (char *)(Lib_XmlCvtIdToStr(ID_MENU_TMS_OTHER_SERVER))

// Theme Settings
#define MENU_THEME_TITLE                        (char *)(Lib_XmlCvtIdToStr(ID_MENU_THEME_TITLE))
#define MENU_THEME_BLUE                         (char *)(Lib_XmlCvtIdToStr(ID_MENU_THEME_BLUE))
#define MENU_THEME_ORANGE                       (char *)(Lib_XmlCvtIdToStr(ID_MENU_THEME_ORANGE))
#define MENU_THEME_RED                          (char *)(Lib_XmlCvtIdToStr(ID_MENU_THEME_RED))

// APN Setting
#define MENU_APN                                (char *)(Lib_XmlCvtIdToStr(ID_MENU_APN))
#define MENU_DNS                                (char *)(Lib_XmlCvtIdToStr(ID_MENU_DNS))
#define MENU_APN_DNS                            (char *)(Lib_XmlCvtIdToStr(ID_MENU_APN_DNS))
#define MENU_APN_SET                            (char *)(Lib_XmlCvtIdToStr(ID_MENU_APN_SET))
#define MENU_DNS_SET                            (char *)(Lib_XmlCvtIdToStr(ID_MENU_DNS_SET))

//RKI
#define MENU_RKI_GET_CERT                       (char *)"GetCert"           //(Lib_XmlCvtIdToStr(ID_MENU_RKI_GETCERT))
#define MENU_RKI_BIND                           (char *)"Bind"              //(Lib_XmlCvtIdToStr(ID_MENU_RKI_BIND))
#define MENU_RKI_KEY                            (char *)"Key Transport"     //(Lib_XmlCvtIdToStr(ID_MENU_RKI_KEY))
#define MENU_RKI_UNBIND                         (char *)"UnBind"            //(Lib_XmlCvtIdToStr(ID_MENU_RKI_UNBIND))
#define MENU_RKI_REBIND                         (char *)"ReBind"            //(Lib_XmlCvtIdToStr(ID_MENU_RKI_REBIND))


// USB Setting
#define MENU_USB_CCID                           (char *)(Lib_XmlCvtIdToStr(ID_MENU_USB_CCID))
#define MENU_USB_COM                            (char *)(Lib_XmlCvtIdToStr(ID_MENU_USB_COM))

// More
#define MENU_MAINTENANCE                        (char *)(Lib_XmlCvtIdToStr(ID_MENU_MAINTENANCE))
#define MENU_SELF_CHECK                         (char *)(Lib_XmlCvtIdToStr(ID_MENU_SELF_CHECK))
#define MENU_DEBUG_SWITCH                       (char *)(Lib_XmlCvtIdToStr(ID_MENU_DEBUG_SWITCH))
#define MENU_USB_MODE                           (char *)(Lib_XmlCvtIdToStr(ID_MENU_USB_MODE))

// Single item self check menu
#define MENU_DEV_CHECK                          (char *)(Lib_XmlCvtIdToStr(ID_MENU_DEV_CHECK))
#define MENU_FEATURE_CHECK                      (char *)(Lib_XmlCvtIdToStr(ID_MENU_FEATURE_CHECK))
#define MENU_AGING_CHECK                        (char *)(Lib_XmlCvtIdToStr(ID_MENU_AGING_CHECK))
#define MENU_CHECK_DISP                         (char *)(Lib_XmlCvtIdToStr(ID_MENU_CHECK_DISP))
#define MENU_CHECK_KEYS                         (char *)(Lib_XmlCvtIdToStr(ID_MENU_CHECK_KEYS))
#define MENU_CHECK_PICC                         (char *)(Lib_XmlCvtIdToStr(ID_MENU_CHECK_PICC))
#define MENU_CHECK_MCR                          (char *)(Lib_XmlCvtIdToStr(ID_MENU_CHECK_MCR))
#define MENU_CHECK_ICC                          (char *)(Lib_XmlCvtIdToStr(ID_MENU_CHECK_ICC))
#define MENU_CHECK_CHG                          (char *)(Lib_XmlCvtIdToStr(ID_MENU_CHECK_CHG))
#define MENU_CHECK_SCAN                         (char *)(Lib_XmlCvtIdToStr(ID_MENU_CHECK_SCAN))
#define MENU_CHECK_PRN                          (char *)(Lib_XmlCvtIdToStr(ID_MENU_CHECK_PRN))
#define MENU_CHECK_NET                          (char *)(Lib_XmlCvtIdToStr(ID_MENU_CHECK_NET))
#define MENU_CHECK_WIFI                         (char *)(Lib_XmlCvtIdToStr(ID_MENU_CHECK_WIFI))
#define MENU_FUNCTION_TEST                      "Function Test"

// System prompt message
#define TIP_SYS_INIT                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_SYS_INIT))
#define TIP_BAT_LOW_TO_CHG                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_BAT_LOW_TO_CHG))
#define TIP_BAT_LOW_SHUT_DOWN                   (char *)(Lib_XmlCvtIdToStr(ID_TIP_BAT_LOW_SHUT_DOWN))
#define TIP_IS_SHUT_DOWN                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_IS_SHUT_DOWN))

// Payment reminder message
#define TIP_ENTER_AMOUNT                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_ENTER_AMOUNT))
#define TIP_SHOP_NAME                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_SHOP_NAME))
#define TIP_TRADE_OK                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_TRADE_OK))
#define TIP_AMOUNT                              (char *)(Lib_XmlCvtIdToStr(ID_TIP_AMOUNT))
#define TIP_PRINTING                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_PRINTING))
#define TIP_AMOUNT_OVER                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_AMOUNT_OVER))
#define TIP_CONFIRM_AMOUNT                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_CONFIRM_AMOUNT))
#define TIP_PAY_BY_CARD                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_PAY_BY_CARD))
#define TIP_PULL_OUT_CARD                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_PULL_OUT_CARD))
#define TIP_RECORDS_FULL                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_RECORDS_FULL))
#define TIP_QR_TRADE_SUCC                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_QR_TRADE_SUCC))

// Purchase Order Prompt message
#define TIP_RECEIPT                             (char *)(Lib_XmlCvtIdToStr(ID_TIP_RECEIPT))
#define TIP_MERCHANT_ID                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_MERCHANT_ID))
#define TIP_MERCHANT_NO                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_MERCHANT_NO))
#define TIP_TERMINAL_ID                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_TERMINAL_ID))
#define TIP_CASHIER_NO                          (char *)(Lib_XmlCvtIdToStr(ID_TIP_CASHIER_NO))
#define TIP_TOTAL                               (char *)(Lib_XmlCvtIdToStr(ID_TIP_TOTAL))
#define TIP_PAY_TYPE                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_PAY_TYPE))
#define TIP_DATE_TIME                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_DATE_TIME))
#define TIP_PAY_CARD                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_PAY_CARD))
#define TIP_PAY_SCAN                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_PAY_SCAN))
#define TIP_PAY_UNKNOW                          (char *)(Lib_XmlCvtIdToStr(ID_TIP_PAY_UNKNOW))
#define TIP_PURCHASE                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_PURCHASE))

// Network prompt message
#define TIP_SEARCH_NET_OPER                     (char *)(Lib_XmlCvtIdToStr(ID_TIP_SEARCH_NET_OPER))
#define TIP_SEARCH_SUCC                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_SEARCH_SUCC))
#define TIP_SEARCH_FAIL                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_SEARCH_FAIL))
#define TIP_NO_SIM_CARD                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_NO_SIM_CARD))
#define TIP_NET_SEND_DATA                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_NET_SEND_DATA))
#define TIP_NET_RECV_DATA                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_NET_RECV_DATA))
#define TIP_NET_INIT                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_NET_INIT))
#define TIP_NET_INIT_TIMEOUT                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_NET_INIT_TIMEOUT))
#define TIP_NET_CONNECT                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_NET_CONNECT))
#define TIP_NET_CONNECT_TIMEOUT                 (char *)(Lib_XmlCvtIdToStr(ID_TIP_NET_CONNECT_TIMEOUT))
#define TIP_NET_CONNECT_FAIL                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_NET_CONNECT_FAIL))
#define TIP_NET_RESTART                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_NET_RESTART))
#define TIP_NET_READ_DATA                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_NET_READ_DATA))
#define TIP_NET_READ_TIMEOUT                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_NET_READ_TIMEOUT))
#define TIP_NET_WIFI_ABNORMAL                   (char *)(Lib_XmlCvtIdToStr(ID_TIP_NET_WIFI_ABNORMAL))
#define TIP_NET_SWITCH_GPRS                     (char *)(Lib_XmlCvtIdToStr(ID_TIP_NET_SWITCH_GPRS))

// WiFi prompt message
#define TIP_WIFI_INFO                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_INFO))
#define TIP_WIFI_INFO_GET_FAIL                  (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_INFO_GET_FAIL))
#define TIP_WIFI_STATIC_IP                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_STATIC_IP))
#define TIP_WIFI_DYNAMIC_IP                     (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_DYNAMIC_IP))
#define TIP_WIFI_GATEWAY                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_GATEWAY))
#define TIP_WIFI_DNS1                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_DNS1))
#define TIP_WIFI_IP                             (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_IP))
#define TIP_WIFI_SUBWAY                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_SUBWAY))
#define TIP_WIFI_DNS2                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_DNS2))
#define TIP_WIFI_MAC                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_MAC))
#define TIP_WIFI_BID                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_BID))
#define TIP_WIFI_SEARCH                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_SEARCH))
#define TIP_WIFI_GET_LIST                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_GET_LIST))
#define TIP_WIFI_NO_AVAILABLE                   (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_NO_AVAILABLE))
#define TIP_WIFI_NAME_UNAVAILABLE               (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_NAME_UNAVAILABLE))
#define TIP_WIFI_AP_LIST                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_AP_LIST))
#define TIP_WIFI_FORGET_NETWORK                 (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_FORGET_NETWORK))
#define TIP_WIFI_DEL_SUCC                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_DEL_SUCC))
#define TIP_WIFI_INPUT_SSID                     (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_INPUT_SSID))
#define TIP_WIFI_INPUT_PASS                     (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_INPUT_PASS))
#define TIP_WIFI_SSID                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_SSID))
#define TIP_WIFI_PASS                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_PASS))
#define TIP_WIFI_CONNECT_SUCC                   (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_CONNECT_SUCC))
#define TIP_WIFI_CONNECT_FAIL                   (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_CONNECT_FAIL))
#define TIP_WIFI_PLEASE_CHECK                   (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_PLEASE_CHECK))
#define TIP_WIFI_UNABLE_CONNECT                 (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_UNABLE_CONNECT))
#define TIP_WIFI_CONNECTIONG                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_CONNECTIONG))

// Echo test prompt message
#define TIP_ONCE_LINK                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_ONCE_LINK))
#define TIP_SHORT_LINK                          (char *)(Lib_XmlCvtIdToStr(ID_TIP_SHORT_LINK))
#define TIP_LONG_LINK                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_LONG_LINK))
#define TIP_RESTART_LINK                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_RESTART_LINK))
#define TIP_COMBINATION_TEST                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_COMBINATION_TEST))
#define TIP_SET_TIMES                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_SET_TIMES))
#define TIP_RESTART_FAIL                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_RESTART_FAIL))
#define TIP_INIT_FAIL                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_INIT_FAIL))
#define TIP_CONNECT_FAIL                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_CONNECT_FAIL))
#define TIP_SEND_FAIL                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_SEND_FAIL))
#define TIP_LINK_FAIL                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_LINK_FAIL))
#define TIP_RECV_FAIL                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_RECV_FAIL))
#define TIP_ECHO_SUCC                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_ECHO_SUCC))
#define TIP_ECHO_FAIL                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_ECHO_FAIL))
#define TIP_TIMES_AND_LEN                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_TIMES_AND_LEN))
#define TIP_PASS_AND_FAIL                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_PASS_AND_FAIL))

// Echo test result prompt message
#define TIP_DEVICE_PN                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_DEVICE_PN))
#define TIP_OS_VER                              (char *)(Lib_XmlCvtIdToStr(ID_TIP_OS_VER))
#define TIP_SDK_VER                             (char *)(Lib_XmlCvtIdToStr(ID_TIP_SDK_VER))
#define TIP_APP_VER                             (char *)(Lib_XmlCvtIdToStr(ID_TIP_APP_VER))
#define TIP_SE_VER                              (char *)(Lib_XmlCvtIdToStr(ID_TIP_SE_VER))
#define TIP_START_TIME                          (char *)(Lib_XmlCvtIdToStr(ID_TIP_START_TIME))
#define TIP_END_TIME                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_END_TIME))
#define TIP_NET_OPERATOR                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_NET_OPERATOR))
#define TIP_NET_MODE                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_NET_MODE))
#define TIP_NET_MODE_AUTO                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_NET_MODE_AUTO))
#define TIP_NET_MODE_2G                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_NET_MODE_2G))
#define TIP_NET_MODE_4G                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_NET_MODE_4G))
#define TIP_NUM_OF_TEST                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_NUM_OF_TEST))
#define TIP_VARIABLE_LEN                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_VARIABLE_LEN))
#define TIP_VARIABLE_RANGE                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_VARIABLE_RANGE))
#define TIP_SIGNAL_RANGE                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_SIGNAL_RANGE))
#define TIP_SIGNAL_STRENGTH                     (char *)(Lib_XmlCvtIdToStr(ID_TIP_SIGNAL_STRENGTH))
#define TIP_NUM_OF_PASS                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_NUM_OF_PASS))
#define TIP_NUM_OF_INIT_FAIL                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_NUM_OF_INIT_FAIL))
#define TIP_NUM_OF_LINK_FAIL                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_NUM_OF_LINK_FAIL))
#define TIP_NUM_OF_SEND_FAIL                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_NUM_OF_SEND_FAIL))
#define TIP_NUM_OF_RECV_FAIL                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_NUM_OF_RECV_FAIL))
#define TIP_NUM_OF_COMP_FAIL                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_NUM_OF_COMP_FAIL))
#define TIP_TEST_DURATION                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_TEST_DURATION))
#define TIP_TEST_CITY                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_TEST_CITY))

// Management prompt message
#define TIP_LOG_IN_SUCC                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_LOG_IN_SUCC))
#define TIP_NO_RECORD                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_NO_RECORD))
#define TIP_MONEY                               (char *)(Lib_XmlCvtIdToStr(ID_TIP_MONEY))
#define TIP_STATUS                              (char *)(Lib_XmlCvtIdToStr(ID_TIP_STATUS))
#define TIP_PAY_OK                              (char *)(Lib_XmlCvtIdToStr(ID_TIP_PAY_OK))
#define TIP_OPCODE                              (char *)(Lib_XmlCvtIdToStr(ID_TIP_OPCODE))
#define TIP_TRANS_NO                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_TRANS_NO))
#define TIP_TIME                                (char *)(Lib_XmlCvtIdToStr(ID_TIP_TIME))
#define TIP_OTHERS                              (char *)(Lib_XmlCvtIdToStr(ID_TIP_OTHERS))

// Device information prompt message
#define TIP_DEV_INFO                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_DEV_INFO))
#define TIP_MODEL                               (char *)(Lib_XmlCvtIdToStr(ID_TIP_MODEL))
#define TIP_SN                                  (char *)(Lib_XmlCvtIdToStr(ID_TIP_SN))
#define TIP_IMEI                                (char *)(Lib_XmlCvtIdToStr(ID_TIP_IMEI))
#define TIP_POWER                               (char *)(Lib_XmlCvtIdToStr(ID_TIP_POWER))

// Version information prompt message
#define TIP_OS                                  (char *)(Lib_XmlCvtIdToStr(ID_TIP_OS))
#define TIP_SDK                                 (char *)(Lib_XmlCvtIdToStr(ID_TIP_SDK))
#define TIP_APP                                 (char *)(Lib_XmlCvtIdToStr(ID_TIP_APP))
#define TIP_SE                                  (char *)(Lib_XmlCvtIdToStr(ID_TIP_SE))
#define TIP_HW                                  (char *)(Lib_XmlCvtIdToStr(ID_TIP_HW))
#define TIP_WIFI                                (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI))

// Upgrade prompt message
#define TIP_OTA_FAIL                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_OTA_FAIL))
#define TIP_OTA_CMD_VER_SEND_FAIL               (char *)(Lib_XmlCvtIdToStr(ID_TIP_OTA_CMD_VER_SEND_FAIL))
#define TIP_OTA_CMD_VER_RECV_TIMEOUT            (char *)(Lib_XmlCvtIdToStr(ID_TIP_OTA_CMD_VER_RECV_TIMEOUT))
#define TIP_OTA_VER_CHECK_FAIL                  (char *)(Lib_XmlCvtIdToStr(ID_TIP_OTA_VER_CHECK_FAIL))
#define TIP_FS_FREE_SIZE_NOT_ENOUGH             (char *)(Lib_XmlCvtIdToStr(ID_TIP_FS_FREE_SIZE_NOT_ENOUGH))
#define TIP_FW_FILE_OPEN_FAIL                   (char *)(Lib_XmlCvtIdToStr(ID_TIP_FW_FILE_OPEN_FAIL))
#define TIP_OTA_CMD_DOWNLOAD_FAIL               (char *)(Lib_XmlCvtIdToStr(ID_TIP_OTA_CMD_DOWNLOAD_FAIL))
#define TIP_CMD_DOWNLOAD_SEND_FAIL              (char *)(Lib_XmlCvtIdToStr(ID_TIP_CMD_DOWNLOAD_SEND_FAIL))
#define TIP_CMD_RECV_DOWNLOAD_TIMEOUT           (char *)(Lib_XmlCvtIdToStr(ID_TIP_CMD_RECV_DOWNLOAD_TIMEOUT))
#define TIP_CMD_DEPACAGE_DOWNLOAD_FAIL          (char *)(Lib_XmlCvtIdToStr(ID_TIP_CMD_DEPACAGE_DOWNLOAD_FAIL))
#define TIP_RECV_FILE_OFFSET_ERR                (char *)(Lib_XmlCvtIdToStr(ID_TIP_RECV_FILE_OFFSET_ERR))
#define TIP_PACK_SIZE_ERR                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_PACK_SIZE_ERR))
#define TIP_FW_FILE_WRITE_FAIL                  (char *)(Lib_XmlCvtIdToStr(ID_TIP_FW_FILE_WRITE_FAIL))
#define TIP_OTA_EXIT                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_OTA_EXIT))
#define TIP_OTA_CMD_FINISH_PACK_FAIL            (char *)(Lib_XmlCvtIdToStr(ID_TIP_OTA_CMD_FINISH_PACK_FAIL))
#define TIP_OTA_CMD_FINISH_SEND_FAIL            (char *)(Lib_XmlCvtIdToStr(ID_TIP_OTA_CMD_FINISH_SEND_FAIL))
#define TIP_DOWNLOAD_FINISH                     (char *)(Lib_XmlCvtIdToStr(ID_TIP_DOWNLOAD_FINISH))
#define TIP_SETUP_AND_WAIT                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_SETUP_AND_WAIT))
#define TIP_SETUP_FAIL                          (char *)(Lib_XmlCvtIdToStr(ID_TIP_SETUP_FAIL))
#define TIP_TOTAL_SIZE                          (char *)(Lib_XmlCvtIdToStr(ID_TIP_TOTAL_SIZE))
#define TIP_DOWNLOAD_SIZE                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_DOWNLOAD_SIZE))

//TMS related prompt message
#define TIP_TMS_TASK_REQUEST                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_TASK_REQUEST))
#define TIP_TMS_SOFT_DOWNLOAD                   (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_SOFT_DOWNLOAD))
#define TIP_TMS_DOWNLOAD_DATA_ERR               (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_DOWNLOAD_DATA_ERR))
#define TIP_TMS_NO_SOFT_TO_DOWNLOAD             (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_NO_SOFT_TO_DOWNLOAD))
#define TIP_TMS_CHECK_SOFT                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_CHECK_SOFT))
#define TIP_TMS_FIRM_APP_DATA_ERR               (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_FIRM_APP_DATA_ERR))
#define TIP_TMS_PARA_DATA_ERR                   (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_PARA_DATA_ERR))
#define TIP_TMS_DOWNLOAD_CONFIRM                (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_DOWNLOAD_CONFIRM))
#define TIP_TMS_DOWNLOAD_PROGRESS               (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_DOWNLOAD_PROGRESS))
#define TIP_TMS_DOWNLOAD_DONE                   (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_DOWNLOAD_DONE))
#define TIP_TMS_SOFT_UP_TO_DATE                 (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_SOFT_UP_TO_DATE))
#define TIP_TMS_PARA_DOWNLOAD                   (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_PARA_DOWNLOAD))
#define TIP_TMS_PARA_UP_TO_DATE                 (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_PARA_UP_TO_DATE))
#define TIP_TMS_PARA_DOWNLOAD_DONE              (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_PARA_DOWNLOAD_DONE))
#define TIP_TMS_READY_STORAGE_SPACE             (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_READY_STORAGE_SPACE))
#define TIP_TMS_KEEP_UPGRADE                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_KEEP_UPGRADE))
#define TIP_TMS_WAIT_REBOOT                     (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_WAIT_REBOOT))
#define TIP_TMS_NONE_MODULE_INFO                (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_NONE_MODULE_INFO))
#define TIP_TMS_GET_MODULE_INFO                 (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_GET_MODULE_INFO))
#define TIP_TMS_MODULE_DOWNLOAD                 (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_MODULE_DOWNLOAD))
#define TIP_TMS_MODULE_UP_TO_DATE               (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_MODULE_UP_TO_DATE))
#define TIP_TMS_MODULE_VER_NO_MATCH             (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_MODULE_VER_NO_MATCH))
#define TIP_TMS_MODULE_DATA_ERR                 (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_MODULE_DATA_ERR))

// Set prompt message message
#define TIP_DATE_AND_TIME                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_DATE_AND_TIME))
#define TIP_APN_SET                             (char *)(Lib_XmlCvtIdToStr(ID_TIP_APN_SET))
#define TIP_APN_USERNAME_SET                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_APN_USERNAME_SET))
#define TIP_APN_PASSWORD_SET                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_APN_PASSWORD_SET))
#define TIP_DNS_MAIN                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_DNS_MAIN))
#define TIP_DNS_BACK                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_DNS_BACK))
#define TIP_SERVER_IP_MAIN                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_SERVER_IP_MAIN))
#define TIP_SERVER_PORT_MAIN                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_SERVER_PORT_MAIN))
#define TIP_SERVER_IP_BACK                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_SERVER_IP_BACK))
#define TIP_SERVER_PORT_BACK                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_SERVER_PORT_BACK))
#define TIP_DOMAIN_NAME_MAIN                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_DOMAIN_NAME_MAIN))
#define TIP_DOMAIN_PORT_MAIN                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_DOMAIN_PORT_MAIN))
#define TIP_DOMAIN_NAME_BACK                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_DOMAIN_NAME_BACK))
#define TIP_DOMAIN_PORT_BACK                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_DOMAIN_PORT_BACK))
#define TIP_POSP_CFG                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_POSP_CFG))
#define TIP_TMS_SERVER_IP_MAIN                  (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_SERVER_IP_MAIN))
#define TIP_TMS_SERVER_PORT_MAIN                (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_SERVER_PORT_MAIN))
#define TIP_TMS_SERVER_IP_BACK                  (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_SERVER_IP_BACK))
#define TIP_TMS_SERVER_PORT_BACK                (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_SERVER_PORT_BACK))
#define TIP_TMS_DOMAIN_NAME_MAIN                (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_DOMAIN_NAME_MAIN))
#define TIP_TMS_DOMAIN_PORT_MAIN                (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_DOMAIN_PORT_MAIN))
#define TIP_TMS_DOMAIN_NAME_BACK                (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_DOMAIN_NAME_BACK))
#define TIP_TMS_DOMAIN_PORT_BACK                (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_DOMAIN_PORT_BACK))
#define TIP_TMS_OS_DOMAIN_NAME_MAIN             (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_OS_DOMAIN_NAME_MAIN))
#define TIP_TMS_OS_DOMAIN_PORT_MAIN             (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_OS_DOMAIN_PORT_MAIN))
#define TIP_TMS_CFG                             (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_CFG))
#define TIP_CURRENT_RESULT                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_CURRENT_RESULT))
#define TIP_POSP_MODE_SETTING                   (char *)(Lib_XmlCvtIdToStr(ID_TIP_POSP_MODE_SETTING))
#define TIP_TMS_MODE_SETTING                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_TMS_MODE_SETTING))
#define TIP_IS_USE_DOMAIN                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_IS_USE_DOMAIN))
#define TIP_BEEP_SETTING                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_BEEP_SETTING))
#define TIP_EN_TO_DEBUG_TOOLS                   (char *)(Lib_XmlCvtIdToStr(ID_TIP_EN_TO_DEBUG_TOOLS))
#define TIP_EN_TO_FILE                          (char *)(Lib_XmlCvtIdToStr(ID_TIP_EN_TO_FILE))
#define TIP_EN_TO_UPLOAD_FILE                   (char *)(Lib_XmlCvtIdToStr(ID_TIP_EN_TO_UPLOAD_FILE))
#define TIP_GPRS_IS_SELECT                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_GPRS_IS_SELECT))
#define TIP_WIFI_IS_SELECT                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_IS_SELECT))
#define TIP_GPRS_AND_WIFI_SELECT                (char *)(Lib_XmlCvtIdToStr(ID_TIP_GPRS_AND_WIFI_SELECT))
#define TIP_WIFI_PRIOR                          (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_PRIOR))
#define TIP_UNIT_S                              (char *)(Lib_XmlCvtIdToStr(ID_TIP_UNIT_S))
#define TIP_UNIT_MIN                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_UNIT_MIN))
#define TIP_POWER_CURRENT_TIME                  (char *)(Lib_XmlCvtIdToStr(ID_TIP_POWER_CURRENT_TIME))
#define TIP_POWER_BACKLIGHT_TIME_SETTING        (char *)(Lib_XmlCvtIdToStr(ID_TIP_POWER_BACKLIGHT_TIME_SETTING))
#define TIP_POWER_SLEEP_TIME_SETTING            (char *)(Lib_XmlCvtIdToStr(ID_TIP_POWER_SLEEP_TIME_SETTING))
#define TIP_POWER_POWEROFF_TIME_SETTING         (char *)(Lib_XmlCvtIdToStr(ID_TIP_POWER_POWEROFF_TIME_SETTING))
#define TIP_GPRS_APN_INFO                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_GPRS_APN_INFO))
#define TIP_GPRS_APN_AUTH_MATCH                 (char *)(Lib_XmlCvtIdToStr(ID_TIP_GPRS_APN_AUTH_MATCH))
#define TIP_GPRS_APN_IP                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_GPRS_APN_IP))
#define TIP_GPRS_APN_IPV6                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_GPRS_APN_IPV6))
#define TIP_GPRS_APN_IPV4V6                     (char *)(Lib_XmlCvtIdToStr(ID_TIP_GPRS_APN_IPV4V6))
#define TIP_GPRS_APN_NONE                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_GPRS_APN_NONE))
#define TIP_GPRS_APN_PAP                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_GPRS_APN_PAP))
#define TIP_GPRS_APN_CHAP                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_GPRS_APN_CHAP))
#define TIP_GPRS_APN_PROCOTOL                   (char *)(Lib_XmlCvtIdToStr(ID_TIP_GPRS_APN_PROCOTOL))
#define TIP_GPRS_APN_AUTH_TYPE                  (char *)(Lib_XmlCvtIdToStr(ID_TIP_GPRS_APN_AUTH_TYPE))
#define TIP_GPRS_APN_LOADING                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_GPRS_APN_LOADING))
#define TIP_GPRS_APN_FUN                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_GPRS_APN_FUN))
#define TIP_GPRS_APN_MAN                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_GPRS_APN_MAN))
#define TIP_GPRS_APN_LIST                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_GPRS_APN_LIST))
#define TIP_APN_PROTOCOL_SET                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_APN_PROTOCOL_SET))
#define TIP_APN_AUTH_TYPE_SET                   (char *)(Lib_XmlCvtIdToStr(ID_TIP_APN_AUTH_TYPE_SET))
#define TIP_APN_ENTE_NUM_SELECT                 (char *)(Lib_XmlCvtIdToStr(ID_TIP_APN_ENTE_NUM_SELECT))
#define TIP_SIM_INIT_SUCC                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_SIM_INIT_SUCC))
#define TIP_INSERT_SIM_CARD                     (char *)(Lib_XmlCvtIdToStr(ID_TIP_INSERT_SIM_CARD))


// LCD and LED self-test prompt message
#define TIP_CHECK_FLASH_LAMP                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_CHECK_FLASH_LAMP))
#define TIP_ENTER_CONTINUE                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_ENTER_CONTINUE))
#define TIP_CANCEL_EXIT                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_CANCEL_EXIT))
#define TIP_COLOR_BLUE                          (char *)(Lib_XmlCvtIdToStr(ID_TIP_COLOR_BLUE))
#define TIP_COLOR_YELLOW                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_COLOR_YELLOW))
#define TIP_COLOR_GREEN                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_COLOR_GREEN))
#define TIP_COLOR_RED                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_COLOR_RED))
#define TIP_CUR_LED_COLOR                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_CUR_LED_COLOR))
#define TIP_TURN_ON_LED_2S                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_TURN_ON_LED_2S))
#define TIP_LED_TEST_END                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_LED_TEST_END))

// Keys self check prompt message
#define TIP_KB_UP                               (char *)(Lib_XmlCvtIdToStr(ID_TIP_KB_UP))
#define TIP_KB_DOWN                             (char *)(Lib_XmlCvtIdToStr(ID_TIP_KB_DOWN))
#define TIP_KB_MENU                             (char *)(Lib_XmlCvtIdToStr(ID_TIP_KB_MENU))
#define TIP_KB_FUN                              (char *)(Lib_XmlCvtIdToStr(ID_TIP_KB_FUN))
#define TIP_KB_CLEAR                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_KB_CLEAR))
#define TIP_KB_ENTER                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_KB_ENTER))
#define TIP_KB_CANCEL                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_KB_CANCEL))
#define TIP_KB_ZM                               (char *)(Lib_XmlCvtIdToStr(ID_TIP_KB_ZM))
#define TIP_PRESS                               (char *)(Lib_XmlCvtIdToStr(ID_TIP_PRESS))
#define TIP_KEY                                 (char *)(Lib_XmlCvtIdToStr(ID_TIP_KEY))
#define TIP_IS_BEEP_OK                          (char *)(Lib_XmlCvtIdToStr(ID_TIP_IS_BEEP_OK))
#define TIP_ENTER_CANCLE                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_ENTER_CANCLE))

// Contactless Card self check prompt message
#define TIP_NFC_TEST                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_NFC_TEST))
#define TIP_M_TIMES_N_SUCC                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_M_TIMES_N_SUCC))
#define TIP_SWIPE_PICC_CARD                     (char *)(Lib_XmlCvtIdToStr(ID_TIP_SWIPE_PICC_CARD))
#define TIP_MOVE_PICC_CARD                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_MOVE_PICC_CARD))
#define TIP_CANCLE_TO_NEXT                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_CANCLE_TO_NEXT))

// Magnetic stripe card self check prompt message
#define TIP_MSC_TEST                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_MSC_TEST))
#define TIP_MSC_N1_N2_N3                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_MSC_N1_N2_N3))
#define TIP_MSC_TEST_3_TIMES                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_MSC_TEST_3_TIMES))
#define TIP_MSC_TRACK2_FAIL                     (char *)(Lib_XmlCvtIdToStr(ID_TIP_MSC_TRACK2_FAIL))
#define TIP_MSC_TRACK3_FAIL                     (char *)(Lib_XmlCvtIdToStr(ID_TIP_MSC_TRACK3_FAIL))
#define TIP_MSC_TIMEOUT                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_MSC_TIMEOUT))
#define TIP_TEST_N_TIMES                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_TEST_N_TIMES))
#define TIP_REPEAT_NEXT                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_REPEAT_NEXT))

// Contact card self check prompt message
#define TIP_ICC_TEST                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_ICC_TEST))
#define TIP_TOUCH_ICC                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_TOUCH_ICC))
#define TIP_MOVE_ICC                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_MOVE_ICC))

// USB detection prompt message
#define TIP_INSERT_USB                          (char *)(Lib_XmlCvtIdToStr(ID_TIP_INSERT_USB))

// Scan code prompt message
#define TIP_QR_OPEN_FAIL                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_QR_OPEN_FAIL))
#define TIP_QR_PLEASE_SCAN                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_QR_PLEASE_SCAN))
#define TIP_QR_M_SCAN_N_SUCC                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_QR_M_SCAN_N_SUCC))
#define TIP_QR_RESULT_RET                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_QR_RESULT_RET))
#define TIP_QR_CODE_SUCC                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_QR_CODE_SUCC))
#define TIP_QR_CODE_FAIL                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_QR_CODE_FAIL))

// Printer prompt message
#define TIP_PRN_EXIT                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_PRN_EXIT))
#define TIP_PRN_TESTING                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_PRN_TESTING))
#define TIP_PRN_PAPER_OUT                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_PRN_PAPER_OUT))
#define TIP_PRN_PAPER_ADD                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_PRN_PAPER_ADD))
#define TIP_PRN_IS_CLEAR                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_PRN_IS_CLEAR))
#define TIP_PRN_CHECK_SUCC                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_PRN_CHECK_SUCC))
#define TIP_PRN_CHECK_FAIL                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_PRN_CHECK_FAIL))
#define TIP_PRN_OPEN_FAIL                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_PRN_OPEN_FAIL))
#define TIP_PRN_PRINTING                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_PRN_PRINTING))
#define TIP_PRN_ERR                             (char *)(Lib_XmlCvtIdToStr(ID_TIP_PRN_ERR))

// Electronic signature prompt message
#define TIP_TOUCH_SCR_CALIB                     (char *)(Lib_XmlCvtIdToStr(ID_TIP_TOUCH_SCR_CALIB))
#define TIP_SIGN_ERR                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_SIGN_ERR))
#define TIP_SIGN_FAIL                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_SIGN_FAIL))
#define TIP_SIGN_SUCC                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_SIGN_SUCC))

// GPRS prompt message
#define TIP_GPRS_CHECK                          (char *)(Lib_XmlCvtIdToStr(ID_TIP_GPRS_CHECK))
#define TIP_GPRS_TEST_N_TIMES                   (char *)(Lib_XmlCvtIdToStr(ID_TIP_GPRS_TEST_N_TIMES))
#define TIP_GPRS_TEST_STRENGTH                  (char *)(Lib_XmlCvtIdToStr(ID_TIP_GPRS_TEST_STRENGTH))
#define TIP_GPRS_DEF_N_DBM                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_GPRS_DEF_N_DBM))
#define TIP_GPRS_TEST_FAIL                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_GPRS_TEST_FAIL))
#define TIP_GPRS_TEST_SUCC                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_GPRS_TEST_SUCC))
#define TIP_GPRS_SIGNAL_N_DBM                   (char *)(Lib_XmlCvtIdToStr(ID_TIP_GPRS_SIGNAL_N_DBM))

// Wifi prompt message
#define TIP_WIFI_TEST                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_TEST))
#define TIP_WIFI_DISCONNECTED                   (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_DISCONNECTED))
#define TIP_WIFI_CONNECTING                     (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_CONNECTING))
#define TIP_WIFI_CONN_SUCC                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_CONN_SUCC))
#define TIP_WIFI_CONN_FAIL                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_CONN_FAIL))
#define TIP_NET_TIMEOUT                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_NET_TIMEOUT))
#define TIP_WIFI_SIGNAL_N_DBM                   (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_SIGNAL_N_DBM))
#define TIP_WIFI_SIGNAL_TEST_SUCC               (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_SIGNAL_TEST_SUCC))
#define TIP_WIFI_CONN_TCP_SERVER                (char *)(Lib_XmlCvtIdToStr(ID_TIP_WIFI_CONN_TCP_SERVER))
#define TIP_CONN_FAIL                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_CONN_FAIL))
#define TIP_SEND_DATA_FAIL                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_SEND_DATA_FAIL))
#define TIP_TEST_FAIL                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_TEST_FAIL))
#define TIP_TEST_SUCC                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_TEST_SUCC))
#define TIP_ERR_CODE_FAIL                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_ERR_CODE_FAIL))
#define TIP_ERR_CODE_SUCC                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_ERR_CODE_SUCC))

// Machine self inspection prompt message
#define TIP_SELF_CHECK_MODE                     (char *)(Lib_XmlCvtIdToStr(ID_TIP_SELF_CHECK_MODE))
#define TIP_PLEASE_RETURN                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_PLEASE_RETURN))
#define TIP_DEV_CHECK_PASS                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_DEV_CHECK_PASS))
#define TIP_DEV_CHECK_FAIL                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_DEV_CHECK_FAIL))
#define TIP_PLEASE_SHUT_DOWN                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_PLEASE_SHUT_DOWN))

// others
#define TIP_PLEASE_WAIT                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_PLEASE_WAIT))
#define TIP_WAIT_3MINS                          (char *)(Lib_XmlCvtIdToStr(ID_TIP_WAIT_3MINS))
#define TIP_WAITTING_CHA                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_WAITTING_CHA))
#define TIP_OK_CHA                              (char *)(Lib_XmlCvtIdToStr(ID_TIP_OK_CHA))
#define TIP_CFG_CHA                             (char *)(Lib_XmlCvtIdToStr(ID_TIP_CFG_CHA))
#define TIP_DEL_CHA                             (char *)(Lib_XmlCvtIdToStr(ID_TIP_DEL_CHA))
#define TIP_SET_CHA                             (char *)(Lib_XmlCvtIdToStr(ID_TIP_SET_CHA))
#define TIP_CLR_CHA                             (char *)(Lib_XmlCvtIdToStr(ID_TIP_CLR_CHA))
#define TIP_CON_CHA                             (char *)(Lib_XmlCvtIdToStr(ID_TIP_CON_CHA))
#define TIP_UD_CHA                              (char *)(Lib_XmlCvtIdToStr(ID_TIP_UD_CHA))
#define TIP_OK                                  (char *)(Lib_XmlCvtIdToStr(ID_TIP_OK))
#define TIP_ERROR                               (char *)(Lib_XmlCvtIdToStr(ID_TIP_ERROR))
#define TIP_YES                                 (char *)(Lib_XmlCvtIdToStr(ID_TIP_YES))
#define TIP_NO                                  (char *)(Lib_XmlCvtIdToStr(ID_TIP_NO))
#define TIP_ON                                  (char *)(Lib_XmlCvtIdToStr(ID_TIP_ON))
#define TIP_OFF                                 (char *)(Lib_XmlCvtIdToStr(ID_TIP_OFF))
#define TIP_ON_EN                               (char *)(Lib_XmlCvtIdToStr(ID_TIP_ON_EN))
#define TIP_OFF_EN                              (char *)(Lib_XmlCvtIdToStr(ID_TIP_OFF_EN))
#define TIP_NONE                                (char *)(Lib_XmlCvtIdToStr(ID_TIP_NONE))
#define TIP_NUM                                 (char *)(Lib_XmlCvtIdToStr(ID_TIP_NUM))
#define TIP_CHA                                 (char *)(Lib_XmlCvtIdToStr(ID_TIP_CHA))
#define TIP_CHA_NUM                             (char *)(Lib_XmlCvtIdToStr(ID_TIP_CHA_NUM))
#define TIP_ALPHA_INPUT                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_ALPHA_INPUT))
#define TIP_DIGIT_INPUT                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_DIGIT_INPUT))
#define TIP_CURRENT                             (char *)(Lib_XmlCvtIdToStr(ID_TIP_CURRENT))
#define TIP_DATA_ERR                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_DATA_ERR))
#define TIP_SET_SUCC                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_SET_SUCC))
#define TIP_SET_FAIL                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_SET_FAIL))
#define TIP_WAIT_TIMEOUT                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_WAIT_TIMEOUT))
#define TIP_X_TO_EXIT_OTHERS_CONTINUE           (char *)(Lib_XmlCvtIdToStr(ID_TIP_X_TO_EXIT_OTHERS_CONTINUE))
#define TIP_ANY_TO_END_TEST                     (char *)(Lib_XmlCvtIdToStr(ID_TIP_ANY_TO_END_TEST))
#define TIP_ANY_TO_CONTINUE                     (char *)(Lib_XmlCvtIdToStr(ID_TIP_ANY_TO_CONTINUE))
#define TIP_ANY_TO_EXIT                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_ANY_TO_EXIT))
#define TIP_CANCEL_TO_STOP                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_CANCEL_TO_STOP))
#define TIP_ENTER_TO_EDIT                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_ENTER_TO_EDIT))
#define TIP_O_CONFIRM_X_CANCEL                  (char *)(Lib_XmlCvtIdToStr(ID_TIP_O_CONFIRM_X_CANCEL))
#define TIP_ARR_NUM_TO_SET                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_ARR_NUM_TO_SET))
#define TIP_NOT_SUPPORT                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_NOT_SUPPORT))
#define TIP_PLEASE_INPUT                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_PLEASE_INPUT))
#define TIP_PARA_CANNOT_EMPTY                   (char *)(Lib_XmlCvtIdToStr(ID_TIP_PARA_CANNOT_EMPTY))
#define TIP_PARA_CANNOT_MORE                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_PARA_CANNOT_MORE))
#define TIP_PARA_LEN_LESS                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_PARA_LEN_LESS))
#define TIP_PARA_CANNOT_ZERO                    (char *)(Lib_XmlCvtIdToStr(ID_TIP_PARA_CANNOT_ZERO))
#define TIP_PARA_LEN_ERR                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_PARA_LEN_ERR))
#define TIP_PARA_INPUT_ERR                      (char *)(Lib_XmlCvtIdToStr(ID_TIP_PARA_INPUT_ERR))
#define TIP_PASS                                (char *)(Lib_XmlCvtIdToStr(ID_TIP_PASS))
#define TIP_UNKNOWN                             (char *)(Lib_XmlCvtIdToStr(ID_TIP_UNKNOWN))
#define TIP_USER_CANCEL                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_USER_CANCEL))
#define TIP_UNLOCKING                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_UNLOCKING))
#define TIP_SET_AUTOMODE                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_SET_AUTOMODE))
#define TIP_XOR_CHECK_ERR                       (char *)(Lib_XmlCvtIdToStr(ID_TIP_XOR_CHECK_ERR))
#define TIP_UPGRADING                           (char *)(Lib_XmlCvtIdToStr(ID_TIP_UPGRADING))
#define TIP_UPGRADE_FAIL                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_UPGRADE_FAIL))
#define TIP_PLEASE_REBOOT_LATER                 (char *)(Lib_XmlCvtIdToStr(ID_TIP_PLEASE_REBOOT_LATER))
#define TIP_CHARGING                            (char *)(Lib_XmlCvtIdToStr(ID_TIP_CHARGING))
#define TIP_ARROW_TO_SET                        (char *)(Lib_XmlCvtIdToStr(ID_TIP_ARROW_TO_SET))
#define TIP_SYNC_PERIOD                         (char *)(Lib_XmlCvtIdToStr(ID_TIP_SYNC_PERIOD))

//LVGL
#define MENU_POS_MANAGE                         "Pos Manage"
#define MENU_SETTLEMENT                         "Settlement"
#define MENU_SYSTEM_MANAGE                      "System Manage"
#define MENU_SERVERS                            "Servers"
#define MENU_OTA                                "OTA"
#define MENU_KEY_INJECT                         "Key Inject"
#define MENU_SYSTEM_SETTING                     "System Setting"
#define MENU_ABOUT                              "About"
#define MENU_SERVER                             "Server"
#define MENU_SERVER_MODE                        "Server Mode"
#define MENU_ECHO_TEST                          "Echo Test"
#define MENU_BATTERY_TEST                       "Charge-discharge Test"
#define MENU_START_TEST                         "Start Testing"
#define MENU_CHARGING_TEST                      "Charging Test"
#define MENU_DISCHARGE_TEST                     "Discharge Test"
#define MENU_DISCHARGE_CARD_TEST                "Discharge Test_Card"
#define MENU_DISCHARGE_SCAN_TEST                "Discharge Test_Scan"
#define MENU_ENDURANCE_TEST                     "Endurance Test"
#define MENU_FILESYSTEM_TEST                    "FileSystem Test"
#define MENU_AGING_TEST                         "Aging Test"
#define MENU_PRINTER_TEST                       "Printer Test"
#define MENU_PRINTER_GRADE                      "Printer Grade"
#define MENU_PRINTER_SINGLE_TEST                "Printer Single Test"
#define MENU_PRINT_QUARTER_BLACK_TEST           "Print 1/4 black"
#define MENU_PRINT_LOGO_TEST                    "Print logo"
#define MENU_PRINT_QR_TEST                      "Print QR"
#define MENU_PRINT_CHARACTER_TEST               "Print character"
#define MENU_PRINT_ALL_BLACK_TEST               "Print all black"
#define MENU_PRINT_SPEC96_DOTBLACE_TEST         "96 Dot Special Print"
#define MENU_PRINT_CYCLE_TEST                   "Cycle Print"
#define MENU_PRINT_RECEIPT_TEST                 "Print Receipt"

#define MENU_GPS                                "GPS"
#define MENU_GPS_SATELLITE                      "GPS Satellite"
#define MENU_GPS_SIGNAL                         "GPS Signal"

#define TIP_MESSAGEBOX                          "TIP"
#define TIP_TOUCH_TO_SLIDE_ENTER_TO_CONTINUE    "Touch screen to slide\nPress O to continue"
#define TIP_PROCESSING                          "Processing"
#define TIP_SUCCESS                             "SUCCESS"
#define TIP_FAILURE                             "FAILURE"
#define TIP_USD                                 "$"
#define TIP_DEFAULT_TRASACTION_AMOUNT           "0.00"

#endif

