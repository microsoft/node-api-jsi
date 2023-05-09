// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once
#ifndef SRC_NODEAPIJSIRUNTIME_H_
#define SRC_NODEAPIJSIRUNTIME_H_

#include <jsi/jsi.h>
#include <functional>
#include <memory>
#include "NodeApi.h"
#include "js_native_api.h"

namespace Microsoft::NodeApiJsi {

std::unique_ptr<facebook::jsi::Runtime>
makeNodeApiJsiRuntime(napi_env env, NodeApi *nodeApi, std::function<void()> onDelete) noexcept;

} // namespace Microsoft::NodeApiJsi

#endif // !SRC_NODEAPIJSIRUNTIME_H_
