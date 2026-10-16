@echo off

if exist ".\examples\Demo\Lib\libSelfCheck_A30R.a" (

    goto next1

) else (

@echo on

echo libSelfCheck_A30R.a dose not exist.
exit /B 0

)

:next1

if exist ".\examples\Demo\Lib\libSDK_A30R.a" (

    goto next2

) else (

@echo on

echo libSDK_A30R.a dose not exist.
exit /B 0

)

:next2

@echo on

set MY_PRJ=-DHW_A30R -D__NOTRACE -DXML_MENU

call .\script\svn_ver.bat RELEASE

call tools\core_launch.bat A30R release

cd %PROJECT_OUT% & cmake ..\.. -G Ninja & ninja & cd ../..

call .\script\rename.bat A30R_APP_R mainapp_flash A30R release
