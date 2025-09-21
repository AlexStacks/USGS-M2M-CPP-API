cmake_minimum_required(VERSION 3.10)
project(usgsm2mcpp)

set(usgsM2M_Sources
    src/usgsm2m.cpp
)

add_library(usgsm2mcpp SHARED ${usgsM2M_Sources})


target_include_directories(usgsm2mcpp PRIVATE 
"${CMAKE_CURRENT_SOURCE_DIR}/inc" 
)

set_target_properties(usgsm2mcpp PROPERTIES VERSION "0.0.2")

target_link_libraries(usgsm2mcpp
    -lcurl
    )