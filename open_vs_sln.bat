mkdir msvc
cd msvc
set VCPKG_DIR=%VCPKG_ROOT_DIR%/installed/x64-windows
set PATH=%SPAM_ROOT_DIR%/install/wxWidgets/lib/vc_x64_dll;%PATH%
cmake -G"Visual Studio 15 2017 Win64" ".." -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT_DIR%\scripts\buildsystems\vcpkg.cmake
cmake --open .