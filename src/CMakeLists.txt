
include_directories(.)

#=============================================================================
# Setup helpful paths to avoid relative includes in source files
#=============================================================================
#set(ESS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
#set(ESS_MODULE_DIR ${ESS_SOURCE_DIR}/modules/)
#set(ESS_COMMON_DIR ${ESS_SOURCE_DIR}/common/)

#=============================================================================
# Required external libraries
#=============================================================================

find_package(ROOT REQUIRED)
find_package(GTest REQUIRED)
# Explicitely list required HDF5 components
find_package(HDF5 COMPONENTS CXX HL)
find_package(nlohmann_json REQUIRED)

# include_directories(${HDF5_INCLUDE_DIRS})
# include_directories(${GTEST_INCLUDE_DIRS})
# include_directories(${ROOT_INCLUDE_DIRS})

#=============================================================================
# Include subdirs
#=============================================================================
add_subdirectory(loki)