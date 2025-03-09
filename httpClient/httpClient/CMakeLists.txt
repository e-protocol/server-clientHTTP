file(GLOB SRC ${PROJECT_SOURCE_DIR} *.h *.cpp)
set(BOOST "D://Programs//VisualStudio2022//libs//boost_1_83_0")
#file(GLOB BOOST_SRC ${BOOST}//boost *.hpp *.cpp)
add_executable (${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} PUBLIC ${BOOST})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET httpClient PROPERTY CXX_STANDARD 20)
endif()


