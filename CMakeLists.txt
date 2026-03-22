cmake_minimum_required(VERSION 3.16)
project(DilithiumClient VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(WIN32)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
    set(BUILD_SHARED_LIBS OFF)
    add_definitions(-DUNICODE -D_UNICODE)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBOQS REQUIRED liboqs)
    pkg_check_modules(OPENSSL REQUIRED openssl)
    pkg_check_modules(LIBZIP REQUIRED libzip)
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/apiclient.cpp
    src/crypto.cpp
)

set(HEADERS
    src/mainwindow.h
    src/apiclient.h
    src/crypto.h
)

set(UI_FILES
    src/mainwindow.ui
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            Qt6::Core
            Qt6::Gui
            Qt6::Widgets
            Qt6::Network
            liboqs
            libzip
            ws2_32
            secur32
    )
else()
    target_include_directories(${PROJECT_NAME} PRIVATE ${LIBOQS_INCLUDE_DIRS} ${LIBZIP_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            Qt6::Core
            Qt6::Gui
            Qt6::Widgets
            Qt6::Network
            ${LIBOQS_LIBRARIES}
            ${OPENSSL_LIBRARIES}
            ${LIBZIP_LIBRARIES}
    )
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
