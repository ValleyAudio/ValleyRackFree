#ifndef VALLEY_SSE_INCLUDE
#define VALLEY_SSE_INCLUDE

#if defined(__SSE2__) || defined(_M_AMD64) || defined(_M_X64) ||                                   \
    (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
#include <emmintrin.h>
#include <pmmintrin.h>
#else
#if defined(__arm__) || defined(__aarch64__) || defined(__riscv)
#define SIMDE_ENABLE_NATIVE_ALIASES
#include "simde/x86/sse2.h"
#else
#error Valley requires either X86/SSE2 or ARM architectures.
#endif
#endif

#endif

