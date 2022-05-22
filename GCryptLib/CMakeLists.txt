cmake_minimum_required(VERSION 3.16)
project(GCrypt)

###################
# Library project #
###################
set(CMAKE_CXX_STANDARD 17)

FILE(GLOB main_src src/*.cpp)
add_library(${PROJECT_NAME}
  ${main_src}
)

target_include_directories(${PROJECT_NAME} PRIVATE
  include
)

target_compile_options(${PROJECT_NAME} PRIVATE
  -Werror
  -fdiagnostics-color=always
)

#########
# Tests #
#########
FILE(GLOB test_src test/*.cpp)
add_executable(test
  test/Catch2.h
  ${test_src}
)
target_link_libraries(test ${PROJECT_NAME})

target_include_directories(test PRIVATE
  include
)

target_compile_options(test PRIVATE
  -Werror
  -fdiagnostics-color=always
)


## Move test images to build dir
ADD_CUSTOM_COMMAND(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/test/testAssets/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/testAssets/
)


##############
# Executable #
##############
FILE(GLOB exec_src exec/*.cpp)
add_executable(exec
  ${exec_src}
)
target_link_libraries(exec ${PROJECT_NAME})

target_include_directories(exec PRIVATE
  include
)

target_compile_options(exec PRIVATE
  -Werror
  -fdiagnostics-color=always
)

