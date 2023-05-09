// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "NodeApi.h"

EXTERN_C_START

extern napi_status NAPI_CDECL default_napi_ext_get_description(napi_env env, char *buf, size_t bufsize, size_t *result);

extern napi_status NAPI_CDECL default_napi_ext_drain_microtasks(napi_env env, int32_t max_count_hint, bool *result);

extern napi_status NAPI_CDECL default_napi_ext_is_inspectable(napi_env env, bool *result);

extern napi_status NAPI_CDECL default_napi_ext_create_prepared_script(
    napi_env env,
    uint8_t *script_data,
    size_t script_length,
    napi_finalize finalize_cb,
    void *finalize_hint,
    const char *source_url,
    napi_ext_prepared_script *result);

extern napi_status NAPI_CDECL
default_napi_ext_delete_prepared_script(napi_env env, napi_ext_prepared_script prepared_script);

extern napi_status NAPI_CDECL
default_napi_ext_prepared_script_run(napi_env env, napi_ext_prepared_script prepared_script, napi_value *result);

EXTERN_C_END

namespace Microsoft::NodeApiJsi {

namespace {

struct NodeApiNames {
#define NODE_API_FUNC(func) static constexpr const char func[] = #func;
#define NODE_API_EXT_FUNC NODE_API_FUNC
#define NODE_API_PREPARED_SCRIPT NODE_API_FUNC
#include "NodeApiFunctions.inc"
};

// Prepared script function either should be all loaded or we use all default functions.
void loadPreparedScriptFuncs() {
  NodeApi *current = NodeApi::current();
  bool useDefault = false;
#define NODE_API_PREPARED_SCRIPT(func)                                                                             \
  decltype(::func) *loaded_##func = reinterpret_cast<decltype(::func) *>(current->getFuncPtr(NodeApiNames::func)); \
  useDefault = useDefault || loaded_##func == nullptr;
#include "NodeApiFunctions.inc"
#define NODE_API_PREPARED_SCRIPT(func)                                                        \
  size_t offset_##func = offsetof(NodeApi, func);                                             \
  *reinterpret_cast<decltype(::func) **>(reinterpret_cast<char *>(current) + offset_##func) = \
      useDefault ? &default_##func : loaded_##func;
#include "NodeApiFunctions.inc"
}

} // namespace

LibFuncResolver::LibFuncResolver(const char *libName) : libHandle_(LibLoader::loadLib(libName)) {}

FuncPtr LibFuncResolver::getFuncPtr(const char *funcName) {
  return LibLoader::getFuncPtr(libHandle_, funcName);
}

DelayLoadedApi::DelayLoadedApi(IFuncResolver *funcResolver) : funcResolver_(funcResolver) {}

DelayLoadedApi::~DelayLoadedApi() = default;

FuncPtr DelayLoadedApi::getFuncPtr(const char *funcName) {
  return funcResolver_->getFuncPtr(funcName);
}

thread_local NodeApi *NodeApi::current_{};

NodeApi::NodeApi(IFuncResolver *funcResolver)
    : DelayLoadedApi(funcResolver)
#define NODE_API_FUNC(func) \
  , func(&ApiFuncResolver<NodeApi, decltype(::func) *, NodeApiNames::func, offsetof(NodeApi, func)>::stub)
#define NODE_API_EXT_FUNC(func)                                                                                      \
  ,                                                                                                                  \
      func(&ApiFuncResolver<NodeApi, decltype(::func) *, NodeApiNames::func, offsetof(NodeApi, func)>::optionalStub< \
           &default_##func>)
#define NODE_API_PREPARED_SCRIPT(func)                                                                              \
  ,                                                                                                                 \
      func(&ApiFuncResolver<NodeApi, decltype(::func) *, NodeApiNames::func, offsetof(NodeApi, func)>::preloadStub< \
           &loadPreparedScriptFuncs>)
#include "NodeApiFunctions.inc"
{
}

} // namespace Microsoft::NodeApiJsi
