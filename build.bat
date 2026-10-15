@echo off
rem ------------------------------------------------------------
rem  Build and run one exercise with MSVC.
rem  Usage (run in repo root):  build.bat code\lesson_byte_split.c
rem
rem  MSVC on this PC:
rem    D:\visual_studio\VC\Auxiliary\Build\vcvars64.bat
rem  /utf-8 is required: the .c sources are saved as UTF-8.
rem  NOTE: keep this file ASCII-only. cmd.exe parses .bat with the
rem  OEM codepage (936 here), so UTF-8 Chinese inside breaks it.
rem ------------------------------------------------------------
chcp 65001 >nul
setlocal

if "%~1"=="" (
    echo Usage: build.bat ^<source.c^>
    echo Example: build.bat code\lesson_byte_split.c
    exit /b 1
)

set "VCVARS=D:\visual_studio\VC\Auxiliary\Build\vcvars64.bat"
if not exist "%VCVARS%" (
    echo [ERROR] MSVC environment not found: %VCVARS%
    echo         Edit VCVARS in this script if your path differs.
    exit /b 1
)

call "%VCVARS%" >nul
if errorlevel 1 (
    echo [ERROR] failed to init MSVC
    exit /b 1
)

pushd "%~dp1"
echo ==== build %~nx1 ====
cl /nologo /W3 /utf-8 /Fe:"%~n1.exe" "%~nx1"
if errorlevel 1 (
    echo [FAILED] see compiler output above
    popd
    exit /b 1
)

echo.
echo ==== run %~n1.exe ====
"%~n1.exe"
set "RC=%ERRORLEVEL%"
popd
endlocal & exit /b %RC%
