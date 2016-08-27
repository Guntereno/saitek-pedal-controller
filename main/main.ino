#include <Joystick.h>

#include "button.h"
#include "pedal.h"


const int kModeButtonPin = 5;
const int kModeLedPin = 14;
const int kLeftButtonPin = 16;
const int kRightButtonPin = 10;

bool _rudderMode = false;

Button _button;

const int kPedalCount = 2;
uint8_t _pedals[kPedalCount];

void OnModeButtonPressed();


void setup()
{
  Joystick.begin(false);

  _button.Setup(kModeButtonPin, OnModeButtonPressed);
  
  pinMode(kModeLedPin, OUTPUT);

  pinMode(kLeftButtonPin, INPUT);
  pinMode(kRightButtonPin, INPUT);

  _pedals[0] = CreatePedal(0, 1, 16);
  _pedals[1] = CreatePedal(3, 2, 10);

  Serial.begin(9600); // This pipes to the serial monitor
}

void loop()
{
  _button.Loop();

  Joystick.setButton(0, !_button._buttonState);

  int pedalRight = ReadPedal(_pedals[1]);
  int pedalLeft = ReadPedal(_pedals[0]);

  if(_rudderMode)
  {
    sendJoystickRudder(pedalLeft, pedalRight);
  }
  else
  {
    sendJoystickIndividualAxis(pedalLeft, pedalRight);
  }

  Joystick.setButton(0, digitalRead(kRightButtonPin));
  Joystick.setButton(1, digitalRead(kLeftButtonPin));

  Joystick.sendState();
}

void OnModeButtonPressed()
{
  _rudderMode = !_rudderMode;
  digitalWrite(kModeLedPin, _rudderMode);
}

void sendJoystickRudder(int pedalLeft, int pedalRight)
{
  uint8_t pedalsCombined = pedalRight - pedalLeft;
  Joystick.setXAxis(scalePedalOutput(pedalsCombined, 128));

  Joystick.setThrottle(scalePedalOutput(pedalLeft, 0));
  Joystick.setRudder(0);
}

void sendJoystickIndividualAxis(int pedalLeft, int pedalRight)
{
  Joystick.setThrottle(scalePedalOutput(pedalLeft, 255));
  Joystick.setRudder(scalePedalOutput(pedalRight, 255));

  Joystick.setXAxis(0);
}

uint8_t scalePedalOutput(int pedal, int max)
{
  uint8_t output = ((float)pedal / kPedalMax) * max;
  // Serial.println(output);
  return output;
}

