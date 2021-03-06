cmake_minimum_required(VERSION 2.8.6) # for CMakePushCheckState
project(icicle CXX C)
enable_testing()

# search for a local version of libcloudphxx
find_library(libcloudphxx_LIBRARIES cloudphxx_lgrngn ${CMAKE_SOURCE_DIR}/../../build/src NO_DEFAULT_PATH)
if (libcloudphxx_LIBRARIES)
  message("icicle will use libclouphxx++ from current repository")
  include_directories(PUBLIC ${CMAKE_SOURCE_DIR}/../../include)
else()
  message("libcloudphxx++ not found in current repository, icicle will use an installed version")
endif()

# if no local version found, look for an installed one
find_library(libcloudphxx_LIBRARIES cloudphxx_lgrngn)

if (NOT libcloudphxx_LIBRARIES)
  message(FATAL_ERROR "libclouphxx++ not found")
endif()

find_package(libmpdata++)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
  # make the Release flags the default
  set(CMAKE_CXX_FLAGS ${libmpdataxx_CXX_FLAGS_RELEASE})
  # ignore CMake defaults
  set(CMAKE_CXX_FLAGS_RELEASE "")
else()
  set(CMAKE_CXX_FLAGS_DEBUG ${libmpdataxx_CXX_FLAGS_DEBUG})
endif()

add_subdirectory(src)
add_subdirectory(tests)
