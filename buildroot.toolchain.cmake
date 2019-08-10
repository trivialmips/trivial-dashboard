SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 1)

if(NOT "$ENV{TDB_BUILDROOT_OUTPUT}" STREQUAL "")
  SET(TDB_BR_OUTPUT "$ENV{TDB_BUILDROOT_OUTPUT}")
else()
  SET(TDB_BR_OUTPUT "/opt/buildroot/output")
endif()

SET(CMAKE_C_COMPILER "${TDB_BR_OUTPUT}/host/bin/mipsel-linux-gcc")
SET(CMAKE_CXX_COMPILER "${TDB_BR_OUTPUT}/host/bin/mipsel-linux-g++")

# SET(CMAKE_SYSROOT "${TDB_BR_OUTPUT}/target")

# Find in staging directory, because the cmake package configurations are not included
# in the target direcotry
SET(CMAKE_FIND_ROOT_PATH "${TDB_BR_OUTPUT}/staging")

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
