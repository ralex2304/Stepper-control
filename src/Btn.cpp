#include "Btn.h"

Btn::Btn(void(*func)(int ind, float x),int num, int* pins, int* invs, float* vals)
{
  _btn_f=func;
  _num=num;
  _pins=pins;
  _vals=vals;
  _invs=invs;
  _timer=new unsigned long [_num];
}

void Btn::begin(){
  for(int i=0;i<_num;i++){
    pinMode(_pins[i],INPUT_PULLUP);
    _timer[i]=0;
  }
}

void Btn::proccess(){
  for(int i=0;i<_num;i++){
    if(_timer[i]==0 && (digitalRead(_pins[i])^_invs[i])){
      _timer[i]=millis();
    }
    else if(_timer[i]!=0 && !(digitalRead(_pins[i])^_invs[i])){
        if(millis()-_timer[i]>100){
          _btn_f(i,_vals[i]);
          #ifdef DEBUG
            Serial.print("Button pressed: ");
            Serial.println(i);
          #endif
        }
        _timer[i]=0;
    }
  }
}

void Btn::wait_for_pressed(){
  bool wait=true;
  while(wait){
    for(int i=0;i<_num;i++){
      if(digitalRead(_pins[i])^_invs[i]){
        wait=false;
      }
    }
  }
}