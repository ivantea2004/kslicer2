# Kernel slicer 2

This project is a rewrite of the original [kernel slicer](https://github.com/Ray-Tracing-Systems/kernel_slicer).

## How to build

### Install dependencies (Windows)
Download [llvm release archive from github](https://github.com/llvm/llvm-project/releases/tag/llvmorg-22.1.8). Extract the archive and provide path to the llvm directory when configuring project (see below).

### Install dependencies (Ubuntu/Debian)
According to the [official docs](https://apt.llvm.org/)

    wget https://apt.llvm.org/llvm.sh
    chmod +x llvm.sh
    sudo ./llvm.sh 22 all

### Build (Windows)
Provide path to the extracted archive when configuring on windows

    cmake -S . -B build -DLLVM_ROOT=path/to/clang+llvm -DClang_ROOT=path/to/clang+llvm
    cmake --build build

### Build (Linux)

    cmake -S . -B build
    cmake --build build
