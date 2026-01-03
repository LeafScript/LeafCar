add_subdirectory(debug_shared)
set(dep debug_shared_lib)
if(PRODUCT STREQUAL "leafcar")
    add_subdirectory(debug_leafcar)
    list(APPEND dep debug_leafcar_lib)
elseif(PRODUCT STREQUAL "standcar")
    add_subdirectory(debug_standcar)
    list(APPEND dep debug_standcar_lib)
endif()

add_library(debug_lib INTERFACE)
target_link_libraries(debug_lib INTERFACE
    project_common
    ${dep}
)