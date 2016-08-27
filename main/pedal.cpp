#include "Pedal.h"

struct Pedal
{
    int _pinA;
    int _pinB;
    int _pinMax;

    volatile int _pos;
};

static const int kDeadzone = 3;

static const uint8_t kMaxPedals = 2;

static uint8_t _pedalCount = 0;
static Pedal _pedals[kMaxPedals];


void Decode(Pedal& pedal)
{
    if (digitalRead(pedal._pinA) == digitalRead(pedal._pinB))
    {
        pedal._pos++;
        static const int kAbsoluteMax = (kPedalMax + (kDeadzone * 2));
        if (pedal._pos >= kAbsoluteMax)
        {
            pedal._pos = kAbsoluteMax;
        }
    }
    else
    {
        pedal._pos--;
        if (pedal._pos < 0)
        {
            pedal._pos = 0;
        }
    }
}

typedef int (*Interrupt)(void);

static void Interrupt0(void)
{
    Decode(_pedals[0]);
}
static void Interrupt1(void)
{
    Decode(_pedals[1]);
}

Interrupt GetInterrupt(int index)
{
    switch (index)
    {
        case 0:
            return Interrupt0;
            break;
        case 1:
            return Interrupt1;
            break;
    }

    return NULL;
}

uint8_t CreatePedal(int pinA, int pinB, int pinMax)
{
    if (_pedalCount < kMaxPedals)
    {
        Pedal& pedal = _pedals[_pedalCount];
        
        pedal._pinA = pinA;
        pedal._pinB = pinB;
        pedal._pinMax = pinMax;

        pinMode (pedal._pinA, INPUT);
        pinMode (pedal._pinB, INPUT);
        pinMode (pedal._pinMax, INPUT);

        attachInterrupt(digitalPinToInterrupt(pedal._pinA), GetInterrupt(_pedalCount), CHANGE);

        return _pedalCount++;
    }
    else
    {
        Serial.print("Exceeded maximum number of pedals: ");
        Serial.println(kMaxPedals);
        return -1;
    }
}

int ReadPedal(uint8_t pedalIndex)
{
    if(pedalIndex < _pedalCount)
    {
        int value = _pedals[pedalIndex]._pos;
        if (value < kDeadzone)
        {
            return 0;
        }
        else if (value > (kPedalMax + kDeadzone))
        {
            return kPedalMax;
        }
        else
        {
            return value - kDeadzone;
        }
    }

    Serial.print("Pedal index oob: ");
    Serial.println(pedalIndex);

    return 0;
}

