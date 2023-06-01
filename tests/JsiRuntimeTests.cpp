// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include <ApiLoaders/HermesApi.h>
#include <ApiLoaders/V8Api.h>
#include <NodeApiJsiRuntime.h>
#include <jsi/test/testlib.h>
#include <vector>

using namespace Microsoft::NodeApiJsi;

namespace facebook::jsi {

std::vector<RuntimeFactory> runtimeGenerators() {
  return {
      RuntimeFactory([]() {
        HermesApi *hermesApi = HermesApi::fromLib();
        HermesApi::setCurrent(hermesApi);

        jsr_config config{};
        jsr_runtime runtime{};
        napi_env env{};
        hermesApi->jsr_create_config(&config);
        hermesApi->jsr_config_enable_gc_api(config, true);
        hermesApi->jsr_create_runtime(config, &runtime);
        hermesApi->jsr_delete_config(config);
        hermesApi->jsr_runtime_get_node_api_env(runtime, &env);

        NodeApiEnvScope envScope{env};

        return makeNodeApiJsiRuntime(
            env, hermesApi, [runtime]() { HermesApi::current()->jsr_delete_runtime(runtime); });
      }),
      RuntimeFactory([]() {
        V8Api *v8Api = V8Api::fromLib();
        V8Api::setCurrent(v8Api);

        jsr_config config{};
        jsr_runtime runtime{};
        napi_env env{};
        v8Api->jsr_create_config(&config);
        v8Api->jsr_config_enable_gc_api(config, true);
        v8Api->jsr_create_runtime(config, &runtime);
        v8Api->jsr_delete_config(config);
        v8Api->jsr_runtime_get_node_api_env(runtime, &env);

        NodeApiEnvScope envScope{env};

        return makeNodeApiJsiRuntime(env, v8Api, [runtime]() { V8Api::current()->jsr_delete_runtime(runtime); });
      })};
}

} // namespace facebook::jsi
