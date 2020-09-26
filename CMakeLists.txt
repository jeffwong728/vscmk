cmake_minimum_required(VERSION 3.0.0)
project(apue VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(ZLIB REQUIRED)
find_package(hdf5 CONFIG REQUIRED)
find_package(TBB CONFIG REQUIRED)
find_package(Threads)

add_executable(apue main.cpp)
target_link_libraries(apue PRIVATE TBB::tbb)
target_link_libraries(apue PRIVATE hdf5::hdf5-static hdf5::hdf5_hl-static hdf5::hdf5_cpp-static hdf5::hdf5_hl_cpp-static)
target_link_libraries(apue PRIVATE ${CMAKE_THREAD_LIBS_INIT})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)