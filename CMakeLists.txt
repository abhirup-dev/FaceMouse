cmake_minimum_required(VERSION 2.8.12)
project(Dlib_tests)
set(cmake_cxx_flags "-std=c++14 -O3 -march=native")
include(/opt/dlib-19.16/dlib/cmake)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

macro(add_code name)
   add_executable(${name} ${name}.cpp)
   target_link_libraries(${name} dlib::dlib )
   target_link_libraries(${name} ${OpenCV_LIBS} )
   target_link_libraries(${name} Xtst )
   target_link_libraries(${name} X11 )
endmacro()

add_code(face_mouse)
add_code(landmark)