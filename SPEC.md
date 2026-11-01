# ValleyDSP Overhaul — Specification (`SPEC.md`)

## 1. Objective
Decouple all digital signal processing (DSP) primitives, SIMD utilities, oscillators, filters, and synthesis/effect engines in `ValleyAudio/ValleyRackFree` from the VCV Rack SDK (`rack.hpp`).
Extract them into a standalone, host-agnostic, real-time safe C++17 static/header library (`valley_dsp` under `libs/valley_dsp/`) suitable for VCV Rack, VST3/AU/CLAP plugins, and embedded hardware, while refactoring the existing VCV Rack modules under `src/` into thin UI and I/O wrappers.

---

## 2. Phase Progress Tracker
*(Agent Instruction: Update the status checkboxes below from `[ ]` to `[x]` at the end of each phase before committing).*

- [x] **Phase 0:** CMake build system, SIMDe vendoring, and Audio Reference Catch2 regression harness (`tests/reference/*.bin`).
- [ ] **Phase 1:** Core headers, portable `Vec4f`/`Vec4i` SIMD wrapper, math utilities, delays (`InterpDelay`, `AllpassFilter`), and filters (`OnePoleFilters`, `OTAFilter`).
- [ ] **Phase 2:** Oscillators, LFOs, Wavetable views (RT-safe), and FM `OPCore`.
- [ ] **Phase 3A:** Standalone Engines & Rack Adapters for `Plateau`, `Amalgam`, and `Feline`.
- [ ] **Phase 3B:** Standalone Engines & Rack Adapters for `Interzone`, `Dexter`, and `Terrorform`.
- [ ] **Phase 4:** GPLv3 isolation (`Topograph`/`uGraph`), ASan/UBSan sanitizer verification, single-precision float audit, and final `make dist` Rack verification.

---

## 3. Non-Negotiable Architectural Laws

### Law 1: Zero VCV Rack Contamination in `libs/valley_dsp/`
No header or source file inside `libs/valley_dsp/` may `#include <rack.hpp>`, `<nanovg.h>`, `<jansson.h>`, or reference the `rack::` namespace.

### Law 2: Real-Time Audio Thread Safety
- Zero heap allocations (`new`, `delete`, `malloc`, `std::vector::resize`, `std::vector::push_back`, `std::string`) inside `processSample()` or `processBlock()`.
- All dynamic memory allocation (such as delay line buffers) must occur strictly inside `prepare(double sampleRate, size_t maxBlockSize)` or non-RT setup methods.
- All audio processing methods must be marked `noexcept`.

### Law 3: Explicit Zero-Initialization of All State
- Every scalar, array, and SIMD member variable in every DSP class/struct MUST have an explicit C++11 default member initializer (e.g., `float _z = 0.0f;`, `Vec4f _phase{};`) so cold-starting a module on any OS/architecture (especially macOS ARM64) never reads uninitialized stack/heap memory or propagates `NaN` into recursive feedback loops.

### Law 4: Portable SIMD via `valley::simd::Vec4f` and `Vec4i`
- Do NOT use raw `__m128` or `__m128i` types or GCC-specific vector arithmetic operators (`__m128 a = b * c;`) in DSP code, as they fail on MSVC (`cl.exe`).
- Do NOT define `SIMDE_ENABLE_NATIVE_ALIASES` in public headers.
- All 4-lane SIMD operations must go through `valley::simd::Vec4f` and `valley::simd::Vec4i` (located in `libs/valley_dsp/include/valley/simd/Vec4f.hpp`), backed by header-only SIMDe (`simde_mm_*`).

### Law 5: Sonic Preservation (The Null-Test Gate)
- Any refactored primitive or engine must match its pre-refactor reference output snapshot in `tests/reference/` within a maximum absolute error of `1e-5f` (`-100 dB`), unless an explicit bug fix documented in Section 6 changes broken behavior (e.g., high sample-rate buffer wrap or uninitialized memory).

### Law 6: Licensing Separation
- `Topograph` and `uGraph` are ports of Mutable Instruments Grids (GPLv3). Keep their pattern-generation code separate from `libs/valley_dsp/` (either in `libs/valley_dsp_grids/` or inside `src/Topograph/`) so `libs/valley_dsp/` remains 100% original work by Dale Johnson.

---

## 4. Target Directory Layout

