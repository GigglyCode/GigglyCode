cmake_minimum_required(VERSION 3.29)
project(GigglyCode)

set(CMAKE_CXX_STANDARD 17)

set(BUILD_ARCH "-m64")
set(TARGET_64 ON)

# import llvm
find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(GigglyCode src/main.cpp)

llvm_map_components_to_libnames (llvm_libs
                                Analysis
                                Core
                                ExecutionEngine
                                InstCombine
                                Object
                                OrcJIT
                                RuntimeDyld
                                ScalarOpts
                                Support
                                native
    )

target_link_libraries(GigglyCode ${llvm_libs})
