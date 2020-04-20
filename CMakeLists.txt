cmake_minimum_required (VERSION 3.14)
project (spidx C)

option(ENABLE_TESTING "Enable Testing" ON)
enable_testing()

# add our cmake module directory to the path
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
     "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include (xpkg-import)
find_package (mercury REQUIRED)
include_directories(${MERCURY_INCLUDE_DIR})
xpkg_import_module (argobots REQUIRED argobots)
xpkg_import_module (margo REQUIRED margo)
xpkg_import_module (abtio REQUIRED abt-io)
xpkg_import_module (ssg REQUIRED ssg)

find_package(MPI REQUIRED) 


#add include dir
include_directories(${spidx_SOURCE_DIR}/include)


#add server code
add_library(spx_server_inner ${spidx_SOURCE_DIR}/src/spx_server_inner.c)
target_link_libraries(spx_server_inner argobots)

add_library(spx_server ${spidx_SOURCE_DIR}/src/spx_server.c)
target_link_libraries(spx_server argobots margo)

# hash_domain lib
add_library(spx_hash_domain ${spidx_SOURCE_DIR}/src/spx_hash_domain.c)
target_link_libraries(spx_hash_domain argobots)

# hash_module lib
add_library(spx_hash_mod 
${spidx_SOURCE_DIR}/src/spx_hash_mod.c
${spidx_SOURCE_DIR}/src/hashmodules/sfc.c
)
target_link_libraries(spx_hash_mod)

#add client code
add_library(spx_client ${spidx_SOURCE_DIR}/src/spx_client.c)
target_link_libraries(spx_client argobots margo)


if (ENABLE_TESTING)
  add_subdirectory(test)
endif()

