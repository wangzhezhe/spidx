cmake_minimum_required (VERSION 3.14)
project (spidx C)

option(ENABLE_TESTING "Enable Testing" ON)

if (ENABLE_TESTING)
  add_subdirectory(test)
endif()