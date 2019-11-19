project( op2D )
cmake_minimum_required(VERSION 3.15)
set (CMAKE_CXX_STANDARD 11)

add_compile_definitions(DISABLE_RCPP) # disable rcpp specific import

# specify here which python interpreter you want to compile this module for
# set(PYTHON_EXECUTABLE "~/miniconda3/bin/python3")

option(BUILD_TESTS "Build tests." ON)
option(BUILD_PYTHON_MODULE "Build python module" OFF)

message("BUILD_TESTS = ${BUILD_TESTS}")

add_library(oplib INTERFACE)
target_include_directories(oplib INTERFACE include)

if (BUILD_TESTS)
    add_subdirectory(test)
endif()

# The following requires pybind installed in the compiling system
if (BUILD_PYTHON_MODULE)
    set(PYBIND11_CPP_STANDARD -std=c++11)
    message(">>> Python module will be built")
    add_subdirectory(pybind11)
    pybind11_add_module(optimalpartitioning src/pybind_def.cpp)
    target_link_libraries(optimalpartitioning PRIVATE oplib)
endif()