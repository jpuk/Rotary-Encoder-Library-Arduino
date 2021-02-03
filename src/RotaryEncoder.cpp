#include "Arduino.h"
#include "RotaryEncoder.h" 

uint8_t RotaryEncoder::setup(uint8_t clk, uint8_t dt, uint8_t sw, int defaultValue,
						int minValue, int maxValue, uint8_t increment,
						void (*rotaryInterruptRoutine), void (*buttonInterruptRoutine)){
  Serial.println("Setting up rotary encoder");
  _clk = clk;
  _dt = dt;
  _sw = sw;
  _defaultValue = defaultValue;
  lastCount = defaultValue;
  _increment = increment;
  virtualPosition = defaultValue;
  _minValue = minValue;
  _maxValue = maxValue;
  _buttonInterruptRoutine = buttonInterruptRoutine;
  pinMode(clk, INPUT_PULLUP);
  pinMode(dt, INPUT_PULLUP);
  pinMode(sw, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(clk), rotaryInterruptRoutine, LOW);
  return 0;
}

int RotaryEncoder::getValue(void){
      if (digitalRead(_dt) == LOW)
      {
          virtualPosition = virtualPosition - _increment;
          //Serial.println("--");
      }
      else {
          virtualPosition = virtualPosition + _increment;
          //Serial.println("++");
      }
      // Restrict value from 0 to +100
      virtualPosition = min(_maxValue, max(_minValue, virtualPosition));
  
    // Write out to serial monitor the value and direction
    //Serial.print(virtualPosition > lastCount ? "Up  :" : "Down:");
    //Serial.println(virtualPosition);
    // Keep track of this new value
    lastCount = virtualPosition ;
  
  
  return virtualPosition;
}

void RotaryEncoder::setValue(int value){
  virtualPosition = value;
}

void RotaryEncoder::resetValue(void){
  //Serial.println("Resetting to default position");
  virtualPosition = _defaultValue;
}

bool RotaryEncoder::checkButton(void){
  bool buttonPressed = false;
  // Is someone pressing the rotary switch?
  if ((!digitalRead(_sw))) {
    buttonPressed = true;
    (*_buttonInterruptRoutine)();
//rotaryButtonInterruptHandler();
    while (!digitalRead(_sw))
      delay(1);
    //Serial.println("Rotary switch pressed");
  }
  return buttonPressed;
}
