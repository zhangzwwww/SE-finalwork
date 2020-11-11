# 最低版本要求
cmake_minimum_required(VERSION 3.0)

# 设置工程名称
project(MedPlatform)

# 设置工程包含当前目录
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# VTK
find_package(VTK REQUIRED)
if(VTK_FOUND)
  include(${VTK_USE_FILE})
else(VTK_FOUND)
  message(FATAL_ERROR "VTK not found. Please set VTK_DIR.")
endif(VTK_FOUND)

# ITK
find_package(ITK)
if(ITK_FOUND)
  include(${ITK_USE_FILE})
else(ITK_FOUND)
  message(FATAL_ERROR "ITK not found. Please set ITK_DIR.")
endif(ITK_FOUND)

# QT
find_package(Qt5Widgets)
find_package(Qt5WebEngineWidgets)

# 包含目录，引入头文件搜索路径
include_directories(
  ${VTK_DIR}
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/app
  ${PROJECT_SOURCE_DIR}/app/registration
  ${PROJECT_SOURCE_DIR}/app/commu
  ${PROJECT_SOURCE_DIR}/app/login
  ${PROJECT_SOURCE_DIR}/app/general_processing
  ${CMAKE_CURRENT_BINARY_DIR})

# SRC
# GLOB将所有匹配<globbing-expressions>的文件挑选出来
file(GLOB
     PROJECT_SRCS
     ${PROJECT_SOURCE_DIR}/app
     ${PROJECT_SOURCE_DIR}/app/*.cpp
     ${PROJECT_SOURCE_DIR}/app/*.h
     ${PROJECT_SOURCE_DIR}/app/login/*.cpp
     ${PROJECT_SOURCE_DIR}/app/login/*.h
     ${PROJECT_SOURCE_DIR}/app/general_processing/*.cpp
     ${PROJECT_SOURCE_DIR}/app/general_processing/*.h
)

# QRC
# set(PROJECT_QRCS src/.qrc)

# UI
set(PROJECT_UIS app/mainwindow.ui  app/login/loginform.ui)

# 链接库文件
#link_directories(${PROJECT_SOURCE_DIR}/lib)

foreach(source IN LISTS PROJECT_SRCS)
    source_group("\\" FILES "${source}")
endforeach()

# 添加可执行文件
add_executable(MedPlatform ${PROJECT_SRCS} ${PROJECT_UIS} ${PROJECT_QRCS})

# 链接库
target_link_libraries(MedPlatform
                      ${VTK_LIBRARIES}
                      ${ITK_LIBRARIES}
                      Registration
                      Qt5::Widgets)

# 屏蔽Release运行时的控制台窗口
set_target_properties(${PROJECT_NAME}
                      PROPERTIES LINK_FLAGS_RELEASE
                                 "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
						 
# 添加子目录
add_subdirectory(app)