```text
ValleyRackFree/
├── SPEC.md
├── CMakeLists.txt                  # Standalone build for valley_dsp + Catch2 tests
├── Makefile                        # VCV Rack plugin build (compiles src/ + libs/valley_dsp/src/)
├── libs/
│   ├── valley_dsp/
│   │   ├── third_party/
│   │   │   └── simde/              # Header-only x86 SSE->NEON SIMDe headers
│   │   ├── include/valley/
│   │   │   ├── core/
│   │   │   │   ├── Config.hpp          # VALLEY_FORCE_INLINE, compiler macros
│   │   │   │   ├── Denormals.hpp       # ScopedNoDenormals (FTZ/DAZ)
│   │   │   │   ├── EurorackScale.hpp   # +/-5V & 1V/Oct <-> normalized [-1, +1] helpers
│   │   │   │   └── ParameterSpec.hpp   # Declarative min/max/default/skew parameter metadata
│   │   │   ├── simd/
│   │   │   │   ├── Vec4f.hpp           # Vec4f and Vec4i wrapper structs over SIMDe
│   │   │   │   └── VecMath.hpp         # Ported SIMDUtilities (fastSin, fastCos, fastTanh, fastExp2, linterp)
│   │   │   ├── math/
│   │   │   │   ├── Constants.hpp       # Single-precision kPi, kTwoPi, etc.
│   │   │   │   └── Utilities.hpp       # Scalar clamp, rescale, linterp, pitch-to-freq
│   │   │   ├── delays/
│   │   │   │   ├── InterpDelay.hpp     # Dynamic power-of-two ring buffer sized in prepare()
│   │   │   │   ├── AllpassFilter.hpp   # Single & nested allpass filters
│   │   │   │   └── TunedDelay.hpp
│   │   │   ├── filters/
│   │   │   │   ├── OnePoleFilters.hpp  # OnePoleLPFilter, OnePoleHPFilter, DCBlocker
│   │   │   │   ├── OTAFilter.hpp
│   │   │   │   ├── VecLPG.hpp
│   │   │   │   ├── VecOnePoleFilters.hpp
│   │   │   │   └── VecOTAFilter.hpp
│   │   │   ├── oscillators/
│   │   │   │   ├── DOsc.hpp
│   │   │   │   ├── LFO.hpp
│   │   │   │   ├── WavetableView.hpp   # Non-owning RT-safe view over wavetable frames
│   │   │   │   └── OPCore.hpp
│   │   │   ├── engines/
│   │   │   │   ├── PlateauEngine.hpp
│   │   │   │   ├── AmalgamEngine.hpp
│   │   │   │   ├── FelineEngine.hpp
│   │   │   │   ├── InterzoneEngine.hpp
│   │   │   │   ├── DexterEngine.hpp
│   │   │   │   └── TerrorformEngine.hpp
│   │   │   └── src/                    # Non-template .cpp implementations for valley_dsp
│   │   └── tests/
│   │       ├── CMakeLists.txt
│   │       ├── reference/              # Binary float snapshots generated in Phase 0
│   │       ├── AudioReferenceGenerator.cpp
│   │       ├── test_audio_reference.cpp
│   │       ├── test_simd_math.cpp
│   │       ├── test_delays_filters.cpp
│   │       ├── test_oscillators.cpp
│   │       └── test_engines.cpp
│   └── valley_dsp_grids/           # Optional isolated GPLv3 Grids/Topograph logic
├── src/                            # VCV Rack Modules, Widgets, and Panel UI ONLY
└── Makefile                        # VCV Rack plugin build (compiles src/ + libs/valley_dsp/src/)
```

---

## 5. Standard Engine API Pattern

Every module engine in `libs/valley_dsp/include/valley/engines/` must follow this lifecycle contract so it can be embedded in both per-sample hosts (VCV Rack) and block-based DAW plugins (VST3/CLAP/AU):

```cpp
namespace valley::engines {

class ExampleEngine {
public:
    struct Params {
        // Normalized or physical parameter values with sensible defaults
    };

    ExampleEngine() noexcept = default;

    // Called on init or sample-rate change (allocates delay buffers if needed)
    void prepare(double sampleRate, size_t maxBlockSize = 512);

    // Clears all internal delay lines, filter states, and phase accumulators to 0.0f
    void reset() noexcept;

    // Updates parameter targets (smoothed internally)
    void setParams(const Params& params) noexcept;

    // Single-sample execution path for VCV Rack
    void processSample(const float* inputs, float* outputs) noexcept;

    // Block execution path for VST3 / CLAP / AU
    void processBlock(const float* const* inputs, float* const* outputs, size_t numSamples) noexcept;
};

} // namespace valley::engines
```

---

## 6. Known Bugs & Technical Debt to Fix During Extraction

1. **Uninitialized State / NaN on Cold Boot:**
   - Ensure every member variable in `DOsc`, `OnePoleLPFilter`, `OnePoleHPFilter`, `StateVariableFilter`, `Dattorro`, `InterpDelay`, `AllpassFilter`, `VecAmalgam`, and `FourVoiceDexterCore` has an inline default initializer (`= 0.0f` or `{}`).
2. **High Sample Rate Delay Buffer Wrap (`InterpDelay` & `Dattorro`):**
   - `Dattorro` scales reference tap lengths (`dattorroSampleRate = 29761.0f`) by `sampleRate / 29761.0f * size`. Fixed buffer sizes overflow at `96 kHz` / `192 kHz`.
   - `InterpDelay::prepare(double maxDelaySeconds, double sampleRate)` must dynamically compute the required power-of-two capacity (`nextPowerOfTwo(ceil(maxDelaySeconds * sampleRate) + 4)`), store `_mask = _capacity - 1`, and wrap all read/write indices with `& _mask`.
3. **Double-to-Float Promotion in Inner Loops:**
   - Replace `sin()`, `cos()`, `pow()`, `exp()`, `tanh()`, and `M_PI` with `std::sin(float)`, `std::pow(float, float)`, and `valley::math::kPi` (`3.14159265358979323846f`), and append `f` to all floating-point literals in DSP equations.
4. **Dexter CV & Algorithm Scaling Bug:**
   - Verify that V/Oct and modulator CV attenuverter inputs scale across the full `[-1.0f, +1.0f]` range (fixing the `0.1f` scaling regression) and that the Algorithm CV input addresses all 23 algorithms (`0` through `22`).
5. **Wavetable File I/O Separation (`Terrorform` / `Dexter`):**
   - Keep `AudioFile.h` and disk reading out of the real-time oscillator classes. Pass wavetable data into the DSP engine via a lightweight `WavetableView` struct (`const float* data`, `size_t frameSize`, `size_t numFrames`).

---

## 7. Build & Verification Commands
- **Configure & Build Standalone DSP + Tests:**
  `cmake -B build -DCMAKE_BUILD_TYPE=Debug -DVALLEY_ENABLE_SANITIZERS=ON && cmake --build build -j`
- **Run Unit & Audio Reference Regression Tests:**
  `ctest --test-dir build --output-on-failure`
- **Build VCV Rack Plugin (when `RACK_DIR` is set):**
  `make -j`
