add_library(controller INTERFACE)

target_include_directories(controller
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(controller
    INTERFACE
        dualshock
)