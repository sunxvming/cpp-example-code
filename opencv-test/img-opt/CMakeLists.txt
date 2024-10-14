cmake_minimum_required(VERSION 3.7)

project(
    shipar
    VERSION 0.1.0
    LANGUAGES CXX C
)
SET(PROJECT_NAME "shipar")
SET(ROOT_DIR ${CMAKE_SOURCE_DIR})


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



# Specify MSVC UTF-8 encoding   
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")


# ====================find_package====================
find_package(OpenCV CONFIG REQUIRED)

# ====================include_directories====================
include_directories( ${OpenCV_INCLUDE_DIRS} )



file(GLOB SRC "*.cpp")
set(SOURCES ${SRC})  


add_executable(${PROJECT_NAME} ${SRC} )


# ====================link_libraries====================

target_link_libraries(${PROJECT_NAME} PRIVATE  opencv_video opencv_core opencv_videoio opencv_highgui opencv_imgproc  opencv_calib3d opencv_ml opencv_flann )


set(EXECUTABLE_OUTPUT_PATH ${ROOT_DIR}/bin)


