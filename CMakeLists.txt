cmake_minimum_required(VERSION 3.12)
project(tp_cpp_iz2)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDART_REQURED ON)

set(CMAKE_C_FLAGS "-g -O0 -Wall -Werror -Wextra -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")
#
#add_library(EmployeeStatic STATIC include/employee.h src/employee.c src/consistent.c)
#target_include_directories(EmployeeStatic PUBLIC include)

add_library(EmployeeDynamic SHARED include/employee.h src/employee.c src/parallel.c)
target_include_directories(EmployeeDynamic PUBLIC include)

add_executable(prog src/main.c)
target_link_libraries(prog EmployeeDynamic)
