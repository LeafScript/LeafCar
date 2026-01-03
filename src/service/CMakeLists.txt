add_subdirectory(service_shared)
set(dep service_shared_lib)
if(PRODUCT STREQUAL "leafcar")
    add_subdirectory(service_leafcar)
    list(APPEND dep service_leafcar_lib)
elseif(PRODUCT STREQUAL "standcar")
    add_subdirectory(service_standcar)
    list(APPEND dep service_standcar_lib)
endif()

add_library(service_lib INTERFACE)
target_link_libraries(service_lib INTERFACE
    project_common
    ${dep}
)