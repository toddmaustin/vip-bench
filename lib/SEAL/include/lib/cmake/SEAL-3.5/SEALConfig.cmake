# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT license.

# Exports target SEAL::seal
#
# Creates variables:
#   SEAL_FOUND : If a static Microsoft SEAL library was found
#   SEAL_SHARED_FOUND : If a shared Microsoft SEAL library was found
#   SEAL_C_FOUND : If a Microsoft SEAL C export library was found
#   SEAL_VERSION : The full version number
#   SEAL_VERSION_MAJOR : The major version number
#   SEAL_VERSION_MINOR : The minor version number
#   SEAL_VERSION_PATCH : The patch version number
#   SEAL_DEBUG : Set to non-zero value if library is compiled with extra debugging code (very slow!)
#   SEAL_USE_CXX17 : Set to non-zero value if library is compiled as C++17 instead of C++14
#   SEAL_ENFORCE_HE_STD_SECURITY : Set to non-zero value if library is compiled to enforce at least
#       a 128-bit security level based on HomomorphicEncryption.org security estimates
#   SEAL_USE_MSGSL : Set to non-zero value if library is compiled with Microsoft GSL support
#   SEAL_USE_ZLIB : Set to non-zero value if library is compiled with zlib support


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was SEALConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

include(CMakeFindDependencyMacro)

macro(seal_find_dependency dep)
    find_dependency(${dep})
    if(NOT ${dep}_FOUND)
        if(NOT SEAL_FIND_QUIETLY)
            message(WARNING "Could not find dependency `${dep}` required by this configuration")
        endif()
        set(SEAL_FOUND FALSE)
        return()
    endif()
endmacro()

set(SEAL_FOUND FALSE)
set(SEAL_SHARED_FOUND FALSE)
set(SEAL_C_FOUND FALSE)

set(SEAL_VERSION 3.5.4)
set(SEAL_VERSION_MAJOR 3)
set(SEAL_VERSION_MINOR 5)
set(SEAL_VERSION_PATCH 4)

set(SEAL_BUILD_TYPE Release)
set(SEAL_DEBUG OFF)
set(SEAL_USE_CXX17 ON)
set(SEAL_ENFORCE_HE_STD_SECURITY )

set(SEAL_USE_MSGSL ON)
set(SEAL_USE_ZLIB ON)

# Add the current directory to the module search path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
seal_find_dependency(Threads)

include(${CMAKE_CURRENT_LIST_DIR}/SEALTargets.cmake)

if(TARGET SEAL::seal)
    set(SEAL_FOUND TRUE)
endif()

if(TARGET SEAL::seal_shared)
    set(SEAL_SHARED_FOUND TRUE)
endif()

if(TARGET SEAL::sealc)
    set(SEAL_C_FOUND TRUE)
endif()

if(SEAL_FOUND)
    if(NOT SEAL_FIND_QUIETLY)
        message(STATUS "Microsoft SEAL -> Version ${SEAL_VERSION} detected")
    endif()
    if(SEAL_DEBUG AND NOT SEAL_FIND_QUIETLY)
        message(STATUS "Performance warning: Microsoft SEAL compiled in debug mode")
    endif()

    set(SEAL_TARGETS_AVAILABLE "Microsoft SEAL -> Targets available: SEAL::seal")
    if(SEAL_SHARED_FOUND)
        string(APPEND SEAL_TARGETS_AVAILABLE ", SEAL::seal_shared")
    endif()
    if(SEAL_C_FOUND)
        string(APPEND SEAL_TARGETS_AVAILABLE ", SEAL::sealc")
    endif()
    if(NOT SEAL_FIND_QUIETLY)
        message(STATUS ${SEAL_TARGETS_AVAILABLE})
    endif()
else()
    if(NOT SEAL_FIND_QUIETLY)
        message(STATUS "Microsoft SEAL -> NOT FOUND")
    endif()
endif()
