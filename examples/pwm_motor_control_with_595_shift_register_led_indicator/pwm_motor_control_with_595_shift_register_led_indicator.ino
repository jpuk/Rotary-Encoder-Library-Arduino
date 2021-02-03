/*MIT License

Copyright (c) 2021 John Porter

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

// take input from a rotary encoder and control the speed of a motor via pwm
// give visual indication of the encoder position by using pwm to light up 4 leds sequentially 
// using ping 7 to 4 on the 595
//
// using non-standard data port on the 595 and overriding default in Shift register library
#define ShiftRegisterPWM_DATA_PORT PORTB
#define ShiftRegisterPWM_DATA_MASK 1
// using https://github.com/Simsso/ShiftRegister-PWM-Library to provide software controlled pwm
// to the 595 shift register which does not natively support this.
#include <ShiftRegisterPWM.h>
#define SHIFT_REG_DATA_PIN 8
#define SHIFT_REG_CLOCK_PIN 4
#define SHIFT_REG_LATCH_PIN 3

#include "RotaryEncoder.h"
//#include "ButtonObject.h"

// one shift register with 8 positions 
ShiftRegisterPWM sr(1, 8);

const int PWMPin = 11;
const int rotaryPinA = 2;
const int rotaryPinB = 5;
const int rotaryButtonPin = 6;
const int defaultPosition = 0;
const int maxValue = 255;
const int minValue = 0;

RotaryEncoder *rotary;


void setMotorSpeed(int motorSpeed){
  // validate that value is in range of what pwm output can handle
  if ((motorSpeed >= 0) && (motorSpeed <= 255)){
    analogWrite(PWMPin, motorSpeed);
  }
}

void rotaryInterruptHandler(void){
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  // If interrupts come faster than 5ms, assume it's a bounce and ignore
  if (interruptTime - lastInterruptTime > 5) {
    //Serial.println("Rotary Interrupt fired");  
    int value = rotary->getValue();
    setMotorSpeed(value);

    // light up each led sequentially from 7 to 4 as the rotary encoder moves from 0 to 255
    if (value < 64){
      for(int i = 4; i < 7;i++){
        sr.set(i,0); 
        }   
       sr.set(7, value * 4);     
      }
    if (value > 64 && value < 128){
      for(int i = 4; i < 6;i++){
        sr.set(i,0); 
        }  
        sr.set(7, 255);
        sr.set(6, (value - 64) * 4);
    }
    if (value > 128 && value < 192){
      for(int i = 4; i < 5;i++){
        sr.set(i,0); 
        }  
        sr.set(7, 255);
        sr.set(6, 255);
        sr.set(5, (value - 128) * 4);
    }
    if (value > 192 && value < 256){
        sr.set(7, 255);
        sr.set(6, 255);
        sr.set(5, 255);
        sr.set(4, (value - 192) * 4);
    }
      
  }
  lastInterruptTime = interruptTime;
    
}

void rotaryButtonInterruptHandler(void){
  //Serial.println("Rotary button Interrupt fired");  
  rotary->resetValue();
  setMotorSpeed(rotary->getValue());  
}

void checkButtons(void){
  rotary->checkButton();

}

void setup() {
  Serial.begin(115200);
  
  // setup pins for 595 shift register controlled by shift register pwm library
  pinMode(SHIFT_REG_DATA_PIN, OUTPUT); // data
  pinMode(SHIFT_REG_CLOCK_PIN, OUTPUT); // clock
  pinMode(SHIFT_REG_LATCH_PIN, OUTPUT); // latch pin 
  sr.interrupt(ShiftRegisterPWM::UpdateFrequency::SuperFast);

  // pwm for motor
  pinMode(PWMPin, OUTPUT);
  
  rotary = new RotaryEncoder();
  rotary->setup(rotaryPinA, rotaryPinB, rotaryButtonPin, defaultPosition, minValue, maxValue, 2,
                rotaryInterruptHandler, rotaryButtonInterruptHandler);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  checkButtons();
}
