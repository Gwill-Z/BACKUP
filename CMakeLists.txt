cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(BackupProject VERSION 1.0)

# 指定C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含头文件目录
include_directories(include)
add_subdirectory(third_lib/spdlog)

# 添加Google Test的子目录
add_subdirectory(googletest)

# 包含Google Test的头文件
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 添加要编译的源文件
add_executable(BackupProject 
    src/main.cpp
    src/BackupManager.cpp
    src/FileManager.cpp
    src/Packer.cpp
    src/Compressor.cpp
    src/Encryptor.cpp
    src/CRCValidator.cpp
)

# 创建一个测试目标
add_executable(BackupProjectTest
    tests/test_huffman.cpp  # 测试主文件
    src/Compressor.cpp      # 测试文件依赖的源文件
    # 其他测试文件
)

# 链接Google Test库和项目的其他依赖
target_link_libraries(BackupProjectTest gtest gtest_main spdlog::spdlog)
