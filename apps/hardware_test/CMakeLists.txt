add_executable(tx tx.c)
add_executable(rx rx.c)

target_link_libraries(tx PRIVATE sx1278 platform_hal)
target_link_libraries(rx PRIVATE sx1278 platform_hal)