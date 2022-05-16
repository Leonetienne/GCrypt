cmake_minimum_required(VERSION 3.16)
project(gecrypt)

set(CMAKE_CXX_STANDARD 17)

# Add library StringTools
SET(stringtools_dir ../StringTools/StringTools)
SET(stringtools_include ${stringtools_dir}/include)
FILE(GLOB stringtools_src ${stringtools_dir}/src/*.cpp)

# Add library GeneralUtility
SET(generalutility_dir ../GeneralUtility/GeneralUtility)
SET(generalutility_include ${generalutility_dir}/include)
FILE(GLOB generalutility_src ${generalutility_dir}/src/*.cpp)

# Add library Hazelnupp
SET(hazelnupp_dir ../Hazelnupp/Hazelnupp)
SET(hazelnupp_include ${hazelnupp_dir}/include)
FILE(GLOB hazelnupp_src ${hazelnupp_dir}/src/*.cpp)

# Add library GCrypt
SET(gcrypt_dir ../GCryptLib)
SET(gcrypt_include ${gcrypt_dir}/include)
FILE(GLOB gcrypt_src ${gcrypt_dir}/src/*.cpp)

FILE(GLOB main_src src/*.cpp)

add_executable(${PROJECT_NAME}
  ${main_src}

  ${stringtools_src}
  ${generalutility_src}
  ${hazelnupp_src}
  ${gcrypt_src}
)

target_include_directories(${PROJECT_NAME} PRIVATE
  include
  ${stringtools_include}
  ${generalutility_include}
  ${hazelnupp_include}
  ${gcrypt_include}
)

