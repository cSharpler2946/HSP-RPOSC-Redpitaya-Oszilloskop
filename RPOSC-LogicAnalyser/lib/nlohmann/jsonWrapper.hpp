#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>

#ifdef __cplusplus
}
#endif

#define JSON_ASSERT(x) assert(x)
#include "json.hpp"