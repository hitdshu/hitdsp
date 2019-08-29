cmake_minimum_required(VERSION 3.1.3)
project(hitdsp C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BUILD_TYPE Release)

include_directories(
${PROJECT_SOURCE_DIR}
)

file(GLOB COMMON_CC "${PROJECT_SOURCE_DIR}/common/*.cc")
file(GLOB FFT_CC "${PROJECT_SOURCE_DIR}/fft/*.cc")
file(GLOB FILTER_CC "${PROJECT_SOURCE_DIR}/filter/*.cc")
file(GLOB MODEM_CC "${PROJECT_SOURCE_DIR}/modem/*.cc")
file(GLOB ECC_CC "${PROJECT_SOURCE_DIR}/ecc/*.cc")

add_library(${PROJECT_NAME} SHARED
${COMMON_CC}
${FFT_CC}
${FILTER_CC}
${MODEM_CC}
${ECC_CC}
)

target_link_libraries(${PROJECT_NAME}
)

file(GLOB TEST_CC "${PROJECT_SOURCE_DIR}/test/*.cc")

add_executable(RunAllTest
${TEST_CC})
target_link_libraries(RunAllTest
${PROJECT_NAME})
