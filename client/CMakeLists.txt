cmake_minimum_required(VERSION 3.16)
project(DilithiumClient VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

if(WIN32)
    # libzip comes from vcpkg (small, no OpenSSL) and exposes a CMake config package.
    find_package(libzip CONFIG REQUIRED)

    # liboqs is built from source instead of vcpkg: the vcpkg port depends on
    # OpenSSL, which is slow to build under MSVC and fills the CI runner disk.
    # The client only needs ML-DSA (Dilithium) signature verification, so we
    # build liboqs statically with its internal crypto (OQS_USE_OPENSSL=OFF).
    include(FetchContent)
    set(OQS_USE_OPENSSL OFF CACHE BOOL "" FORCE)
    set(OQS_BUILD_ONLY_LIB ON CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    # Pin liboqs 0.15.0: it implements FINAL FIPS 204 ML-DSA, matching the
    # Linux system liboqs and the server's dilithium-py. liboqs 0.10.x only
    # shipped the pre-final "ipd" ML-DSA (and used different algorithm names),
    # which is NOT interoperable with FIPS 204 signatures, so verification of
    # server-produced packages always failed on the Windows build.
    FetchContent_Declare(
        liboqs
        GIT_REPOSITORY https://github.com/open-quantum-safe/liboqs.git
        GIT_TAG 0.15.0
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(liboqs)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBOQS REQUIRED liboqs)
    pkg_check_modules(OPENSSL REQUIRED openssl)
    pkg_check_modules(LIBZIP REQUIRED libzip)
endif()

# Resolve the actual imported target names (they differ between vcpkg versions
# and Linux distributions), so the build does not depend on a single spelling.
if(TARGET OQS::oqs)
    set(OQS_TARGET OQS::oqs)
elseif(TARGET oqs)
    set(OQS_TARGET oqs)
elseif(TARGET liboqs)
    set(OQS_TARGET liboqs)
else()
    set(OQS_TARGET "")
endif()

if(TARGET libzip::zip)
    set(ZIP_TARGET libzip::zip)
elseif(TARGET zip)
    set(ZIP_TARGET zip)
else()
    set(ZIP_TARGET "")
endif()

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
    if(NOT OQS_TARGET)
        message(FATAL_ERROR "liboqs CMake target not found (expected OQS::oqs or liboqs from vcpkg)")
    endif()
    if(NOT ZIP_TARGET)
        message(FATAL_ERROR "libzip CMake target not found (expected libzip::zip or zip from vcpkg)")
    endif()
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            Qt6::Core
            Qt6::Gui
            Qt6::Widgets
            Qt6::Network
            ${OQS_TARGET}
            ${ZIP_TARGET}
    )
    # liboqs generates its umbrella headers (oqs/oqs.h, oqs/oqsconfig.h) into the
    # build tree and does not expose them on the target's interface when built as
    # a subproject, so add the generated include dir explicitly.
    target_include_directories(${PROJECT_NAME} PRIVATE "${liboqs_BINARY_DIR}/include")
    # GUI application: do not open a console window on launch.
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
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
