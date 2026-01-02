add_subdirectory(bsp_shared)
set(dep bsp_shared_lib)
if(PRODUCT STREQUAL "leafcar")
    add_subdirectory(bsp_leafcar)
    list(APPEND dep bsp_leafcar_lib)
elseif(PRODUCT STREQUAL "standcar")
    add_subdirectory(bsp_standcar)
    list(APPEND dep bsp_standcar_lib)
endif()

add_library(bsp_lib INTERFACE)
target_link_libraries(bsp_lib INTERFACE
    project_common
    ${dep}
)