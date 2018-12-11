cmake_minimum_required (VERSION 3.5)
project (test_lib)
SET(CMAKE_CXX_FLAGS "-std=c++0x")
#add_library (openlsm openlsm.cpp)
add_library (openlsm SHARED openlsm.cpp)
#if you make a shared library the executable needs to use the library, 
#I usually put them in the same folder

#=====you don't need things below================#
target_include_directories (openlsm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable (test_openlsm main.cpp)

target_link_libraries (test_openlsm LINK_PUBLIC openlsm)
