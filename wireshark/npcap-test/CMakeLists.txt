cmake_minimum_required(VERSION 3.0)

project(npcap)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")

# 定义工程根目录; CMAKE_SOURCE_DIR为内建变量，表示工程根目录的CMakeLists.txt文件路径
SET(ROOT_DIR ${CMAKE_SOURCE_DIR})



# 编译那些源码
file(GLOB SOURCES "src/*.c")

# 编译目标
add_executable(${PROJECT_NAME} ${SOURCES})

# 包含路径
target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${ROOT_DIR}/include
)

# 添加外部静态
add_library(wpcap STATIC IMPORTED)


set_property(TARGET wpcap PROPERTY IMPORTED_LOCATION ${ROOT_DIR}/libs/wpcap.lib)
# set_property(TARGET wpcap PROPERTY IMPORTED_LOCATION ${GLUT_DIR}/libs/Packet.lib)


target_link_libraries(${PROJECT_NAME} wpcap ) # 添加freeglut.lib


set(EXECUTABLE_OUTPUT_PATH ${ROOT_DIR}/bin)
