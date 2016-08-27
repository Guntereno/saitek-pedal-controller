#include <Arduino.h>

static const int kPedalMax = 63;

uint8_t CreatePedal(int pinA, int pinB, int pinMax);
int ReadPedal(uint8_t pedalIndex);

