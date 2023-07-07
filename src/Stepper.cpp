#include "Stepper.h"


Stepper::Stepper(byte step, byte dir, byte en, byte step_inv, byte dir_inv, byte en_inv){
  _en_pin=en;
  _en_inv=en_inv;
  _stepper = AccelStepper(1,step,dir);
  _stepper.setPinsInverted(dir_inv,step_inv);
  _coord = 0;
  _spm=0;
  _min=0;
  _max=0;
  _max_speed=0;
  _endstop_inv=0;
  _endstop_coord=0;
}

void Stepper::begin(float step_per_mm, int max_speed, int acc, int min_coord, int max_coord, byte endstop_pin, byte endstop_inv, int endstop_coord){
  _spm=step_per_mm;
  _stepper.setAcceleration(_calc_spm(acc));
  _stepper.enableOutputs();
  _min=min_coord;
  _max=max_coord;
  _max_speed=max_speed;
  set_speed(_max_speed);
  _endstop_pin=endstop_pin;
  _endstop_inv=endstop_inv;
  _endstop_coord=min(endstop_coord,_min);
  pinMode(_endstop_pin,INPUT_PULLUP);
  pinMode(_en_pin,HIGH^_en_inv);
  return;
}

void Stepper::move_relative(float x){
  if(_coord+x<_min){
    _coord=_min;
  }else if(_coord+x>_max){
    _coord=_max;
  }else{
    _coord+=x;
  }
  _stepper.moveTo(_calc_spm(_coord)); 
  return;
}

int Stepper::get_coord(){
  return _coord;
}

void Stepper::proccess(){
  _stepper.run();
  return;
}

void Stepper::set_speed(long speed){
  if(speed<=_max_speed){
    _stepper.setMaxSpeed(_calc_spm(speed));
    _stepper.setSpeed(_calc_spm(speed));
  }
  return;
}

bool Stepper::auto_home(int speed, int bounce_dist, int bounce_speed, int add_dist){
  set_speed(speed);
  _stepper.move(_calc_spm(_min-_max-add_dist));
  while(!_endstop_read()){
    if(!_stepper.run()){
      return false;
    }
  }
  _stepper.stop();
  _stepper.setCurrentPosition(0);
  _stepper.moveTo(_calc_spm(bounce_dist));
  while(_stepper.run()){}
  set_speed(bounce_speed);
  _stepper.moveTo(_calc_spm(-add_dist));
  while(!_endstop_read()){
    if(!_stepper.run()){
      return false;
    }
  }
  _stepper.stop();
  _stepper.setCurrentPosition(_endstop_coord);
  set_speed(speed);
  _stepper.moveTo(0);
  while(_stepper.run()){}
  return true;
}

long Stepper::_calc_spm(long double x){
  return x*_spm;
}

bool Stepper::_endstop_read(){
  return digitalRead(_endstop_pin)^_endstop_inv;
}
