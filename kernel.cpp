#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

// defining memory variables
#define NOMEM 0xFFFF
uint8_t loopbackbuff =
    64; // very minimal to try to maintain speed but reliable comms
uint8_t dychpmem[128] = {0}; // declares all slots in ring 1 are empty
// defining functions
void gettel();
void clock();
void reslot(uint16_t slot);
void freeslot(uint16_t slot);
uint16_t virtualmmu(uint16_t slot);
uint16_t getfreemem();
uint16_t kernelpanic();
uint8_t fillslot(uint16_t slot, uint8_t byte);

// main thing
int main() {
  uint16_t avaslot = getfreemem();
  if (avaslot == NOMEM) {
    kernelpanic();
    return 0;
  }
  reslot(avaslot);
  uint16_t vmtr = virtualmmu(avaslot);
}

void reslot(uint16_t slot) {
  dychpmem[slot] = 1;
} // this marks a memory slot as filled

void freeslot(uint16_t slot) { dychpmem[slot] = 0; } // marks a slot as empty

uint16_t getfreemem() {
  for (uint8_t i = 0; i < 128; i++) {
    if (dychpmem[i] == 0) {
      return static_cast<uint16_t>(i);
    }
  }
  return NOMEM;
}

uint16_t virtualmmu(uint16_t slot) {
  return NOMEM; // placeholder till i implement vmmu logic
}

uint8_t fillslot(uint16_t slot, uint8_t byte) { return byte; }

uint16_t kernelpanic() { return NOMEM; }

// notes because the avr architecture isnt what i thought it is
// kernel/bitmaps 0x0100-0x02FF
// managed dynamic heaps 0x0300-0x06FF
// hardware call stack 0x0700-0x08FF

// ring roles
// ring 0, kernel
// ring 1, dynamic heap
// ring 2, filesystem
// ring 3, userspace
