add_executable(test_bbxtool test_bbxtool.c)

add_executable(test_server_inner test_server_inner.c)
target_link_libraries(test_server_inner spx_server_inner argobots)

add_executable(test_encode test_encode.c)

add_executable(test_server test_server.c)
target_link_libraries(test_server spx_server argobots spx_server spx_server_inner)

add_executable(test_client test_client.c)
target_link_libraries(test_client spx_client argobots)

add_executable(test_hash test_hash.c)
target_link_libraries(test_hash spx_hash)