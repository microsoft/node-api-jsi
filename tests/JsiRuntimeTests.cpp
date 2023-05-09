// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include <HermesApi.h>
#include <NodeApiJsiRuntime.h>
#include <hermes/hermes_api.h>
#include <vector>
#include "../jsi/test/testlib.h"

using namespace Microsoft::NodeApiJsi;

namespace facebook::jsi {
std::vector<RuntimeFactory> runtimeGenerators() {
  return {RuntimeFactory([]() {
    HermesApi *hermesApi = HermesApi::fromLib();
    HermesApi::setCurrent(hermesApi);

    hermes_config config{};
    hermes_runtime runtime{};
    napi_env env{};
    hermesApi->hermes_create_config(&config);
    hermesApi->hermes_create_runtime(config, &runtime);
    hermesApi->hermes_get_node_api_env(runtime, &env);

    return makeNodeApiJsiRuntime(env, hermesApi, [runtime]() { HermesApi::current()->hermes_delete_runtime(runtime); });
  })};
}
} // namespace facebook::jsi
