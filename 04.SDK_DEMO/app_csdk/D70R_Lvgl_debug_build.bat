@echo off

if exist ".\examples\Demo\Lib\libSelfCheck_D70R.a" (

    goto next1

) else (

@echo on

echo libSelfCheck_D70R.a dose not exist.
exit /B 0

)

:next1

if exist ".\examples\Demo\Lib\libSDK_D70R.a" (

    goto next2

) else (

@echo on

echo libSDK_D70R.a dose not exist.
exit /B 0

)

:next2

@echo on

set MY_PRJ=-DHW_D70R -DXML_MENU

call .\script\svn_ver.bat DEBUG

call tools\core_launch.bat D70R debug

cd %PROJECT_OUT% & cmake ..\.. -G Ninja & ninja & cd ../..

call .\script\rename.bat D70R_APP_R mainapp_flash D70R debug
