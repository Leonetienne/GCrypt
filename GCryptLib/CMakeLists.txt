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


## Move test assest to build dir
ADD_CUSTOM_COMMAND(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/test/testAssets/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/testAssets/
)


###############
# Executables #
###############
FILE(GLOB bmpp_src exec/BmpPP/BmpPP/src/*.cpp)
FILE(GLOB eule_src exec/Eule/Eule/src/*.cpp)
add_compile_definitions(_EULE_NO_INTRINSICS_)
include_directories(
  include
  exec/BmpPP/BmpPP/include
  exec/Eule/Eule/include
)

## Move exec assets to build dir
ADD_CUSTOM_COMMAND(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/exec/execAssets/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/execAssets/
)

function(DECLARE_EXEC_EXAMPLE name)
  add_executable(example-${name} exec/${name}.cpp ${bmpp_src} ${eule_src})
  target_link_libraries(example-${name} ${PROJECT_NAME})
  target_compile_options(example-${name} PRIVATE -Werror -fdiagnostics-color=always)
endfunction()

# These are the names of the cpp files in /exec/, without the ".cpp".
DECLARE_EXEC_EXAMPLE(encrypt-decrypt-files)
DECLARE_EXEC_EXAMPLE(encrypt-decrypt-strings)
DECLARE_EXEC_EXAMPLE(benchmark-encryption)
DECLARE_EXEC_EXAMPLE(benchmark-prng)
DECLARE_EXEC_EXAMPLE(visualize-singleblock-diffusion)
DECLARE_EXEC_EXAMPLE(visualize-multiblock-diffusion)
DECLARE_EXEC_EXAMPLE(visualize-extreme-input-diffusion)
DECLARE_EXEC_EXAMPLE(visualize-prng-distribution)
DECLARE_EXEC_EXAMPLE(visualize-hashing-distribution)

