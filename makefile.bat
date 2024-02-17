@echo off
mkdir build
cd build

:: Set MinGW Makefiles and specify the toolchain file for vcpkg
cmake -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE=C:/Users/Jack/Documents/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-mingw-static -DCMAKE_MAKE_PROGRAM=C:\msys64\mingw64\bin\mingw32-make.exe ..

:: Build the project
cmake --build . --config Release || (
    echo Build failed.
    pause
    exit /b
)

:: Copy the built executable to the parent directory
copy /Y main.exe ..

cd ..

pause