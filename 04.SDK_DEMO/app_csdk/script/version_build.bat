if %2==DEBUG (
    if "%5"=="" (
        set MY_PRJ=-D%1
    ) else (
        set MY_PRJ=-D%1 -D%5
    )
) else (
    if "%5"=="" (
        set MY_PRJ=-D%1 -D__NOTRACE
    ) else (
        set MY_PRJ=-D%1 -D__NOTRACE -D%5
    )
)
call script\svn_ver.bat %2
call tools\core_launch.bat %3 %4
cd out\%3_%4
cmake ..\.. -G Ninja
ninja
cd ../..
call .\script\rename.bat %3_MainAPP_D mainapp_file %3 %4