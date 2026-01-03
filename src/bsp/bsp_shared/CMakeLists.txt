file(GLOB_RECURSE SRCS "*.c")
add_library(bsp_shared_lib STATIC ${SRCS})
target_link_libraries(bsp_shared_lib PRIVATE
    project_common
    st_lib
)