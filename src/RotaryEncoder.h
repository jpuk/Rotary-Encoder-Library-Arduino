#ifndef RotaryEncoder_h
#define RotaryEncoder_h
#include "arduino.h"

class RotaryEncoder {
  public:
  uint8_t _defaultValue; 
  uint8_t lastCount;
  volatile int virtualPosition;
  uint8_t _clk, _dt, _sw, _minValue, _increment;
  int _maxValue;
  void (*_buttonInterruptRoutine)();
  uint8_t setup(uint8_t clk, uint8_t dt, uint8_t sw, uint8_t defaultValue,
  				 uint8_t minValue, int maxValue,
                uint8_t increment,
  				 void (*interruptRoutine), void (*buttonInterruptRoutine));
  uint8_t getValue(void);
  bool checkButton(void);
  void setValue(uint8_t value);
  void resetValue(void);
};


#endif
