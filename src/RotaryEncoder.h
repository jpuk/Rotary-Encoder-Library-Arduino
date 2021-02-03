#ifndef RotaryEncoder_h
#define RotaryEncoder_h
#include "arduino.h"

class RotaryEncoder {
  public:
  int _defaultValue;
  int lastCount;
  volatile int virtualPosition;
  uint8_t _clk, _dt, _sw, _increment;
  int _minValue, _maxValue;
  void (*_buttonInterruptRoutine)();
  uint8_t setup(uint8_t clk, uint8_t dt, uint8_t sw, int defaultValue,
  				 int minValue, int maxValue,
                uint8_t increment,
  				 void (*interruptRoutine), void (*buttonInterruptRoutine));
  int getValue(void);
  bool checkButton(void);
  void setValue(int value);
  void resetValue(void);
};


#endif
