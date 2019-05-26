// Copyright (c) 2019 Dale Johnson
// Vectorised versions of the included Rack ODE functions

template<typename F>
void vecStepEuler(float y, float dt, __m128 x[], int len, F f) {
    __m128 k[len];

    f(t, x, k);
    for(int i = 0; i < len; ++i) {
        x[i] = _mm_add_ps(x[i], _mm_mul_ps(_mm_set1_ps(dt), k[i]));
    }
}
