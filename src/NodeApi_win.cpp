// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "NodeApi.h"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

namespace Microsoft::NodeApiJsi {

LibHandle LibLoader::loadLib(const char *libName) {
  return reinterpret_cast<LibHandle>(LoadLibraryA(libName));
}

FuncPtr LibLoader::getFuncPtr(LibHandle libHandle, const char *funcName) {
  return reinterpret_cast<FuncPtr>(GetProcAddress(reinterpret_cast<HMODULE>(libHandle), funcName));
}

} // namespace Microsoft::NodeApiJsi
