file(GLOB_RECURSE BSP_SRCS "*.c")
add_library(bsp_shared_lib STATIC ${BSP_SRCS})
target_link_libraries(bsp_shared_lib PRIVATE
    project_common
    st_lib
)