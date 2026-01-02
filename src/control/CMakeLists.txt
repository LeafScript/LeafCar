if(PRODUCT STREQUAL "leafcar")
    add_subdirectory(control_leafcar)
    set(dep "control_leafcar_lib")
elseif(PRODUCT STREQUAL "standcar")
    add_subdirectory(control_standcar)
    set(dep "control_standcar_lib")
endif()

add_library(control_lib INTERFACE)
target_link_libraries(control_lib INTERFACE
    project_common
    ${dep}
)