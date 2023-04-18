cmake_minimum_required(VERSION 3.11) 

project(example)

find_package(OpenSSL REQUIRED)
add_executable(main dgst-sha512.c)
target_link_libraries(main OpenSSL::SSL)
