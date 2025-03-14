#include <stdint.h>
#include <stdio.h>

// Reads GS segment base address (backed by IA32_GS_BASE msr).
//
// Instruction must be supported. Can be checked as
//   cpuid(0x7).ebx & 1 == 1
//
// In case the instruction is not available, the base address can be
// read with the following syscall.
//   uint64_t addr;
//   arch_prctl(ARCH_GET_GS, &addr)
static inline uint64_t rdgsbase() {
  uint64_t gs;
  asm volatile("rdgsbase %0" : "=r"(gs));
  return gs;
}

// Write GS segment base address (backed by IA32_GS_BASE msr).
//
// Instruction must be supported. Can be checked as
//   cpuid(0x7).ebx & 1 == 1
//
// In case the instruction is not available, the base address can be
// set with the following syscall.
//   arch_prctl(ARCH_GET_GS, addr)
static inline void wrgsbase(uint64_t* gs) {
  asm volatile("wrgsbase %0" ::"r"(gs));
}

// Read u32 from off relative to GS segment.
static inline uint32_t rdgs32(int64_t off) {
  uint32_t ret;
  asm volatile("mov %%gs:%1, %0" : "=r"(ret) : "m"(*(uint64_t*)off));
  return ret;
}

// Read u64 from off relative to GS segment.
static inline uint64_t rdgs64(int64_t off) {
  uint64_t ret;
  asm volatile("mov %%gs:%1, %0" : "=r"(ret) : "m"(*(uint64_t*)off));
  return ret;
}

// Write u32 to off relative to GS segment.
static inline void wrgs32(int64_t off, uint32_t val) {
  asm volatile("mov %1, %%gs:%0" ::"m"(*(uint64_t*)off), "r"(val));
}

int main() {
  printf("gsbase=%016lx\n", rdgsbase());

  uint64_t VALS[] = {
      0xa7a6a5a4a3a2a1a0,
      0xb7b6b5b4b3b2b1b0,
  };

  // Setup GS segment base address.
  wrgsbase(VALS);
  printf("gsbase=%016lx | &VALS=%p\n", rdgsbase(), VALS);

  // Read values relative to GS segment.
  int64_t off[] = {0, 8, 4, 1};
  for (int64_t i = 0; i < 4; ++i) {
    printf("64: gs:%ld=%016lx\n", off[i], rdgs64(off[i]));
    printf("32: gs:%ld=%016lx\n", off[i], rdgs32(off[i]));
  }

  // Write value relative to GS segment.
  wrgs32(4, 0xddccbbaa);
  printf("64: gs:%ld=%016lx\n", 0, rdgs64(0));

  // Reset GS segment base address.
  wrgsbase(0);
}
