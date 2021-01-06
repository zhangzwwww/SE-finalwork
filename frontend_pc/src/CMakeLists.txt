# 最低版本要求
cmake_minimum_required(VERSION 3.0)

# 设置工程名称
project(MEDLAB)

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

#set(QT Core Gui Widgets Network DBus Sql)
#find_package(Qt5 REQUIRED ${QT})
#qt5_use_modules(MedPlatform ${QT})

# QT
find_package(Qt5Widgets )
find_package(Qt5WebEngineWidgets)
find_package(Qt5Network)

# 包含目录，引入头文件搜索路径
include_directories(
  ${VTK_DIR}
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/app
  ${PROJECT_SOURCE_DIR}/app/registration
  ${PROJECT_SOURCE_DIR}/app/voxel2mesh
  ${PROJECT_SOURCE_DIR}/app/segmentation
  ${PROJECT_SOURCE_DIR}/app/commu
  ${PROJECT_SOURCE_DIR}/app/login
  ${PROJECT_SOURCE_DIR}/app/utils
  ${PROJECT_SOURCE_DIR}/app/predict
  ${PROJECT_SOURCE_DIR}/app/load
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
     ${PROJECT_SOURCE_DIR}/app/commu/*.h
     ${PROJECT_SOURCE_DIR}/app/commu/*.cpp
     ${PROJECT_SOURCE_DIR}/app/utils/*.h
     ${PROJECT_SOURCE_DIR}/app/load/*.h
     ${PROJECT_SOURCE_DIR}/app/utils/*.cpp
     ${PROJECT_SOURCE_DIR}/app/load/*.cpp
     ${PROJECT_SOURCE_DIR}/app/predict/*.cpp
     ${PROJECT_SOURCE_DIR}/app/predict/*.h
     ${PROJECT_SOURCE_DIR}/app/general_processing/*.cpp
     ${PROJECT_SOURCE_DIR}/app/general_processing/*.h
     ${PROJECT_SOURCE_DIR}/resources/myapp.rc
)

# QRC
set(PROJECT_QRCS app/mainwindow.qrc)

# UI
set(PROJECT_UIS app/mainwindow.ui  
                           app/login/loginform.ui 
                           app/login/signup_form.ui
                           app/load/upload_form.ui
                           app/load/download_form.ui
                           app/predict/predict_form.ui)

# 链接库文件
#link_directories(${PROJECT_SOURCE_DIR}/lib)

foreach(source IN LISTS PROJECT_SRCS)
    source_group("\\" FILES "${source}")
endforeach()

# 添加可执行文件
add_executable(MEDLAB ${PROJECT_SRCS} ${PROJECT_UIS} ${PROJECT_QRCS})

# 链接库
target_link_libraries(MEDLAB
                      ${VTK_LIBRARIES}
                      ${ITK_LIBRARIES}
                      registration
                      voxel2mesh
                      segmentation
                      Qt5::Widgets
                      Qt5::Network)

# 屏蔽Release运行时的控制台窗口
set_target_properties(${PROJECT_NAME}
                      PROPERTIES LINK_FLAGS_RELEASE
                                 "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
						 
# 添加子目录
add_subdirectory(app)

