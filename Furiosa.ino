#include <IRremote.hpp>

#include "Pixel.h"
#include "Furi_auto.h"

#include <SumoIR.h> 

SumoIR IR; 


// ====================================================================================
// Bluetooth
// ====================================================================================


/*#include <BluetoothSerial.h> 

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig'to and enable it
#endif

BluetoothSerial SerialBT; // variavel para guardar valores do tipo BluetoothSerial

char Val_rcv; // valor recebido do dispositivo conectado ao bluetooth*/


// ====================================================================================
// Controle de estados
// ====================================================================================


#define Btn_pin 0 // Botao geral VESPA 35

enum{

  Mod_stp = 5, // modo parado
  Mod_auto, // modo autonomo
  Mod_RC, // modo radio controlado
};

int Btn_actst = LOW; // estado atual do botao comeca no LOW

int Btn_lastst = LOW; // ultimo estado do botao comeca no LOW

int Mod_op = Mod_stp; // modo atual de operacao, inicia ja no modo parado


// ====================================================================================
// ESPNOW
// ====================================================================================


#include <esp_now.h>

#include <WiFi.h>

esp_now_peer_info_t peerInfo; // variavel para guardar informacao do peer

uint8_t broadcastAddress[] = {0x48, 0x31, 0xb7, 0x3e, 0x64, 0xa0}; // MAC Address em hexadecimal

typedef struct pacote {

    int code;
    int len;
    int ID;
    int ch[20]; //18
    
} pacote; // struct para o tipo de dados que queremos mandar

pacote pack_rx; // variavel que vai guardar os valores das variaveis

// funcao callback que vai ser executada quando os dados forem recebidos
void OnDataReceived(const uint8_t *mac_addr, const uint8_t *data, int data_len) {

  if( Mod_op != Mod_RC ) return;

  if( *((int*)data) == 1804 ) { // checa se é um pacote valido
    memcpy(&pack_rx, data, sizeof(pacote)); // copia os dados recebidos para o pacote

    int channel_sum = pack_rx.ch[0] + pack_rx.ch[1]; // soma os dois canais para melhor direçao

    int vy = map(pack_rx.ch[0], 1000, 2000, 1023, -1023); // mapeamento do eixo y
    int vx = map(pack_rx.ch[1], 1000, 2000, -1023, 1023); // mapeamento do eixo x

    int speedright = constrain(vx + vy,-1023,1023); 
    int speedleft  = constrain(vx - vy,-1023,1023); 

    motor.move(speedright, speedleft); // motor de acordo com o analogico

    Serial.print("Received Data: ID = ");
    Serial.print(pack_rx.ID);
    Serial.print(", Channels: ");
    for(int i = 0; i < pack_rx.len; i++) { // processa os dados recebidos conforme necessario
      Serial.print(" ");
      Serial.print(pack_rx.ch[i]);
    }
    Serial.println();
  }else {
    Serial.println("Received incorrect data");
  }
} 


void setup() {

  Serial.begin(115200); // iniciando Serial Monitor

  px_begin(); // led quando ligado
  px_fill(100, 0, 0);

  pinMode(R_sen, INPUT); // pino do sensor direito, como entrada
  pinMode(L_sen, INPUT); // pino do sensor esquerdo, como entrada
  pinMode(Btn_pin, INPUT); // pino do botao, como entrada

  motor.begin(); // inicia motor
  motor.bip(5,200,2000); // bip do motor iniciando

  IR.begin(35); // S2 VESPA

  // ESPNOW //
  WiFi.mode(WIFI_STA); // definindo o dispositivo como uma estaçao wifi

// verificando o ESPNOW
  if (esp_now_init() != ESP_OK) {
      Serial.println("Error initializing ESP-NOW");
      return;
  } 

  memcpy(peerInfo.peer_addr, broadcastAddress, 6); // registrando o peer
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

// verificando o peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      Serial.println("Failed to add peer");
      return;
  }

  esp_now_register_recv_cb(OnDataReceived); // registro para um funcao callback que sera chamada quando os dados forem recebidos

// BLUETOOTH //
  //SerialBT.begin("Furiosa mini"); // nome do bluetooth no dispositivo
}


void loop() {

  // altera o modo
  if( !digitalRead(Btn_pin) ){
    Mod_op++;
    if(Mod_op > Mod_RC) Mod_op = Mod_stp;

    switch(Mod_op){ // switch para as mudancas do modo do robo
      case Mod_stp: 
        px_fill(100, 0, 0);
      break;
      case Mod_auto: 
        px_fill( 0, 100, 0 );
      break;
      case Mod_RC: 
        px_fill( 0, 0, 100 );
      break;
    }
    delay(300); // fake debounce para o botao
  }
 

  switch(Mod_op){ // switch para as mudancas do modo do robo

    case Mod_stp: // modo parado
    
     motor.stop(); // motor parado
     break;


    case Mod_auto: { // modo autonomo

     IR.update(); // atualizacao do estado

    if(IR.prepare()) { // robo preparado

       px_fill(0,100,0); // led para indicar que o robo esta preparado
       delay(60);
       px_fill(0,0,0); // led para indicar que o robo esta preparado
       delay(60);

    }else if(IR.start()) { // chamada pro robo comecar

    //

    }else if (IR.on()){ // robo ligado

    px_fill( 0, 0, 0 );
    Furi_atk();
    //Furi_madmax();
    //Furi_greatwallofchina();
    //Furi_japan();
    //Furi_ghoul();

    }else if(IR.stop()) { // chamada pro robo parar
      
      px_fill( 100, 0, 0 );
      motor.stop();

    }else {  // robo desligado

    motor.stop();

    }
  }
    break;

    case Mod_RC: // modo radio controlado

    //
 
    break;
 } // fecha switch(Mod_op)
} // fecha loop