@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64

set VENDOR_DIR=%~dp0src\pyvma2\vendor
set VK_INC=C:\Users\grill\VulkanSDK\Include

echo Building VMA static library...
cl /c /nologo /W3 /Ox /EHsc /MD /TP /DVMA_IMPLEMENTATION /DVMA_STATIC_VULKAN_FUNCTIONS=0 /DVMA_DYNAMIC_VULKAN_FUNCTIONS=0 /I"%VK_INC%" /I"%VENDOR_DIR%" "%VENDOR_DIR%\vma_impl.cpp" /Fo:"%VENDOR_DIR%\vma_impl.obj"
if errorlevel 1 (
    echo FAILED: cl compilation
    exit /b 1
)

lib /nologo /OUT:"%VENDOR_DIR%\vk_mem_alloc.lib" "%VENDOR_DIR%\vma_impl.obj"
if errorlevel 1 (
    echo FAILED: lib creation
    exit /b 1
)

del "%VENDOR_DIR%\vma_impl.obj" 2>nul
echo [pyvma2] Built: %VENDOR_DIR%\vk_mem_alloc.lib
