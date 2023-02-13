cmake_minimum_required(VERSION 3.0)

project(CalibrationCalculatorProject)

include_directories(test)


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

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(test.exe test/CalibrationCalculatorTest.cpp CalibrationCalculator.h CalibrationCalculator.cpp)
target_link_libraries(test.exe GTest::gtest_main  ${ROOT_LIBRARIES} HDF5::HDF5)
include(GoogleTest)
gtest_discover_tests(test.exe)
