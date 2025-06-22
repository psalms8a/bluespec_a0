#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstdio>

extern "C" uint32_t rand_x() {
    static bool seeded = false;
    if (!seeded) {
        seeded = true;
        srand(1);
    }
    double ln2 = std::log(2.0);
    double r = (double)rand() / RAND_MAX; // 0..1
    double x = (2.0 * r - 1.0) * ln2; // range [-ln2, ln2]
    int32_t fx = (int32_t)llround(x * (double)(1u<<24));
    return (uint32_t)fx;
}

extern "C" uint32_t exp_ref(uint32_t xb) {
    int32_t sx = (int32_t)xb;
    double x = (double)sx / (double)(1u<<24);
    double y = std::exp(x);
    int64_t fy = llround(y * (double)(1u<<24));
    if (fy > 0x7fffffff) fy = 0x7fffffff;
    if (fy < -0x80000000ll) fy = -0x80000000ll;
    return (uint32_t)fy;
}
