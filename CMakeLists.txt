cmake_minimum_required (VERSION 3.14)
project (spidx C)

option(ENABLE_TESTING "Enable Testing" ON)

# add our cmake module directory to the path
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
     "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include (xpkg-import)
find_package (mercury REQUIRED)
include_directories(${MERCURY_INCLUDE_DIR})
xpkg_import_module (argobots REQUIRED argobots)
xpkg_import_module (margo REQUIRED margo)
xpkg_import_module (abtio REQUIRED abt-io)

#add server code
add_library(spidx_server 
${spidx_SOURCE_DIR}/src/spx_server.c)
target_link_libraries(spidx_server margo)

#add client code

if (ENABLE_TESTING)
  add_subdirectory(test)
endif()

