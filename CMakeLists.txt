# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)
project ("LetsTrackIt")

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

include_directories(${PROJECT_SOURCE_DIR}/tracker/include)
include_directories(${PROJECT_SOURCE_DIR}/tracker/include/video)
include_directories(${PROJECT_SOURCE_DIR}/tracker/include/detector)
include_directories(${PROJECT_SOURCE_DIR}/tracker/include/tracker)


# opencv
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


set(THIRD_PARTY_LIBS
    ${OpenCV_LIBS}
    ${PROJECT_SOURCE_DIR}/lib/libdarknet.so
)

# 包含子项目。
add_subdirectory ("tracker")
add_subdirectory("app")