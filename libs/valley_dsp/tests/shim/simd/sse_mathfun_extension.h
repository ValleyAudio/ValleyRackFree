#pragma once

#if defined(__SSE2__) || defined(_M_AMD64) || defined(_M_X64)
#include <emmintrin.h>
inline __m128 sse_mathfun_exp_ps(__m128 x) { return x; }
inline __m128 sse_mathfun_log_ps(__m128 x) { return x; }
#elif defined(__arm__) || defined(__aarch64__)
#include "simde/x86/sse2.h"
inline simde__m128 sse_mathfun_exp_ps(simde__m128 x) { return x; }
inline simde__m128 sse_mathfun_log_ps(simde__m128 x) { return x; }
#endif
