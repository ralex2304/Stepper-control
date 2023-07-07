/**
 * One axis stepper motor drive
 * Author ralex2304
 * https://github.com/ralex2304
 * For Benzeen Diesel
 * 2022
*/
#include <Arduino.h>
#include "Configuration.h"
#include "Stepper.h"
#include "Btn.h"

#ifdef DEBUG
unsigned long ser_timer=millis();
#endif

Stepper stepper(STEP_PIN,DIR_PIN,EN_PIN,STEP_INV,DIR_INV,EN_INV);

void btn_f(int ind,float x){
  String btn_funcs[BTN_NUM]=BTN_FUNCTIONS;
  if(btn_funcs[ind]=="move"){
    stepper.move_relative(x);
  }
}

int btn_pins[]=BTN_PINS;
int btn_invs[]=BTN_INVS;
float btn_vals[]=BTN_FUNCTIONS_VALS;

Btn btn(btn_f,BTN_NUM,(int *)btn_pins,(int *)btn_invs, (float *)btn_vals);


void setup() {
  #ifdef DEBUG
    Serial.begin(9600);
    while(!Serial){}
    Serial.println("Setup");
  #endif
  btn.begin();
  stepper.begin(STEPS_PER_MM,MAX_SPEED,ACCELERATION,MIN_COORD,MAX_COORD,ENDSTOP_PIN,ENDSTOP_INV,ENDSTOP_POSITION);
  btn.wait_for_pressed();
  if(!stepper.auto_home(HOMING_SPEED,HOMING_RETRACT_DIST,HOMING_RETRACT_SPEED,HOMING_ADD_DIST)){
    #ifdef DEBUG
      Serial.println("Homing failed!!!");
    #endif
    while(true){}
  }
  
}

void loop() {
  stepper.proccess();
  btn.proccess();
  #ifdef DEBUG
    if(millis()-ser_timer>DEBUG_INF_PERIOD){
      Serial.print("Target position: ");
      Serial.println(stepper.get_coord());
      ser_timer=millis();
    }
  #endif
}

