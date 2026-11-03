// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include <ApiLoaders/HermesApi.h>
#include <ApiLoaders/V8Api.h>
#include <NodeApiJsiRuntime.h>
#include <jsi/test/testlib.h>
#include <string>
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

#if JSI_VERSION >= 8
namespace facebook::jsi {

class NodeApiJsiRuntimeTest : public JSITestBase {};

// BigInt::toString must match the engine's BigInt.prototype.toString for values that span several 64-bit words.
TEST_P(NodeApiJsiRuntimeTest, BigIntToStringMultiWord) {
  const char *values[] = {
      "0x10000000000000000n",
      "0x100000000000000000000000n",
      "0xffffffffffffffffffffffffffffffffn",
      "0x100000000000000000000000000000000n",
      "10n ** 40n",
      "36n ** 30n",
      "0x123456789abcdef0fedcba98765432100123456789abcdefn",
      "(1n << 512n) - 1n",
      "(1n << 1000n) + 12345n",
      "0xdeadbeef00000000000000000000000000000000cafebaben",
  };

  for (const char *value : values) {
    for (const char *sign : {"", "-"}) {
      std::string expr = std::string{sign} + "(" + value + ")";
      BigInt bigint = eval(expr.c_str()).asBigInt(rt);
      Function toString = eval("(function(v, r) { return v.toString(r); })").getObject(rt).getFunction(rt);
      for (int radix = 2; radix <= 36; ++radix) {
        std::string expected = toString.call(rt, bigint, radix).getString(rt).utf8(rt);
        EXPECT_EQ(bigint.toString(rt, radix).utf8(rt), expected) << expr << " radix " << radix;
      }
    }
  }
}

INSTANTIATE_TEST_SUITE_P(Runtimes, NodeApiJsiRuntimeTest, ::testing::ValuesIn(runtimeGenerators()));

} // namespace facebook::jsi
#endif
