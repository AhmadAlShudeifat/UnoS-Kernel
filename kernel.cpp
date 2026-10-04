#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

// defining memory variables
#define NOMEM 0xFFFF
bool freemem[2048] = {false}; // declares all slots are empty on startup

// defining functions
void gettel();
void clock();
void reslot();
void freeslot();
uint16_t virtualmmu();
uint16_t getfreemem();

// main thing
int main() { return 0; }

uint16_t virtualmmu(bool mem) { return NOMEM; }

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

// notes because the avr architecture isnt what i thought it is
// kernel/bitmaps 0x0100-0x02FF
// managed dynamic heaps 0x0300-0x06FF
// hardware call stack 0x0700-0x08FF
