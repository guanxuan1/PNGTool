#指定cmake的最小版本
cmake_minimum_required(VERSION 3.2)
#制定项目的名称
project(PNGTool)

# 判断目前是不是编译引擎
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src")
    set(COMPILE_ENGINE true) # 标记当前有源码，是编译引擎的流程
else()
    set(COMPILE_ENGINE false)
endif()

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
# 引用QT
###############################################################################
set(QT_VERSION "Qt5.15.2")

if(MSVC AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/${QT_VERSION}")
    if(MSVC_VERSION EQUAL 1900) # vs2015
        set(QT_LIB_VER "msvc2015_64")
    elseif(MSVC_VERSION GREATER_EQUAL 1910 AND MSVC_VERSION LESS 1920)
        set(QT_LIB_VER "msvc2019_64") # Qt for vs2019 is compatible with vs2017
    elseif(MSVC_VERSION GREATER_EQUAL 1920 AND MSVC_VERSION LESS 1930)
        set(QT_LIB_VER "msvc2019_64")
    endif()

    set(QT_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/${QT_VERSION}/${QT_LIB_VER}")

    if(EXISTS ${QT_LIB_PATH})
        set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT_LIB_PATH})
    else()
        message(FATAL_ERROR "QT not found.")
    endif()

    file(
        COPY
        ${QT_LIB_PATH}/bin/Qt5Cored.dll
        ${QT_LIB_PATH}/bin/Qt5Guid.dll
        ${QT_LIB_PATH}/bin/Qt5Widgetsd.dll
        DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug
    )

    file(
        COPY
        ${QT_LIB_PATH}/bin/Qt5Core.dll
        ${QT_LIB_PATH}/bin/Qt5Gui.dll
        ${QT_LIB_PATH}/bin/Qt5Widgets.dll
        DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Release
    )

    file(
        COPY
        ${QT_LIB_PATH}/plugins/platforms/qdirect2dd.dll
        ${QT_LIB_PATH}/plugins/platforms/qminimald.dll
        ${QT_LIB_PATH}/plugins/platforms/qoffscreend.dll
        ${QT_LIB_PATH}/plugins/platforms/qwindowsd.dll
        DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/platforms
    )

    file(
        COPY
        ${QT_LIB_PATH}/plugins/platforms/qdirect2d.dll
        ${QT_LIB_PATH}/plugins/platforms/qminimal.dll
        ${QT_LIB_PATH}/plugins/platforms/qoffscreen.dll
        ${QT_LIB_PATH}/plugins/platforms/qwindows.dll
        DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Release/platforms
    )

    file(
        COPY
        ${QT_LIB_PATH}/plugins/imageformats/qtga.dll
        ${QT_LIB_PATH}/plugins/imageformats/qtgad.dll
        DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/imageformats
    )

    file(
        COPY
        ${QT_LIB_PATH}/plugins/imageformats/qtga.dll
        ${QT_LIB_PATH}/plugins/imageformats/qtgad.dll
        DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Release/imageformats
    )

    if(COMPILE_ENGINE)
        install(
            DIRECTORY ${QT_LIB_PATH}
            DESTINATION ${CMAKE_INSTALL_PREFIX}/ThirdParty/${QT_VERSION}
        )
    endif()
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

file(GLOB_RECURSE HEAD_FILES
  ./src/*.h 
   ./src/*.hpp
   ./src/*.inl
   ./src/*.h 
   ./src/*.hpp
   ./src/*.inl
)
file(GLOB_RECURSE SRC_FILES
./src/*.h 
./src/*.inl
./src/*.c*
./src/*.imp
./src/*.h 
./src/*.inl
./src/*.c*
./src/*.imp
)

file(GLOB_RECURSE FORMS_FILES
./src/*.ui)

file(GLOB_RECURSE QRC_FILES
./src/*.qrc
./src/*.rc)

  qt5_wrap_ui(QT_GEN_UI_FILE ${FORMS_FILES})

set(ALL_FILES ${HEAD_FILES} ${SRC_FILES} ${FORMS_FILES} ${QRC_FILES} ${QT_GEN_UI_FILE})
set(CMAKE_AUTOUIC_SEARCH_PATHS ./src/ui)

add_executable(PNGTool ${ALL_FILES})

source_group("source" FILES ${SRC_FILES})
source_group("head" FILES ${HEAD_FILES})
source_group("ui" FILES ${FORMS_FILES})

target_include_directories(PNGTool PUBLIC lib/libpng-1.6.38/include)
target_include_directories(PNGTool PUBLIC lib/zlib/include)

target_link_libraries(PNGTool ..\\lib\\libpng-1.6.38\\lib\\libpng16_staticd)
target_link_libraries(PNGTool ..\\lib\\zlib\\lib\\zlibstaticd)
target_link_libraries(
PNGTool
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
)
target_compile_definitions( 
    PNGTool
    PRIVATE # 仅在该库内添加的宏
      _CRT_SECURE_NO_WARNINGS
  )
