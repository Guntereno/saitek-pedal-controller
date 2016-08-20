#include "button.h"

const int kButtonPin = 5;
const int kRxLed = 17;

int ledState = LOW;

Button gButton;

void OnButtonPressed()
{
  ledState = !ledState;
  digitalWrite(kRxLed, ledState);
  
  Serial.print("ledState=");
  Serial.println(ledState);
}

void setup()
{
  gButton.Setup(kButtonPin, OnButtonPressed);
  
  pinMode(kRxLed, OUTPUT);

  Serial.begin(9600); // This pipes to the serial monitor
}

void loop()
{
  gButton.Loop();
}



