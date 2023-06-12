# Copyright (c) Microsoft Corporation.
# Licensed under the MIT License.

add_executable(jsi_tests
  "../jsi/jsi/test/testlib.h"
  "../jsi/jsi/test/testlib.cpp"
  "../jsi/jsi/test/testlib_ext.cpp"
  "../jsi/jsi/decorator.h"
  "../jsi/jsi/instrumentation.h"
  "../jsi/jsi/jsi-inl.h"
  "../jsi/jsi/jsi.cpp"
  "../jsi/jsi/jsi.h"
  "../jsi/jsi/jsilib-posix.cpp"
  "../jsi/jsi/jsilib-windows.cpp"
  "../jsi/jsi/jsilib.h"
  "../jsi/jsi/threadsafe.h"
  "../src/ApiLoaders/HermesApi.cpp"
  "../src/ApiLoaders/HermesApi.h"
  "../src/ApiLoaders/JSRuntimeApi.cpp"
  "../src/ApiLoaders/JSRuntimeApi.h"
  "../src/ApiLoaders/NodeApi_win.cpp"
  "../src/ApiLoaders/NodeApi.cpp"
  "../src/ApiLoaders/NodeApi.h"
  "../src/ApiLoaders/V8Api.cpp"
  "../src/ApiLoaders/V8Api.h"
  "../src/NodeApiJsiRuntime.cpp"
  "../src/NodeApiJsiRuntime.h"
  "JsiRuntimeTests.cpp"
)

target_include_directories(jsi_tests PUBLIC ../jsi ../src ../node-api)
target_link_libraries(jsi_tests PUBLIC gtest_main)

find_program(NUGET_EXE NAMES nuget)
if(NOT NUGET_EXE)
  message("NUGET.EXE not found.")
  message(FATAL_ERROR "Please install this executable, and run CMake again.")
endif()

execute_process(
  COMMAND ${NUGET_EXE}
    install "Microsoft.JavaScript.Hermes"
    -Version 0.1.14
    -ExcludeVersion
    -OutputDirectory ${CMAKE_BINARY_DIR}/packages
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(jsi_tests PRIVATE ${CMAKE_BINARY_DIR}/packages/Microsoft.JavaScript.Hermes/build/native/Microsoft.JavaScript.Hermes.targets)

execute_process(
  COMMAND ${NUGET_EXE}
    install "ReactNative.V8Jsi.Windows"
    -Version 0.71.5
    -ExcludeVersion
    -OutputDirectory ${CMAKE_BINARY_DIR}/packages
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(jsi_tests PRIVATE ${CMAKE_BINARY_DIR}/packages/ReactNative.V8Jsi.Windows/build/native/ReactNative.V8Jsi.Windows.targets)

enable_testing()

add_test(
  NAME jsi_tests
  COMMAND $<TARGET_FILE:jsi_tests>
)