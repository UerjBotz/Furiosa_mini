#include <DRV8833.h>
DRV8833 motor(4,27, 13,14); // motor b e a VESPA pinout


#define l_sen 35 // sensor esquerdo S4 VESPA pinout
#define r_sen 36 // sensor direito S3

// ESTADOS DO AUTONOMO //
enum states_auto {
  search,
  atc,
  l_atc,
  r_atc
};

int state_at = 0; // estado atual

// LEITURA DOS SENSORES //
bool l_dtc() {return !digitalRead(l_sen);}
bool r_dtc() {return !digitalRead(r_sen);}

// DETECÇAO DOS SENSORES //
void furi_state() {
  state_at = 0;

  if(l_dtc() && r_dtc()) {
    state_at =atc;
  }else if(l_dtc()) {
    state_at = l_atc;
  }else if(r_dtc()) {
    state_at = r_atc;
  }else {
    state_at = search;
  }
}

// ESTRATEGIAS //

void search_japan(){

      motor.move(-1023, 1023);
      delay(200);
      motor.move(1023, 1023);
      delay(500);
      motor.move(1023, -1023);
      delay(200);
      motor.move(1023, 1023);
      delay(700);
      motor.move(1023, -1023);
      delay(200);
      motor.move(1023, 1023);
      delay(500);
      motor.move(1023, -1023);
      delay(200);
      motor.move(-800, 800);
}


void search_greatwallofchina(){

      motor.move(800, 0);
      delay(200);
      motor.move(-800, 0);
      delay(200);
      motor.move(0, 800);
      delay(200);
      motor.move(0, -800);
      delay(200);
      motor.move(800, 0);
      delay(200);
      motor.move(-800, 0);
      delay(200);
      motor.move(0, 800);
      delay(200);
      motor.move(0, -800);
      delay(200);
      motor.move(800, 0);
      delay(200);
      motor.move(-800, 0);
      delay(200);
      motor.move(0, 800);
      delay(200);
      motor.move(0, -800);
      delay(200);
}


void furi_japan() {

  furi_state();

    switch(state_at){
      
      case search:
      search_japan();
      break;

      case atc:
      motor.move(1023, 1023);
      break;

      case r_atc:
      motor.move(800, 1023);
      break;

      case l_atc:
      motor.move(1023, 800);
      break;
    }
}


void furi_greatwallofchina() {
  
  furi_state();

    switch(state_at){
      
      case search:
      search_greatwallofchina();
      break;

      case atc:
      motor.move(1023, 1023);
      break;

      case r_atc:
      motor.move(800, 1023);
      break;

      case l_atc:
      motor.move(1023, 800);
      break;
    }
}


void furi_pat() {
  furi_state();

  switch(state_at) {
    case search:
    motor.move(-800, 800);
    break;

    case atc:
    motor.move(1023, 1023);
    break;

    case l_atc:
    motor.move(-800, 1023);
    break;

    case r_atc:
    motor.move(-1023, 800);
    break;
  }
}


void furi_madmax() {
  motor.move(1023, 1023);
}


void furi_ghoul(){

  furi_state();

    switch(state_at){
      
      case search:
      motor.move(200, 200);
      break;

      case atc:
      motor.move(1023, 1023);
      break;

      case r_atc:
      motor.move(800, 1023);
      break;

      case l_atc:
      motor.move(1023, 800);
      break;
    }
}