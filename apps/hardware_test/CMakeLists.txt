add_executable(tx tx.c)
add_executable(rx rx.c)

target_include_directories(tx PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/hal
    ${PROJECT_SOURCE_DIR}/devices/lora
)

target_include_directories(rx PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/hal
    ${PROJECT_SOURCE_DIR}/devices/lora
)

target_link_libraries(tx PRIVATE sx1278 hal)
target_link_libraries(rx PRIVATE sx1278 hal)