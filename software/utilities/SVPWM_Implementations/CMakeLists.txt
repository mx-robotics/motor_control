cmake_minimum_required(VERSION 3.15)
project(SVPWM_Implementations)

set(CMAKE_CXX_STANDARD 17)
find_package(Python2 COMPONENTS Development NumPy)
add_executable(SVPWM_Implementations main.cpp matplotlibcpp.h)

#target_include_directories(SVPWM_Implementations PRIVATE ${/usr/bin} ${/usr/local/lib/python2.7/dist-packages/numpy})
target_include_directories(SVPWM_Implementations PRIVATE ${/usr/bin} ${/usr/local/lib/python2.7/dist-packages/numpy/core/include/numpy})
target_link_libraries(SVPWM_Implementations Python2::Python)
