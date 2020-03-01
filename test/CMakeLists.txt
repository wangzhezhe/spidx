
add_executable(test_bbxtool test_bbxtool.c)

add_executable(test_server_inner test_server_inner.c)
target_link_libraries(test_server_inner spx_server_inner argobots)

