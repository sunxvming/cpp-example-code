cmake_minimum_required(VERSION 3.7)

project(
    coord-convert
    VERSION 0.1.0
    LANGUAGES CXX C
)
SET(PROJECT_NAME "coord-convert")

SET(ROOT_DIR ${CMAKE_SOURCE_DIR})

# set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_BUILD_TYPE "Release")




find_package(OpenSceneGraph REQUIRED COMPONENTS osg osgDB osgGA osgUtil osgViewer osgText)

find_package(osgEarth CONFIG REQUIRED)

# add_executable(myApp main_city.cpp)
add_executable(${PROJECT_NAME} main.cpp)

# target_link_libraries(myApp PRIVATE osgEarth)
message(STATUS "CMAKE_BUILD_TYPE=====: ${CMAKE_BUILD_TYPE}")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug build")
    target_link_libraries(${PROJECT_NAME} PRIVATE ${osgEarth_LIBRARIES_DEBUG})
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Release build")
    target_link_libraries(${PROJECT_NAME} PRIVATE ${osgEarth_LIBRARIES})
else()
    message("Unknown build type: ${CMAKE_BUILD_TYPE}")
endif()



add_library(cppnet STATIC IMPORTED)
set_property(TARGET cppnet PROPERTY IMPORTED_LOCATION ${ROOT_DIR}/CppNet/lib/cppnet.lib)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENSCENEGRAPH_LIBRARIES} cppnet)



include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})
include_directories(${osgEarth_INCLUDE_DIRS})
include_directories(${ROOT_DIR}/CppNet/include)

message(STATUS "OPENSCENEGRAPH_LIBRARIES====debug: ${osgEarth_LIBRARIES_DEBUG}")
message(STATUS "OPENSCENEGRAPH_LIBRARIES====Release: ${osgEarth_LIBRARIES}")


message(STATUS "OPENSCENEGRAPH_LIBRARIES=====: ${OPENSCENEGRAPH_LIBRARIES}")
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)