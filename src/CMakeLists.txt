add_library(
  cgit_lib
    OBJECT
    main.cc
    data.cc
    base.cc
    config.cc
    fs.cc
)

target_include_directories(
  cgit_lib PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)
