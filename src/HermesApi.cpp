// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "HermesApi.h"

namespace Microsoft::NodeApiJsi {

namespace {

struct HermesApiNames {
#define HERMES_FUNC(func) static constexpr const char func[] = #func;
#include "HermesApiFunctions.inc"
};

} // namespace

thread_local HermesApi *HermesApi::current_{};

HermesApi::HermesApi(IFuncResolver *funcResolver)
    : NodeApi(funcResolver)
#define HERMES_FUNC(func) \
  , func(&ApiFuncResolver<NodeApi, decltype(::func) *, HermesApiNames::func, offsetof(HermesApi, func)>::stub)
#include "HermesApiFunctions.inc"
{
}

HermesApi *HermesApi::fromLib() {
  static LibFuncResolver funcResolver("hermes");
  static HermesApi *libHermesApi = new HermesApi(&funcResolver);
  return libHermesApi;
}

} // namespace Microsoft::NodeApiJsi
