add_executable(test_bbxtool test_bbxtool.c)

add_executable(test_server_inner test_server_inner.c)
target_link_libraries(test_server_inner spx_server_inner argobots)

add_executable(test_encode test_encode.c)

add_executable(test_server test_server.c)
target_link_libraries(test_server spx_server argobots spx_server spx_server_inner)

add_executable(test_client test_client.c)
target_link_libraries(test_client spx_client argobots)

add_executable(test_hash_domain test_hash_domain.c)
target_link_libraries(test_hash_domain spx_hash_domain)

add_executable(test_hash_mod test_hash_mod.c)
target_link_libraries(test_hash_mod spx_hash_mod)

add_test( NAME test_bbxtool COMMAND test_bbxtool)

add_test( NAME test_server_inner COMMAND test_server_inner)

add_test( NAME test_encode COMMAND test_encode)

add_test( NAME test_hash_domain COMMAND test_hash_domain)

add_test( NAME spx_hash_mod COMMAND spx_hash_mod)

#test the server and the client manually