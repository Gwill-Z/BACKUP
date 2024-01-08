#!/bin/bash

# 创建并进入build目录
mkdir -p build
cd build

# 运行CMake和Make
cmake ..
make
