# 最低版本要求
cmake_minimum_required(VERSION 3.0)

# 包含目录CMAKE_CURRENT_SOURCE_DIR，指的是当前处理的 CMakeLists.txt 所在的路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 添加子目录
add_subdirectory(registration)
#add_subdirectory(processing)
#add_subdirectory(segmentation)
#add_subdirectory(visualization)
#add_subdirectory(login)
#add_subdirectory(commu)
