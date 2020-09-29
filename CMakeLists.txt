cmake_minimum_required(VERSION 3.0.0)
project(apue VERSION 0.1.0)
set(CMAKE_DEBUG_POSTFIX "d")

include(CTest)
enable_testing()
add_executable(apue main.cpp)

find_package(ZLIB REQUIRED)
find_package(hdf5 CONFIG REQUIRED)
find_package(TBB CONFIG REQUIRED)

if(UNIX) 
    set(Boost_USE_STATIC_LIBS ON)
    find_package(Threads)
    target_link_libraries(apue PRIVATE hdf5::hdf5-static hdf5::hdf5_hl-static hdf5::hdf5_cpp-static hdf5::hdf5_hl_cpp-static)
    target_link_libraries(apue PRIVATE ${CMAKE_THREAD_LIBS_INIT})
else()
    target_link_libraries(apue PRIVATE hdf5::hdf5-shared hdf5::hdf5_hl-shared hdf5::hdf5_cpp-shared hdf5::hdf5_hl_cpp-shared)
endif()

find_package(Boost 1.72.0 REQUIRED COMPONENTS chrono filesystem)
target_link_libraries(apue PRIVATE TBB::tbb)
target_link_libraries(apue PRIVATE Boost::filesystem Boost::chrono)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)