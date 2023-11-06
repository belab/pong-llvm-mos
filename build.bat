set PATH=%PATH%;%LLVM_MOS_DIR%\bin;%SDL2VICE_DIR%;%C64_DEBUG%
mkdir build
call mos-c64-clang -Os -o build/main.prg main.cpp || exit /B 1
@echo running build/main.prg...
call x64sc.exe build/main.prg || exit /B 1