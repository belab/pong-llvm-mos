set PATH=%PATH%;%LLVM_MOS_DIR%\llvm-mos\bin;%SDL2VICE_DIR%\SDL2VICE-3.6.0-win64
mkdir build
call mos-c64-clang -Os -o build/main.prg main.cpp || exit /B 1
@echo running build/main.prg...
call x64sc.exe build/main.prg || exit /B 1