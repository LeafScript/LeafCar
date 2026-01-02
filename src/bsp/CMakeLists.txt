add_subdirectory(bsp_shared)
add_subdirectory(bsp_leafcar)

set(dep "bsp_shared_lib")
set(dep "bsp_leafcar_lib")

file(GLOB_RECURSE BSP_SRCS "*.c")
add_library(bsp_lib STATIC ${BSP_SRCS})
target_link_libraries(bsp_lib PRIVATE
    project_common
    ${dep}
)