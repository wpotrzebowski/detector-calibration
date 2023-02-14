include_directories(.)
include_directories(..)


set(SOURCES main.cpp H5Reader.h CalibrationCalculator.h CalibrationCalculator.cpp)

add_executable(calculate_calibration.exe ${SOURCES})

# Link against the ROOT libraries
target_link_libraries(calculate_calibration.exe ${ROOT_LIBRARIES} ${HDF5_LIBRARIES})


add_executable(test.exe test/CalibrationCalculatorTest.cpp CalibrationCalculator.h CalibrationCalculator.cpp)
target_link_libraries(test.exe ${ROOT_LIBRARIES} ${HDF5_LIBRARIES} ${CONAN_LIBS_GTEST} ${GTEST_LIBRARIES})
# gtest_discover_tests(test.exe)