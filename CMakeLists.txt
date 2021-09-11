cmake_minimum_required(VERSION 3.19)
project(json_to_ts C)

set(CMAKE_C_STANDARD 11)

add_executable(json_to_ts main.c types.h types.c merge_types.c merge_types.h io.c io.h print_type_as_ts.c print_type_as_ts.h json_to_type.c json_to_type.h)
