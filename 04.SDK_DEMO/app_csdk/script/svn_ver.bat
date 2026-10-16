@echo off
@echo %1

if "%1" == "DEBUG"  subwcrev . .\script\svn_version_d.tmpl .\examples\Demo\Application\Include\version.h
if "%1" == "RELEASE"  subwcrev . .\script\svn_version_r.tmpl .\examples\Demo\Application\Include\version.h


subwcrev . .\script\rename.tmpl .\script\rename.bat
