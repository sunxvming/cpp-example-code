cmake_minimum_required(VERSION 3.0)

project (main) 

set(CMAKE_CXX_STANDARD 17)

# 编译类型相关 Debug or Release
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -rdynamic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -rdynamic")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -rdynamic")
set(CMAKE_C_FLAGS_RELEASE  "${CMAKE_C_FLAGS_RELEASE} -rdynamic")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

message("CMAKE_BUILD_TYPE is:${CMAKE_BUILD_TYPE}")

# message("CMAKE_CXX_FLAGS_DEBUG :================")
# message(${CMAKE_CXX_FLAGS_DEBUG})

# message("CMAKE_CXX_FLAGS_RELEASE  :================")
# message(${CMAKE_CXX_FLAGS_RELEASE})

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    message(FATAL_ERROR "CMAKE_BUILD_TYPE must be Debug or Release!!!")
endif()



add_definitions(-Wno-builtin-macro-redefined)

# 打印编译时间
set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CMAKE_COMMAND} -E time")

set(CXX_FLAGS
        -Wall
        )
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -save-temps")

# 定义工程根目录; CMAKE_SOURCE_DIR为内建变量，表示工程根目录的CMakeLists.txt文件路径
SET(ROOT_DIR ${CMAKE_SOURCE_DIR})





# 编译那些源码
file(GLOB SOURCES 
    "*.cpp"
    "utils/*.cpp" 
    )

file(GLOB_RECURSE MAIN_FILE main.cpp)
list(REMOVE_ITEM SOURCES ${MAIN_FILE})

# message("source:================")
# message(${SOURCES})
# message("MAIN_FILE:================")
# message(${MAIN_FILE})




# 编译目标
add_executable(${PROJECT_NAME}  ${MAIN_FILE} ${SOURCES})
include (utils.cmake)
redefine_file_macro(${PROJECT_NAME})


set(EXECUTABLE_OUTPUT_PATH bin)
# 包含路径

include_directories("${ROOT_DIR}/utils")

include_directories("${ROOT_DIR}/spdlog/include")




# 添加动态库
target_link_libraries(${PROJECT_NAME}  dl)
target_link_libraries(${PROJECT_NAME}  z)
#target_link_libraries(${PROJECT_NAME}  bz2)
target_link_libraries(${PROJECT_NAME} "pthread" ) 



# 添加外部静态


add_library(spdlog STATIC IMPORTED)
set_property(TARGET spdlog PROPERTY IMPORTED_LOCATION ${ROOT_DIR}/spdlog/libspdlog.a)
target_link_libraries(${PROJECT_NAME}  spdlog )






