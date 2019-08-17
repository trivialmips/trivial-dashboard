cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)

project(Trivial-Dashboard)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(Threads REQUIRED)
find_package(LibSSH REQUIRED)
find_package(QTermWidget REQUIRED)

file(GLOB_RECURSE SRC_LIST src/*.cpp)
file(GLOB_RECURSE HDR_LIST src/*.h)

include_directories(src)
include_directories(${LIBSSH_INCLUDE})
include_directories(${QTermWidget_INCLUDE})

add_custom_command(
  OUTPUT font.o
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_LINKER} -r -b binary -o ${CMAKE_BINARY_DIR}/font.o res/fonts/source-code-pro.tff
  DEPENDS res/fonts/source-code-pro.tff)

add_executable(tdb ${SRC_LIST} ${HDR_LIST} font.o)

target_link_libraries(tdb Qt5::Core Qt5::Widgets Threads::Threads)
target_link_libraries(tdb ${LIBSSH_LIBRARIES} ${QTermWidget_LIBRARIES})

install(TARGETS tdb DESTINATION bin)
