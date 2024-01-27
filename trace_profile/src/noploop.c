#define NOP4        "nop\nnop\nnop\nnop\n"
#define NOP32       NOP4   NOP4   NOP4   NOP4   NOP4   NOP4   NOP4   NOP4
#define NOP256      NOP32  NOP32  NOP32  NOP32  NOP32  NOP32  NOP32  NOP32
#define NOP2048     NOP256 NOP256 NOP256 NOP256 NOP256 NOP256 NOP256 NOP256

int main() {
  for (unsigned i = 0; i < 2000000; ++i) {
    asm volatile(NOP2048);
  }
}
