cmake_minimum_required(VERSION 3.19)
project(json_to_ts C)

set(CMAKE_C_STANDARD 11)

add_executable(json_to_ts src/main.c src/types.h src/types.c src/merge_types.c src/merge_types.h src/io.c src/io.h src/print_type_as_ts.c src/print_type_as_ts.h src/json_to_type.c src/json_to_type.h)
