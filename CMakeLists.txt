cmake_minimum_required(VERSION 3.20)

project(
        libsched
        VERSION 0.0.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(libsched
        lib/libsched/scheduler.h
        lib/libsched/scheduler/scheduler.h
        lib/libsched/task/task.h
        lib/libsched/wrappers/callable_wrapper.h
        lib/libsched/wrappers/executable.h
        lib/libsched/wrappers/callable.h
        lib/libsched/task_id/task_id.h
        lib/libsched/future/future_result.h

)

set_target_properties(libsched PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(libsched PUBLIC lib/libsched)

add_executable(demo demo/demo.cpp)
target_include_directories(demo PRIVATE lib/)
target_link_libraries(demo libsched)

include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(libschedtest test/libschedtest.cpp)
target_link_libraries(libschedtest libsched gtest gtest_main)
target_include_directories(libschedtest PRIVATE lib/)

include(GoogleTest)

gtest_discover_tests(libschedtest)
