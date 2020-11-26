# 最低版本要求
cmake_minimum_required(VERSION 3.0)

# 模块名
set(MODULE_NAME Voxel2Mesh)

# SRC
file(GLOB MODULE_SRCS1 "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
           

foreach(source IN LISTS MODULE_SRCS1)
    source_group("\\Voxel2Mesh" FILES "${source}")
endforeach()


# 动态库，将指定的源文件生成链接文件，然后添加到工程中去
add_library(${MODULE_NAME} STATIC ${MODULE_SRCS1})

# 链接QT
target_link_libraries(${MODULE_NAME} Qt5::Widgets)