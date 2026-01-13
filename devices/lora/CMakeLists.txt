add_library(lora INTERFACE)

target_include_directories(lora
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(lora
    INTERFACE
        sx1278
)