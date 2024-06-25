#include <DRV8833.h>

DRV8833 motor(18,19, 4,23); // pinos do motor direito e motor esquerdo  (4,27, 13,14)

#define R_sen 39 // S3 VESPA, sensor direito
#define L_sen 34 // S4 VESPA, sensor esquerdo

// estados do autonomo
enum States_auto{

  Search, // busca
  Atk, // ataque 
  L_atk, // ataque esquedo
  R_atk, // ataque direito

};

int State_at; // estado atual

// leitura dos sensores
bool R_dtc(){ return !digitalRead(R_sen); }
bool L_dtc(){ return !digitalRead(L_sen); }

// classificação de estado
void Furi_state(){

  State_at = 0; // estado inicial ja na busca

// deteccao dos dois sensores
  if(R_dtc() && L_dtc()){  

    State_at = 1;

  } else if ( L_dtc() ){ 

    State_at = 2;

  } else if (R_dtc()){ 

    State_at = 3;

  }else {

    State_at = 0; 
  }
}

// estrategias

void Search_japan(){

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

void Search_greatwallofchina(){

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


void Furi_madmax() {

  motor.move(1023, 1023);

}

void Furi_atk() {
    
    Furi_state();

    switch(State_at){
      
      case Search:
      motor.move(-800, 800);
      break;

      case Atk:
      motor.move(1023, 1023);
      break;

      case R_atk:
      motor.move(800, 1023);
      break;

      case L_atk:
      motor.move(1023, 800);
      break;
    }
}



void Furi_japan() {

  Furi_state();

    switch(State_at){
      
      case Search:
      Search_japan();
      break;

      case Atk:
      motor.move(1023, 1023);
      break;

      case R_atk:
      motor.move(800, 1023);
      break;

      case L_atk:
      motor.move(1023, 800);
      break;
}


/*void Furi_greatwallofchina() {
  
  Furi_state();

    switch(State_at){
      
      case Search:
      Search_greatwallofchina();
      break;

      case Atk:
      motor.move(1023, 1023);
      break;

      case R_atk:
      motor.move(800, 1023);
      break;

      case L_atk:
      motor.move(1023, 800);
      break;
    }
}


void Furi_ghoul(){

  Furi_state();

    switch(State_at){
      
      case Search:
      motor.move(200, 200);
      break;

      case Atk:
      motor.move(1023, 1023);
      break;

      case R_atk:
      motor.move(800, 1023);
      break;

      case L_atk:
      motor.move(1023, 800);
      break;
    }
}*/

