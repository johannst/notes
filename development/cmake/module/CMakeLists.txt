cmake_minimum_required(VERSION 3.10)
project(hello-pkg)

# Include as file.
include(cmake/moose.cmake)
# Include as module (search CMAKE_MODULE_PATH, then builtin paths).
include(moose)

# -- Foo package ---------------------------------------------------------------

# Find module (uses CMAKE_MODULE_PATH, Foo_DIR, builtin paths).
message("==> find_package(Foo MODULE)")
find_package(Foo MODULE)

# Find config (uses CMAKE_PREFIX_PATH, Foo_DIR, builtin paths).
message("==> find_package(Foo CONFIG)")
find_package(Foo CONFIG REQUIRED)

# -- Bar package ---------------------------------------------------------------

# Find module (uses CMAKE_MODULE_PATH, Bar_DIR, builtin paths).
message("==> find_package(Bar MODULE)")
find_package(Bar MODULE)

# Find config (uses CMAKE_PREFIX_PATH, Bar_DIR, builtin paths).
message("==> find_package(Bar CONFIG)")
find_package(Bar CONFIG REQUIRED)
