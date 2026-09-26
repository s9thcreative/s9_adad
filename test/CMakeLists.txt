cmake_minimum_required(VERSION 3.10)
project(adad_test)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt")
find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

add_executable(t
	../src/s9_cflow/cflow.cpp
	../src/s9_adad/core/ev.cpp
	../src/s9_adad/core/tb.cpp
	../src/s9_adad/core/ac.cpp
	../src/s9_adad/core/pg.cpp
	../src/s9_adad/view/vdt.cpp
	../src/s9_adad/view/vdt.hpp
	../src/s9_adad/view/vtg.cpp
	../src/s9_adad/view/vtg.hpp
	../src/s9_adad/view/vlib.cpp
	../src/s9_adad/view/vlib.hpp
	../src/test.cpp
)

# Qt link
target_link_libraries(t PRIVATE Qt6::Widgets Qt6::Network)
