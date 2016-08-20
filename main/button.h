struct Button
{
  const int kDebounceDelayMs = 50;
  
  typedef void (* PressHandler) ();
  
  int _buttonPin;
  int _buttonState = HIGH;
  int _lastButtonState = HIGH;
  long _lastDebounceTime = 0;
  PressHandler _onPress;
  
  void Setup(int pin, PressHandler onPress)
  {
    _buttonPin = pin;
    _onPress = onPress;
    
    pinMode(_buttonPin, INPUT);  // Set the button as an input
    digitalWrite(_buttonPin, HIGH);
  }

  void Loop()
  {
      int reading = digitalRead(_buttonPin);

      if (reading != _lastButtonState)
      {
        _lastDebounceTime = millis();
      }
    
      if ((millis() - _lastDebounceTime) > kDebounceDelayMs)
      {
        if (reading != _buttonState)
        {
          _buttonState = reading;
    
          if (_buttonState == LOW)
          {
            _onPress();
          }
        }
      }
    
      _lastButtonState = reading;
  }
};
