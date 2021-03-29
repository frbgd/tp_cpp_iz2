cmake_minimum_required(VERSION 3.12)
project(tp_cpp_iz2 C)

set(CMAKE_C_STANDARD 11)

set(EMPLOYEE_LIB_SOURCE_FILES include/employee.h src/employee.c src/consistent.c)
add_library(employee_consistent_lib STATIC ${EMPLOYEE_LIB_SOURCE_FILES})

add_executable(prog src/main.c)
target_link_libraries(prog employee_lib)
