# Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

# Set projectname (must be done AFTER setting configurationtypes)
project(TrinityCore)

# CMake policies (can not be handled elsewhere)
cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0005 OLD)

# Set RPATH-handing (CMake parameters)
set(CMAKE_SKIP_BUILD_RPATH 0)
set(CMAKE_BUILD_WITH_INSTALL_RPATH 0)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH 1)

# set macro-directory
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/macros")

# build in Release With Debug Information mode by default if not explicitly set
if( NOT CMAKE_BUILD_TYPE )
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-std=gnu++0x HAS_STD_GNUXX0X)
if(HAS_STD_GNUXX0X)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
endif()

include(CheckCXXSourceRuns)
include(CheckIncludeFiles)

# set default buildoptions and print them
include(cmake/options.cmake)

# turn off PCH totally if enabled (hidden setting, mainly for devs)
if( NOPCH )
  set(USE_COREPCH 0)
  set(USE_SCRIPTPCH 0)
endif()

# Set build-directive (used in core to tell which buildtype we used)
if(CMAKE_GENERATOR MATCHES "Visual Studio")
  add_definitions(-D_BUILD_DIRECTIVE=\\"$(ConfigurationName)\\")
elseif(CMAKE_GENERATOR MATCHES Xcode)
  add_definitions(-D_BUILD_DIRECTIVE='"$(CONFIGURATION)"')
elseif(CMAKE_GENERATOR MATCHES Makefile)
  add_definitions(-D_BUILD_DIRECTIVE='"${CMAKE_BUILD_TYPE}"')
else()
  message(SEND_ERROR "Unable to set build-directive: unknown generator")
endif()

include(CheckPlatform)

# basic packagesearching and setup (further support will be needed, this is a preliminary release!)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_ADDITIONAL_VERSIONS
  "1.50.0"
  "1.50"
  "1.49.0"
  "1.49"
  "1.48.0"
  "1.48"
  "1.47.0"
  "1.47"
)
find_package(Boost 1.47.0 REQUIRED
)
find_package(PCHSupport)
find_package(ACE REQUIRED)
find_package(OpenSSL REQUIRED)
if( NOT USE_MYSQL_SOURCES )
  find_package(MySQL REQUIRED)
endif()

if( UNIX )
  find_package(Readline)
  find_package(ZLIB)
  find_package(BZip2)
endif()

# Find revision ID and hash of the sourcetree
include(cmake/genrev.cmake)

# print out the results before continuing
include(cmake/showoptions.cmake)

# add dependencies
add_subdirectory(dep)

# add core sources
add_subdirectory(src)
