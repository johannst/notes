#include <stdint.h>
#include <stdio.h>

typedef struct {
  uint32_t eax, ebx, ecx, edx;
} cpuid_t;

// in: eax is the cpuid leaf number
static inline cpuid_t cpuid(uint32_t eax) {
  cpuid_t r;
  asm volatile("cpuid"
               : "=a"(r.eax), "=b"(r.ebx), "=c"(r.ecx), "=d"(r.edx)
               : "a"(eax), "c"(0));
  return r;
};

// in: eax is the cpuid leaf number
// in: ecx is the cpuid sub-leaf number
static inline cpuid_t cpuid_subleaf(uint32_t eax, uint32_t ecx) {
  cpuid_t r;
  asm volatile("cpuid"
               : "=a"(r.eax), "=b"(r.ebx), "=c"(r.ecx), "=d"(r.edx)
               : "a"(eax), "c"(ecx));
  return r;
};

int main() {
  cpuid_t r;
  uint32_t leafs = 1;

  for (uint32_t l = 0; l < leafs; ++l) {
    r = cpuid(l);
    if (l == 0) {
      // leaf 0 returns the highest available cpuid leaf in eax.
      leafs = r.eax;

      // ebx, edx, ecx contain the vendor string
      // clang-format off
      printf("vendor: %c%c%c%c%c%c%c%c%c%c%c%c\n",
             (r.ebx & 0xFF), (r.ebx >> 8) & 0xFF, (r.ebx >> 16) & 0xFF, (r.ebx >> 24) & 0xFF,
             (r.edx & 0xFF), (r.edx >> 8) & 0xFF, (r.edx >> 16) & 0xFF, (r.edx >> 24) & 0xFF,
             (r.ecx & 0xFF), (r.ecx >> 8) & 0xFF, (r.ecx >> 16) & 0xFF, (r.ecx >> 24) & 0xFF);
      // clang-format on
    }
    printf("[0x%08x] eax=%08x ebx=%08x ecx=%08x edx=%08x\n", l, r.eax, r.ebx, r.ecx, r.edx);
  }
}
