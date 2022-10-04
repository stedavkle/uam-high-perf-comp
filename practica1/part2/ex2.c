#include <xmmintrin.h>
#include <stdio.h>
#include <x86intrin.h>

int main(void) {
    __builtin_cpu_init();
    printf("%d\n", __builtin_cpu_supports ("sse"));
    printf("%d\n", __builtin_cpu_supports ("avx"));

    double a[100], b[100], c[100];
    // iterate in blocks of 4,
    // because that's how many doubles can fit into a 256-bit register
    for (int i = 0; i < 100; i += 4) {
    // load two 256-bit segments into registers
}
