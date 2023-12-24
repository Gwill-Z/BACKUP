cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(BackupProject VERSION 1.0)

# 指定C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含头文件目录
include_directories(include)

# 添加要编译的源文件
add_executable(BackupProject 
    src/main.cpp
    src/BackupManager.cpp
    src/FileManager.cpp
    src/Compressor.cpp
    src/Encryptor.cpp
    src/CRCValidator.cpp
    src/Logger.cpp
)

# 如果有其他库依赖，可以在这里指定
# target_link_libraries(BackupProject PUBLIC ...)
