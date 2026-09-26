cmake_minimum_required(VERSION 3.10)
project(adad)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
add_executable(adad
	src/s9_cflow/cflow.cpp
	src/s9_adad/core/ev.cpp
	src/main.cpp
)