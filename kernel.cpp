#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

// defining memory variables
#define NOMEM 0xFFFF
uint8_t freemem[256] = {false}; // declares all slots are empty on startup
bool kernelpanic = false;

// defining functions
void gettel();
void clock();
void reslot(uint16_t slot);
void freeslot(uint16_t slot);
uint16_t virtualmmu(uint16_t slot);
uint16_t getfreemem();

// main thing
int main() {
  uint16_t avaslot = getfreemem();
  if (avaslot != NOMEM) {
    uint16_t vmtr = virtualmmu(avaslot);
    reslot(avaslot);

    if (avaslot == NOMEM) {
      kernelpanic = true;
    }
  }
}

void reslot(uint16_t slot) {
  freemem[slot] = true;
} // this marks a memory slot as filled

void freeslot(uint16_t slot) { freemem[slot] = false; } // marks a slot as empty

uint16_t getfreemem() {
  for (uint16_t i = 0; i < 2048; i++) {
    if (!freemem[i]) {
      return i;
    }
  }
  return NOMEM;
}

uint16_t virtualmmu(uint16_t slot) {
  return NOMEM; // placeholder till i implement vmmu logic
}

// notes because the avr architecture isnt what i thought it is
// kernel/bitmaps 0x0100-0x02FF
// managed dynamic heaps 0x0300-0x06FF
// hardware call stack 0x0700-0x08FF

// ring roles
// ring 0, kernel
// ring 1, dynamic heap
// ring 2, filesystem
// ring 3, userspace
