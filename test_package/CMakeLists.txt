project(OmrConanTestPackage)
cmake_minimum_required(VERSION 3.7)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
add_executable(main main.cpp)
set_property(TARGET main PROPERTY CXX_STANDARD 11)
target_link_libraries(main ${CONAN_LIBS})
