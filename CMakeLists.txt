 
project(BackupProject)
cmake_minimum_required(VERSION 3.12)
 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
 
set(CMAKE_PREFIX_PATH $ENV{QTDIR512MINGW})
find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

#调试模式
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCMAKE_BUILD_TYPE=Debug")

include_directories(UI)
include_directories(include)

add_subdirectory(third_libs/spdlog)

# 添加Google Test的子目录
add_subdirectory(third_libs/googletest)

# 包含Google Test的头文件
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
 
set(HEADER_LIST
    UI/widget.h  
    include/BackupManager.h
    include/FileManager.h
    include/Packer.h
    include/Compressor.h
    include/Encryptor.h
    include/CRCValidator.h
    include/BackupConfig.h
)
 
set(SOURCE_LIST
    UI/main.cpp 
    UI/widget.cpp
    src/BackupManager.cpp
    src/FileManager.cpp
    src/Packer.cpp
    src/Compressor.cpp
    src/Encryptor.cpp
    src/CRCValidator.cpp
    src/BackupConfig.cpp
)
 
 
add_executable(${PROJECT_NAME} ${HEADER_LIST} ${SOURCE_LIST})

# 创建一个测试目标
add_executable(BackupProjectTest
    tests/test_huffman.cpp  # 测试主文件
    tests/test_encrypt.cpp
    src/Compressor.cpp      # 测试文件依赖的源文件
    src/Encryptor.cpp
    # 其他测试文件
)

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets)

# 链接Google Test库和项目的其他依赖
target_link_libraries(BackupProjectTest gtest gtest_main spdlog::spdlog)
 
set(CMAKE_VERBOSE_MAKEFILE ON)
# make VERBOSE=1
