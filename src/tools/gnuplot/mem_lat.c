#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum : size_t {
  KB = 1024,
  MB = 1024 * KB,

  DFL_MAXSZ  = 64 * MB,
  DFL_STRIDE = 256,
  NUM_ACCESS = 1ull << 24,
};

static inline uint64_t rdtsc() {
  uint32_t eax, edx;
  asm volatile("rdtsc" : "=a"(eax), "=d"(edx));
  return ((uint64_t)edx << 32) | eax;
}

uint64_t run_one_walk(size_t size, size_t stride) {
  // Check size is power of two.
  assert((size & (size -1)) == 0);
  assert((size / stride) <= NUM_ACCESS);

  unsigned char *mem = (unsigned char *)malloc(size);
  memset(mem, 0x55, size);

  const uint64_t start = rdtsc();
  for (size_t it = 0; it < NUM_ACCESS; it++) {
    ((volatile unsigned char *)mem)[(it * stride) & (size - 1)]++;
  }
  const uint64_t stop = rdtsc();

  free(mem);
  return stop - start;
}

int main(int argc, char *argv[]) {
  size_t max_sz = DFL_MAXSZ;
  size_t stride = DFL_STRIDE;

  if (argc > 1) {
    max_sz = strtoll(argv[1], 0, 10);
    max_sz *= MB;
  }
  if (argc > 2) {
    stride = strtoll(argv[2], 0, 10);
  }

  fprintf(stderr, "max_sz=%zuK stride=%zu access=%zu\n",
          max_sz / KB, stride, NUM_ACCESS);

  size_t size = 2048;
  while (size <= max_sz) {
    uint64_t cycles = 0;
    for (size_t loop = 0; loop < 10; ++loop) {
      cycles += run_one_walk(size, stride);
    }
    printf("%6zuK %f\n", size / KB, (double)cycles / NUM_ACCESS / 10.0);

    size *= 2;
  }

  return 0;
}
