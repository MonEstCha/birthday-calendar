cmake_minimum_required(VERSION 3.14)
project(Mongodb)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
option(CPM_USE_LOCAL_PACKAGES "Try `find_package` before downloading dependencies" ON)

file(GLOB SOURCES
    src/*.hpp
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

# using FindPkgConfig
include(FindPkgConfig)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(MONGODB REQUIRED IMPORTED_TARGET GLOBAL libmongocxx)
    if(MONGODB_FOUND)
        target_link_libraries(Mongodb PUBLIC PkgConfig::MONGODB)
    endif()
else()
    # using CPM
    CPMAddPackage(
        NAME MongoDB
        GIT_REPOSITORY https://github.com/mongodb/mongo-cxx-driver
        GIT_TAG r4.0.0
    )
    target_link_libraries(Mongodb PUBLIC MongoDB)
endif()

CPMAddPackage(
    #requires libgtk-3-dev to be installed
    NAME wxWidgets
    GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets
    GIT_TAG v3.2.6
    EXPECTED_HASH SHA1=a9451a7344308c6f81499d2fe2ab37d40fb5feb2
)
target_link_libraries(Mongodb PUBLIC wx::net wx::core wx::base)


# Build into prj build subdir from project build dir (MongoDB_prj/build)
# what to build && build : cmake .. && cmake --build .
