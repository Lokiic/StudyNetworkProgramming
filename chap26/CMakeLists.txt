cmake_minimum_required(VERSION 3.0)

project(THREAD)

include_directories(../lib)



add_executable(thread_dynamic Thread_dynamic.c Echo.c)

add_executable(thread_pool Thread_pool.c Echo.c)

add_executable(race_condition RaceCondition.c Echo.c)

target_link_libraries(thread_dynamic pthread)

target_link_libraries(thread_pool pthread)

target_link_libraries(race_condition pthread)