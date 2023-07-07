#ifndef _STEPPER_H
#define _STEPPER_H

#include <Arduino.h>
#include "Configuration.h"

#include "AccelStepper.h"

class Stepper
{
private:
  AccelStepper _stepper;
  long double _coord;
  int _min;
  int _max;
  int _max_speed;
  int _speed;
  byte _en_pin;
  byte _en_inv;
  byte _endstop_pin;
  byte _endstop_inv;
  int _endstop_coord;
  long double _spm;
  long _calc_spm(long double x);
public:
  Stepper(byte step, byte dir, byte en, byte step_inv, byte dir_inv, byte en_inv);
  void begin(float step_distance, int max_speed, int acc, int min_coord, int max_coord, byte endstop_pin, byte endstop_inv, int endstop_coord);
  void move_relative(float x);
  void set_speed(long speed);
  int get_coord();
  void proccess();
  bool auto_home(int speed, int bounce_dist, int bounce_speed, int add_dist=10);
  bool _endstop_read();
};

#endif

