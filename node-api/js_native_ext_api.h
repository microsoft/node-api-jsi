// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#ifndef SRC_JS_NATIVE_EXT_API_H_
#define SRC_JS_NATIVE_EXT_API_H_

#include "js_native_api.h"

//
// N-API extensions required for JavaScript engine hosting.
//
// It is a very early version of the APIs which we consider to be experimental.
// These APIs are not stable yet and are subject to change while we continue
// their development. After some time we will stabilize the APIs and make them
// "officially stable".
//

#define NAPI_API NAPI_EXTERN napi_status NAPI_CDECL

EXTERN_C_START

// Provides a hint to run garbage collection.
// It is typically used for unit tests.
NAPI_API napi_ext_collect_garbage(napi_env env);

// Checks if the environment has an unhandled promise rejection.
NAPI_API napi_ext_has_unhandled_promise_rejection(napi_env env, bool *result);

// Gets and clears the last unhandled promise rejection.
NAPI_EXTERN
napi_status NAPI_CDECL napi_get_and_clear_last_unhandled_promise_rejection(
    napi_env env,
    napi_value *result);

// To implement JSI description()
NAPI_API napi_ext_get_description(
    napi_env env,
    char *buf,
    size_t bufsize,
    size_t *result);

// To implement JSI drainMicrotasks()
NAPI_API
napi_ext_drain_microtasks(napi_env env, int32_t max_count_hint, bool *result);

// To implement JSI isInspectable()
NAPI_API napi_ext_is_inspectable(napi_env env, bool *result);

//=============================================================================
// Script preparing and running.
//
// Script is usually converted to byte code, or in other words - prepared - for
// execution. Then, we can run the prepared script.
//=============================================================================

typedef struct napi_ext_prepared_script_s *napi_ext_prepared_script;

// Run script with source URL.
NAPI_API napi_ext_run_script(
    napi_env env,
    napi_value source,
    const char *source_url,
    napi_value *result);

// Prepare the script for running.
NAPI_API napi_ext_create_prepared_script(
    napi_env env,
    uint8_t *script_data,
    size_t script_length,
    napi_finalize finalize_cb,
    void *finalize_hint,
    const char *source_url,
    napi_ext_prepared_script *result);

// Delete the prepared script.
NAPI_API napi_ext_delete_prepared_script(
    napi_env env,
    napi_ext_prepared_script prepared_script);

// Run the prepared script.
NAPI_API napi_ext_prepared_script_run(
    napi_env env,
    napi_ext_prepared_script prepared_script,
    napi_value *result);

EXTERN_C_END

#endif // !SRC_JS_NATIVE_EXT_API_H_
