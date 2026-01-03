file(GLOB_RECURSE SRCS "*.c")
add_library(service_standcar_lib STATIC ${SRCS})
target_link_libraries(service_standcar_lib PRIVATE
    project_common
)