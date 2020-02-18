
add_executable(test_bbxtool test_bbxtool.c)

add_executable(test_server_init test_server_init.c)
target_link_libraries(test_server_init spidx_server margo)

add_executable(test_api_init test_api_init.c)
target_link_libraries(test_api_init margo)