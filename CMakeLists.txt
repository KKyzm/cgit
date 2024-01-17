cmake_minimum_required(VERSION 3.15)
project(cgit CXX)

add_compile_definitions("DEBUG=$<CONFIG:Debug>")
add_executable(cgit main.cc)

find_package(cryptopp CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(docopt CONFIG REQUIRED)

set(CGIT_THIRDPARTY_LIBS
  cryptopp::cryptopp
  docopt_s
  fmt::fmt
)

add_subdirectory(src)

set(CGIT_LIBS
  cgit_lib
)

target_link_libraries(
  cgit
    ${CGIT_LIBS}
    ${CGIT_THIRDPARTY_LIBS}
)
