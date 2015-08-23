cmake_minimum_required(VERSION 2.8)

project(Game)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -pedantic")
set(CMAKE_BUILD_TYPE "Release")

file(GLOB SRC "src/*.cpp")

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
  set(CMAKE_INSTALL_RPATH "$ORIGIN/lib/:$$ORIGIN/lib/")
endif()

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/SFML/include")
include_directories("${PROJECT_SOURCE_DIR}/TGUI/include")

find_package(SFML 2.3 COMPONENTS audio graphics window system REQUIRED)
find_package(TGUI 0.7 REQUIRED) # WARNING: Only use the custom version that comes with this project

add_executable(Game ${SRC})
target_link_libraries(Game ${SFML_LIBRARIES} ${TGUI_LIBRARY})

install(TARGETS Game DESTINATION ${PROJECT_SOURCE_DIR})
