include_directories(.)
include_directories(..)


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