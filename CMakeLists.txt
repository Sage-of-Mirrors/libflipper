cmake_minimum_required(VERSION 3.8)
project(libflipper)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE LIBFLIPPER_SRC
  "src/*.cpp"
  "include/*.hpp"
)

add_library(libflipper STATIC ${LIBFLIPPER_SRC})
target_include_directories(libflipper PUBLIC include ${GLM_INCLUDE_DIR})
target_link_libraries(libflipper PUBLIC glm)