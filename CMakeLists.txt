cmake_minimum_required(VERSION 3.5)

project(orgb LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
set(DEPENDS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/depends)

set(OPENCV_DIR "${DEPENDS_DIR}/opencv")
set(OPENCV_INCLUDE_DIR "${OPENCV_DIR}/include")
set(OPENCV_LIBRARY_DIR "${OPENCV_DIR}/lib")
set(OPENCV_BINARY_DIR "${OPENCV_DIR}/bin")
set(OPENCV_VERSION 451)

list(APPEND OPENCV_LIBS
     opencv_core
     opencv_imgcodecs
     opencv_highgui
     )

foreach(lib ${OPENCV_LIBS})
    set(OPENCV_LIBRARIES ${OPENCV_LIBRARIES} ${lib}${OPENCV_VERSION})
endforeach()

include_directories(${OPENCV_INCLUDE_DIR})
include_directories(./include)
link_directories(${OPENCV_LIBRARY_DIR})

set(SOURCE_FILES
    src/main.cpp
    mainwindow.cpp
    src/orgbconverter.cpp
    src/lcc.cpp
    )

set(HEADER_FILES
    mainwindow.h
    include/orgbconverter.h
    include/angletransformer.h
    include/lcc.h
    )

set(RESOURCE_FILES
    mainwindow.ui
    )

add_executable(orgb WIN32
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${RESOURCE_FILES}
    )

target_link_libraries(orgb PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(orgb PRIVATE ${OPENCV_LIBRARIES})

foreach(lib ${OPENCV_LIBRARIES})
    add_custom_command(TARGET orgb
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${OPENCV_BINARY_DIR}/${lib}.dll"
        "${CMAKE_BINARY_DIR}/${lib}.dll"
        )
endforeach()
