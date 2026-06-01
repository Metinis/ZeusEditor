@echo off
setlocal enabledelayedexpansion

:: Check that VULKAN_SDK is set
if "%VULKAN_SDK%"=="" (
    echo [ERROR] VULKAN_SDK environment variable is not set.
    echo Make sure you have the Vulkan SDK installed and the environment initialized.
    pause
    exit /b 1
)

:: Path to glslangValidator
set GLSLANG_VALIDATOR="%VULKAN_SDK%\Bin\glslangValidator.exe"

:: Input shader files (edit these names or use for loop for many)
set VERT_SHADER=vkbasic.vert
set FRAG_SHADER=vkbasic.frag

:: Output directory
set OUT_DIR=compiled_shaders
if not exist %OUT_DIR% mkdir %OUT_DIR%

:: Compile vertex shader
echo Compiling %VERT_SHADER%...
%GLSLANG_VALIDATOR% -V %VERT_SHADER% -o %OUT_DIR%\%VERT_SHADER%.spv

:: Compile fragment shader
echo Compiling %FRAG_SHADER%...
%GLSLANG_VALIDATOR% -V %FRAG_SHADER% -o %OUT_DIR%\%FRAG_SHADER%.spv

echo [SUCCESS] Shaders compiled to %OUT_DIR%.
pause
