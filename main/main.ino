#include <Joystick.h>

#include "button.h"
#include "pedal.h"


const int kButtonPin = 5;
const int kRxLed = 17;

int ledState = LOW;

Button _button;

const int kPedalCount = 2;
uint8_t _pedals[kPedalCount];

void OnButtonPressed()
{
  Joystick.begin(false);
  
  ledState = !ledState;
  digitalWrite(kRxLed, ledState);
  
  Serial.print("ledState=");
  Serial.println(ledState);
}

void setup()
{
  _button.Setup(kButtonPin, OnButtonPressed);
  
  pinMode(kRxLed, OUTPUT);

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

  sendJoystickIndividualAxis(pedalLeft, pedalRight);

  Joystick.sendState();
}

void sendJoystickIndividualAxis(int pedalLeft, int pedalRight)
{
  Joystick.setThrottle(ScalePedalOutput(pedalLeft));
  Joystick.setRudder(ScalePedalOutput(pedalRight));
}

uint8_t ScalePedalOutput(int pedal)
{
  uint8_t output = ((float)pedal / kPedalMax) * 255;
  Serial.println(output);
  return output;
}

