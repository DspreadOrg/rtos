/**********************************************************************************************************
 * Copyright(C)2011,
 * File Name     : main.c
 *  Version      :
 *  Author       :
 * Description   : App main function
 **********************************************************************************************************/
#include "Input.h"
#include "Power.h"
#include "Logon.h"
#include "Ui.h"
#include "WifiConfig.h"
#include "Common.h"
#include "Trade.h"
#include "Idle.h"
#include "TradeRecord.h"
#include "CardTrade.h"
#include "Menu.h"
#include "LanguageProcess.h"
#include "LanguageResource.h"
#include "Tms_Module.h"
#include "Drv.h"
#include "Language.h"
#include "Communication.h"
#include "EmvDemo.h"
#include "api_lvgl.h"
#include "custom_Lvgl.h"
#if defined(MQTT_SUPPORT)
#include "Mqtt.h"
#endif
#include "net_if.h"
#include "version.h"

//To get the version data after renaming the compile script, define this string
const char SWVerStr[] = {"\nVER:"INFO_DEV_SVER};

int main(void)
{
    // If need to output logs to the debugging tool coolwatcher, open the following statement
#ifndef __NOTRACE
    SetTraceLogConfig(1);
    lv_log_register_print_cb((lv_log_print_g_cb_t)Lib_Trace);
#else
    lv_log_register_print_cb(NULL);
#endif
    TRACE("Entry APP Demo!!!\r\n");

    // Init System before entering Application
    SystemInit();

    // DeviceParaInit ResourceInit Lib_AppInit
    // These three functions must be called, please do not modify the call order
    // Device parameter initialization
    DeviceParaInit();

    // Resource initialization
    ResourceInit();

    Lib_LvglInit();

    // System app initialization, must be called
    Lib_AppInit();


    // Power management related function initialization
    PowerInit();

    // Network module initialization
    UIWireNetInit();

    // Transaction record initialization
    ApiInitRecord();

    // Factory equipment inspection, please do not delete
    DevFactoryCheck();

    CardDemoInit();

    LvHomeMenu(NULL);

    return 0;
}

int appimg_enter(void *argv)
{
    // The parameter is OS API
    Lib_OsApiInit(argv);

    main();

    return 0;
}

void appimg_exit(void)
{
    TRACE("External MAINAPP exit");
}


