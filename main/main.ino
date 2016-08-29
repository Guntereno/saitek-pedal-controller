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

Joystick_ _joystick(
  JOYSTICK_DEFAULT_REPORT_ID, // HID report ID
  2,      // Button Count
  0,      // Hat switch count
  false,  // X-Axis
  false,  // Y-Axis
  false,  // Z-Axis
  false,  // X-Rot
  false,  // Y-Rot
  false,  // Z-Rot
  false,  // Rudder
  false,  // Throttle
  true,   // Accelerator
  true,   // Brake
  true);  // Steering

void OnModeButtonPressed();


void setup()
{
  _joystick.setSteeringRange(-63, 63);
  _joystick.setBrakeRange(0, 63);
  _joystick.setAcceleratorRange(0, 63);

  _joystick.begin(false);

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

  _joystick.setButton(0, digitalRead(kRightButtonPin));
  _joystick.setButton(1, digitalRead(kLeftButtonPin));

  _joystick.sendState();
}

void OnModeButtonPressed()
{
  _rudderMode = !_rudderMode;
  digitalWrite(kModeLedPin, _rudderMode);
}

void sendJoystickRudder(int pedalLeft, int pedalRight)
{
  int pedalsCombined = pedalRight - pedalLeft;
  _joystick.setSteering(pedalsCombined);

  Serial.println(pedalsCombined);

  _joystick.setAccelerator(0);
  _joystick.setBrake(0);
}

void sendJoystickIndividualAxis(int pedalLeft, int pedalRight)
{
  _joystick.setAccelerator(pedalRight);
  _joystick.setBrake(pedalLeft);

  Serial.print(pedalLeft);
  Serial.print(",");
  Serial.println(pedalRight);

  _joystick.setSteering(0);
}


