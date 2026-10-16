# Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
# All rights reserved.
#
# This software is supplied "AS IS" without any warranties.
# RDA assumes no responsibility or liability for the use of the software,
# conveys no license or title under any patent, copyright, or mask work
# right to the product. RDA reserves the right to make changes in the
# software without notification.  RDA also make no representation or
# warranty that such application will be suitable for the specified use
# without further testing or modification.

add_definitions("$ENV{MY_PRJ}")

#configure_file(include/mainApp_config.h.in ${BINARY_TOP_DIR}/include/mainApp_config.h)

include_directories(Application/include Driver/include
                    Middle/include
                    Lib/include
                    Emv
                    Lvgl
                    Lvgl/porting
                    Lvgl/include/misc
                    )

if($ENV{BUILD_TARGET} STREQUAL "A30R")
    message("****************A30R****************")
    file(GLOB libSDK  lib/libSDK_A30R.a)
    file(GLOB libCheck  lib/libSelfCheck_A30R.a)

elseif($ENV{BUILD_TARGET} STREQUAL "D70R")
    message("****************D70R****************")
    file(GLOB libSDK  lib/libSDK_D70R.a)
    file(GLOB libCheck  lib/libSelfCheck_D70R.a)
elseif($ENV{BUILD_TARGET} STREQUAL "A75R")
    message("****************A75R****************")
    file(GLOB libSDK  lib/libSDK_A75R.a)
    file(GLOB libCheck  lib/libSelfCheck_D70R.a)
else()
    message("********BUILD.BAT ERROR********")
    return()
endif()

file(GLOB libtyEmvKernel  lib/libEmvKernel.a)
file(GLOB libcJson  lib/libcJson.a)
file(GLOB libTyTms  lib/libTms.a)
file(GLOB libtyBat  lib/libBattery.a)
file(GLOB libLvgl   lib/libLvgl_V8310.a)
if(CONFIG_APPIMG_LOAD_FLASH)
    set(target mainapp_flash)
    file(GLOB MAINSRC  Application/Source/*.c
                       Application/Source/mainapp/*.c
                       Driver/Source/*.c
                       Middle/Source/*.c
                       Emv/*.c
                       Lvgl/porting/*.c)
                        
    add_appimg(${target} ${mainapp_flash_ldscript} ${MAINSRC})
    target_link_libraries(${target} PRIVATE ${libCheck} ${libLvgl} ${libSDK} ${libtyEmvKernel} ${libcJson} ${libTyTms} ${libtyBat} ${libm_file_name} ${libc_file_name} ${libgcc_file_name} )

    set(pac_config ${out_hex_dir}/${target}.json)
    set(pac_file ${out_hex_dir}/${target}.pac)
    pac_init_fdl(init_fdl ${pac_config})
    execute_process(
        COMMAND python3 ${pacgen_py} ${init_fdl}
                cfg-image -i APPIMG -a ${CONFIG_APPIMG_FLASH_ADDRESS} -s ${CONFIG_APPIMG_FLASH_SIZE}
                -p ${out_hex_dir}/${target}.img ${pac_config}
                dep-gen --base ${SOURCE_TOP_DIR} ${pac_config}
        OUTPUT_VARIABLE pac_dep
        OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${SOURCE_TOP_DIR}
    )

    add_custom_command(OUTPUT ${pac_file}
        COMMAND python3 ${pacgen_py} pac-gen ${pac_config} ${pac_file}
        DEPENDS ${pacgen_py} ${pac_config} ${pac_dep}
        WORKING_DIRECTORY ${SOURCE_TOP_DIR}
    )
    add_custom_target(${target}_pacgen ALL DEPENDS ${pac_file})
 endif()
