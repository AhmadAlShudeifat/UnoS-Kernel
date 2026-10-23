#include "kernel.h"
#include <EEPROM.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

// defining memory variables
#define NOADDR 0xFFFF
#define HEAPSTART 0x0300
#define SLOTSIZE 8
#define MAXPROC 254
#define NOPID 0xFF
#define NOMEM 0x0
uint8_t dychpmem[128] = {0}; // declares all slots in ring 1 are empty

// defining cpu scheduling variables
volatile uint32_t systicks = 0;
ISR(TIMER1_COMPA_vect) { systicks++; }

// defining dynamic heap map
struct Process {
  uint8_t pid;
  uintptr_t slot;
  int8_t procount;
  bool running;
};

Process proctable[MAXPROC];

// defining functions
void gettel();
void clock();
void reslot(uint16_t slot, uint8_t index);
void freeslot(uint16_t slot);
void killproc(uint8_t slot, uint8_t index);
uintptr_t virtualmmu(uint8_t slot, int offset);
uint16_t getfreemem();
uint16_t kernelpanic();
uint8_t getrq();
uint8_t fillslot(uintptr_t slot, uint8_t byte);
uint8_t readslot(uintptr_t slot);
int8_t spawnproc(uintptr_t slot, uint8_t pid);

// initializer and step
void kerninit() {
  clock();
  for (uint8_t i = 0; i < 128; i++) {
    dychpmem[i] = 0;
  }
  for (uint8_t slot = 0; slot < 128; slot++) {
    for (uint8_t offset = 0; offset < 8; offset++) {
      fillslot(slot, NOPID);
    }
  }
  for (uint8_t i = 0; i < MAXPROC; i++) {
  }
}

void kernstep() { ; }

int8_t spawnproc(uintptr_t slot, uint8_t pid, int8_t procount) {
  for (uint8_t i = 0; i < MAXPROC; i++) {
    if (proctable[i].pid == NOPID) {
      proctable[i].pid = pid;
      proctable[i].slot = slot;
      proctable[i].running = true;
      proctable[i].procount = procount;
      reslot(slot, i);
      return i;
    }
  }
  return -1;
}

void killproc(uint8_t slot, uint8_t index) {
  proctable[index].pid = NOPID;
  proctable[index].slot = 0;
  proctable[index].running = false;
  proctable[index].procount = 0xFF;
  freeslot(slot);
}

void reslot(uint16_t slot, uint8_t index) {
  dychpmem[index] = 1;
  fillslot(slot, NOPID);
} // this marks a memory slot as filled

void freeslot(uint16_t slot, uint8_t indexslot) {
  dychpmem[indexslot] = 0;
  fillslot(slot, NOMEM);
} // lets a slot be overwritten

uint16_t getfreemem() {
  for (uint8_t i = 0; i < 128; i++) {
    if (dychpmem[i] == 0) {
    }
  }
  return NOADDR;
}

uint8_t getrq() { return 1; }

void clock() {
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 249;
  TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
  sei();
}

void csi() {
  uint16_t csp = SP;
  if (csp <= 0x06FF) {
    kernelpanic();
  }
}

uintptr_t virtualmmu(uint8_t slot, int offset) {
  uint16_t byteslot =
      HEAPSTART + (slot * SLOTSIZE); // converts slot to real address
  return byteslot + offset;
}

uint8_t fillslot(uintptr_t slot, uint8_t byte) {
  *(volatile uint8_t *)slot = byte;
  return byte;
}

uint8_t readslot(uintptr_t slot) { return *(volatile uint8_t *)slot; }

uint16_t kernelpanic() {
  for (uint8_t indexslot = 0; indexslot < 128; indexslot++) {
    for (uint8_t offset = 0; offset < 8; offset++) {
      for (uint16_t eprmaddr = 0; eprmaddr < 1024; eprmaddr++) {
        uint8_t value = EEPROM.read(eprmaddr);
        if (value == NOPID) {
          EEPROM.update(eprmaddr, readslot(virtualmmu(indexslot, offset)));
        }
      }
    }
  }
  resetFunc();
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
