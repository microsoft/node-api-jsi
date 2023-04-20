# Copyright (c) Microsoft Corporation.
# Licensed under the MIT License.

# set minimum cmake version
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
cmake_policy(SET CMP0092 NEW)

# project name and language
project(NodeApiJsi
  HOMEPAGE_URL "https://github.com/microsoft/node-api-jsi"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(external)
add_subdirectory(tests)
