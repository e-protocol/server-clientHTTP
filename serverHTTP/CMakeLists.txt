cmake_minimum_required(VERSION 3.30.5)
project(serverHTTP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BOOST_ROOT /usr/include/boost)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.83 REQUIRED)


if(Boost_FOUND)
   include_directories(${Boost_INCLUDE_DIR})
endif()


#set(BOOST /usr/arm-linux-gnueabihf/include/boost)
#set(Boost_USE_STATIC_LIBS ON)
#include_directories(${BOOST})

add_subdirectory(src)
#add_subdirectory(armhf)
target_link_libraries(${PROJECT_NAME} Boost::headers)