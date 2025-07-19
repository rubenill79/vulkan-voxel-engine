@echo off
setlocal enabledelayedexpansion

set GLSLC="%VULKAN_SDK%\Bin\glslc.exe"
set SHADER_DIR=..\Game\resources\shaders

echo -- [shaders] Compilando shaders en %SHADER_DIR%
for %%f in (%SHADER_DIR%\*.vert %SHADER_DIR%\*.frag) do (
    set "SOURCE=%%~f"
    set "OUTPUT=%%~f.spv"
    echo -- [shaders] Fuente: !SOURCE!
    echo -- [shaders] Salida: !OUTPUT!
    %GLSLC% "!SOURCE!" -o "!OUTPUT!"
)
echo -- [shaders] Shaders compilados correctamente.