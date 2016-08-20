const int kButtonPin = 5;
const int kRxLed = 17;
const int kDebounceDelayMs = 50;

int buttonState = HIGH;
int lastButtonState = HIGH;

int ledState = LOW;

long lastDebounceTime = 0;

void setup()
{
  pinMode(kRxLed, OUTPUT);

  pinMode(kButtonPin, INPUT);  // Set the button as an input
  digitalWrite(kButtonPin, HIGH);

  Serial.begin(9600); // This pipes to the serial monitor
}

void loop()
{
  int reading = digitalRead(kButtonPin);

  if (reading != lastButtonState)
  {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > kDebounceDelayMs)
  {
    if (reading != buttonState)
    {
      buttonState = reading;

      if (buttonState == LOW)
      {
        ledState = !ledState;
        digitalWrite(kRxLed, ledState);
        
        Serial.print("ledState=");
        Serial.println(ledState);
      }
    }
  }

  lastButtonState = reading;
}
