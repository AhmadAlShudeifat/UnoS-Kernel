#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

// defining memory variables
#define NOMEM 0xFFFF
#define HEAPSTART 0x0300
#define SLOTSIZE 8
#define MAXPROC 40
#define NOPID 0xFF
uint8_t dychpmem[128] = {0}; // declares all slots in ring 1 are empty

// defining dynamic heap map
struct Process {
  uint8_t pid;
  uint8_t slot;
  uint8_t offset;
};

Process proctable[MAXPROC];

// defining functions
void gettel();
void clock();
void reslot(uint16_t slot);
void freeslot(uint16_t slot);
void killproc(uint8_t slot, uint8_t index);
uintptr_t virtualmmu(uint16_t slot);
uint16_t getfreemem();
uint16_t kernelpanic();
uint8_t fillslot(uint16_t slot, uint8_t byte, uint8_t offset);
uint8_t readslot(uint16_t slot, uint8_t offset);
int8_t spawnproc(uint8_t slot, uint8_t offset, uint8_t pid);

// initializer and step
void kerninit() {
  for (uint8_t i = 0; i < 128; i++) {
    dychpmem[i] = 0;
  }
  for (uint8_t i = 0; i < MAXPROC; i++) {
    proctable[i].pid = NOPID;
  }
}

void kernstep() { ; }

int8_t spawnproc(uint8_t slot, uint8_t offset, uint8_t pid) {
  for (uint8_t i = 0; i < MAXPROC; i++) {
    if (proctable[i].pid == NOPID) {
      proctable[i].pid = pid;
      proctable[i].slot = slot;
      proctable[i].offset = offset;
      reslot(slot);
      return i;
    }
  }
  return -1;
}

void killproc(uint8_t slot, uint8_t index) {
  proctable[index].pid = NOPID;
  proctable[index].slot = 0;
  proctable[index].offset = 0;
  freeslot(slot);
}

void reslot(uint16_t slot) {
  dychpmem[slot] = 1;
} // this marks a memory slot as filled

void freeslot(uint16_t slot) {
  dychpmem[slot] = 0;
} // lets a slot be overwritten

uint16_t getfreemem() {
  for (uint8_t i = 0; i < 128; i++) {
    if (dychpmem[i] == 0) {
      return static_cast<uint16_t>(i);
    }
  }
  return NOMEM;
}

uintptr_t virtualmmu(uint16_t slot) {
  return HEAPSTART + (slot * SLOTSIZE); // converts slot to real address
}

uint8_t fillslot(uint16_t slot, uint8_t byte, uint8_t offset) {
  *(volatile uint8_t *)(virtualmmu(slot) + offset) = byte;
  return byte;
}

uint8_t readslot(uint16_t slot, uint8_t offset) {
  return *(volatile uint8_t *)(virtualmmu(slot) + offset);
}

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
