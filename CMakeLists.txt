cmake_minimum_required(VERSION 3.14)
project(Mongodb)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
option(CPM_USE_LOCAL_PACKAGES "Try `find_package` before downloading dependencies" ON)

file(GLOB SOURCES
    src/*.hpp
    src/*.h
    src/*.cpp
)

add_executable(Mongodb ${SOURCES})

include(CMakePrintHelpers)

# download CPM.cmake
file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.5/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  EXPECTED_HASH SHA256=c46b876ae3b9f994b4f05a4c15553e0485636862064f1fcc9d8b4f832086bc5d
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)
#cmake_print_variables(PROJECT_SOURCE_DIR)

include(FindPkgConfig)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(MONGODB REQUIRED IMPORTED_TARGET GLOBAL libmongocxx)
    if(MONGODB_FOUND)
        target_link_libraries(Mongodb PUBLIC PkgConfig::MONGODB)
    endif()
endif()

CPMAddPackage(
    #requires libgtk-3-dev to be installed
    NAME wxWidgets
    GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets
    GIT_TAG v3.2.8
    EXPECTED_HASH SHA1=b218ba7e0d472e0763cf67742b1ded946fd21954
)
target_link_libraries(Mongodb PUBLIC wx::net wx::core wx::base)


# Build into prj build subdir from project build dir (MongoDB_prj/build)
# what to build && build : cmake .. && cmake --build .
