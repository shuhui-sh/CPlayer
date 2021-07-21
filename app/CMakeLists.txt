cmake_minimum_required(VERSION 3.4.1)

#声明全局要编译的cpp，指定路径下的所有cpp都要编译进去，下面add_library会使用这个声明的SOURCE变量
file(GLOB SOURCE src/main/cpp/*.cpp)

add_library(
        cplayer
        SHARED
        ${SOURCE})

find_library( # Sets the name of the path variable.
        log-lib
        log)

#编译用到的头文件路径
include_directories(src/main/cpp/include)

#设置要编译的ABI文件路径到变量my_lib_path
set(my_lib_path ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${my_lib_path}")

target_link_libraries(
        cplayer
        avfilter avformat avcodec avutil swresample swscale
        ${log-lib}
        android
        z
        OpenSLES
)
