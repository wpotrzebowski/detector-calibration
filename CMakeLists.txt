cmake_minimum_required(VERSION 3.0)

project(CalibrationCalculatorProject)

# Find the ROOT installation
find_package(ROOT REQUIRED)
find_package(HDF5 REQUIRED COMPONENTS CXX)
include_directories(${ROOT_INCLUDE_DIRS} ${HDF5_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES main.cpp H5Reader.h CalibrationCalculator.h CalibrationCalculator.cpp)

add_executable(calculate_calibration.exe ${SOURCES})

# Link against the ROOT libraries
target_link_libraries(calculate_calibration.exe ${ROOT_LIBRARIES} HDF5::HDF5 )
