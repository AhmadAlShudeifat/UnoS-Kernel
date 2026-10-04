#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

// defining memory variables
#define NOMEM 0xFFFF
bool freemem[2048] = {false};

// defining functions
void gettel();
void clock();
uint16_t virtualmmu();
uint16_t getfreemem();

// main thing
int main() { return 0; }

uint16_t virtualmmu(bool mem) { return NOMEM; }

uint16_t getfreemem() {
  for (uint16_t i = 0; i < 2048; i++) {
  }
}
