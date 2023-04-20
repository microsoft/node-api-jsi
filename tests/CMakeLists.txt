# Copyright (c) Microsoft Corporation.
# Licensed under the MIT License.

add_executable(jsi_tests
  "../jsi/test/testlib.h"
  "../jsi/test/testlib.cpp"
  "../jsi/test/testlib_ext.cpp"
  "../jsi/decorator.h"
  "../jsi/instrumentation.h"
  "../jsi/jsi-inl.h"
  "../jsi/jsi.cpp"
  "../jsi/jsi.h"
  "../jsi/jsilib-posix.cpp"
  "../jsi/jsilib-windows.cpp"
  "../jsi/jsilib.h"
  "../jsi/threadsafe.h"
  "../src/HermesApi.cpp"
  "../src/HermesApi.h"
  "../src/NodeApi_win.cpp"
  "../src/NodeApi.cpp"
  "../src/NodeApi.h"
  "../src/NodeApiJsiRuntime.cpp"
  "../src/NodeApiJsiRuntime.h"
  "JsiRuntimeTests.cpp"
)

target_include_directories(jsi_tests PUBLIC .. ../src)
target_link_libraries(jsi_tests PUBLIC gtest_main)

find_program(NUGET_EXE NAMES nuget)
if(NOT NUGET_EXE)
  message("NUGET.EXE not found.")
  message(FATAL_ERROR "Please install this executable, and run CMake again.")
endif()

exec_program(${NUGET_EXE}
  ARGS
    install "Microsoft.JavaScript.Hermes"
    -Version 0.1.5
    -ExcludeVersion
    -OutputDirectory ${CMAKE_BINARY_DIR}/packages)

target_link_libraries(jsi_tests PRIVATE ${CMAKE_BINARY_DIR}/packages/Microsoft.JavaScript.Hermes/build/native/Microsoft.JavaScript.Hermes.targets)

add_test(
  NAME jsi_tests
  COMMAND $<TARGET_FILE:jsi_tests>
)