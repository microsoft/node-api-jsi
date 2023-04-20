// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once
#ifndef SRC_HERMESAPI_H_
#define SRC_HERMESAPI_H_

#include <napi/hermes_api.h>
#include "NodeApi.h"

namespace Microsoft::NodeApiJsi {

class HermesApi : public NodeApi {
 public:
  HermesApi(IFuncResolver *funcResolver);

  static HermesApi *current() noexcept {
    return current_;
  }

  static void setCurrent(HermesApi *current) noexcept {
    NodeApi::setCurrent(current);
    current_ = current;
  }

  static HermesApi *fromLib();

  class Scope : public NodeApi::Scope {
   public:
    Scope() : Scope(HermesApi::fromLib()) {}

    Scope(HermesApi *hermesApi) : NodeApi::Scope(hermesApi), prevHermesApi_(HermesApi::current_) {
      HermesApi::current_ = hermesApi;
    }

    ~Scope() {
      HermesApi::current_ = prevHermesApi_;
    }

   private:
    HermesApi *prevHermesApi_;
  };

#define HERMES_FUNC(func) decltype(::func) *const func;
#include "HermesFunctions.inc"

 private:
  static thread_local HermesApi *current_;
};

} // namespace Microsoft::NodeApiJsi

#endif // !SRC_HERMESAPI_H_
