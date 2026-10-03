#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

// defining functions
void gettel();
void clock();
void virtualmmu();

// main thing
int main() { return 0; }

void virtualmmu() {
  bool freemem[2048] = {false};
  int group1 = 512;
  int group2 = 512;
  int group3 = 512;
  int group4 = 512;
}
