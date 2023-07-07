#ifndef _BTN_H
#define _BTN_H

#include <Arduino.h>
#include "Configuration.h"

class Btn
{
private:
  void (*_btn_f)(int ind,float x);
  int _num;
  unsigned long* _timer;
  int* _pins;
  int* _invs;
  float* _vals;
public:
  Btn(void(*func)(int ind, float x),int num, int* pins, int* invs, float* vals);
  void begin();
  void proccess();
  void wait_for_pressed();
};
#endif