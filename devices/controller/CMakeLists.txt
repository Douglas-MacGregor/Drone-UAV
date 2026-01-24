add_library(controller INTERFACE)

target_include_directories(controller
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(controller
    INTERFACE
        dualshock
)