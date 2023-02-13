cmake_minimum_required(VERSION 3.0)

project(CalibrationCalculatorProject)

include_directories(test)

#=============================================================================
# Augment CMake with our custom scripts
#=============================================================================
set(EXTRA_MODULES_DIR ${CMAKE_CURRENT_LIST_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${EXTRA_MODULES_DIR}/modules)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

#=============================================================================
# Conan
#=============================================================================

SET(CONAN_PROFILE "default" CACHE STRING "Name of conan profile to use, uses default by default")
SET(CONAN "AUTO" CACHE STRING "conan options AUTO (conan must be in path), MANUAL (expects conanbuildinfo.cmake in build directory) or DISABLE")
if(${CONAN} MATCHES "AUTO")
  include(${EXTRA_MODULES_DIR}/modules/conan.cmake)
  conan_cmake_run(CONANFILE conanfile.txt
      PROFILE ${CONAN_PROFILE}
      BASIC_SETUP NO_OUTPUT_DIRS KEEP_RPATHS NO_IMPORTS
      BUILD_TYPE "None"
      BUILD outdated)
elseif(${CONAN} MATCHES "MANUAL")
  if(EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    conan_basic_setup(NO_OUTPUT_DIRS KEEP_RPATHS)
  else()
    MESSAGE(FATAL_ERROR "CONAN set to MANUAL but no file named conanbuildinfo.cmake found in build directory")
  endif()
elseif(NOT ${CONAN} MATCHES "DISABLE")
  MESSAGE(FATAL_ERROR "Unrecognised option for CONAN (${CONAN}), use AUTO, MANUAL or DISABLE")
endif()


# Find the ROOT installation
find_package(ROOT REQUIRED)
find_package(GTest REQUIRED)
# Explicitely list required HDF5 components
find_package(HDF5 COMPONENTS CXX HL)


include_directories(${HDF5_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${ROOT_INCLUDE_DIRS})
# include(GoogleTest)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES main.cpp H5Reader.h CalibrationCalculator.h CalibrationCalculator.cpp)

add_executable(calculate_calibration.exe ${SOURCES})

# Link against the ROOT libraries
target_link_libraries(calculate_calibration.exe ${ROOT_LIBRARIES} ${HDF5_LIBRARIES})


add_executable(test.exe test/CalibrationCalculatorTest.cpp CalibrationCalculator.h CalibrationCalculator.cpp)
target_link_libraries(test.exe ${ROOT_LIBRARIES} ${HDF5_LIBRARIES} ${CONAN_LIBS_GTEST} ${GTEST_LIBRARIES})
# gtest_discover_tests(test.exe)
