//========================================
// CONTROLE DOS ESTADOS
//========================================
#define btn_pin 35 // VESPA botao pinout

enum {
  mod_stp, // modo parado
  mod_auto, // autonomo
  mod_rc, // radio controlado (ESPNOW)
  mod_ps4 // controle PS4
};

int btn_actst = LOW; // estado atual do botao
int btn_lastst = LOW; // ultimo
int mod_op = mod_stp; // modo de operaçao


//========================================
// ESPNOW
//========================================
#include <esp_now.h>
#include <WiFi.h>

esp_now_peer_info_t peerInfo;

uint8_t broadcastAddress[] = {0x48, 0x31, 0xb7, 0x3e, 0x64, 0xa0};

typedef struct struct_message {
  //int code;
  //int len;
  //int ID;
  int ch[20];
  int x_axis;
  int y_axis;
} strucut_message;

struct_message myData;

void OnDataRecv(const uint8_t* mac_addr, const uint8_t* data, int len) {
  if(mod_op != mod_rc) return;

  memcpy(&myData, data, sizeof(myData));

  //int channel_sum = myData.ch[0] + myData[1];

  int vy = map(myData.ch[0], 1000, 2000, 1023, -1023);
  int vx = map(myData.ch[1], 1000, 2000, -1023, 1023);

  int speedright = constrain(vx + vy, -1023,1023);
  int speedleft = constrain(vx - vy, -1023, 1023);

  //motor.move(speedright, speedleft);
}

//========================================
// AUTO
//========================================
#include "Auto.h"

#include <SumoIR.h>
SumoIR IR;

//========================================
// PIXEL
//========================================
#include "Pixel.h"


//========================================
// BLUETOOTH
//========================================
#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig'to and enable it
#endif

BluetoothSerial SerialBT; // variavel para guardar valores do tipo BluetoothSerial

String strategy = "default"; // valor recebido do dispositivo conectado ao bluetooth


void setup() {

  Serial.begin(115200); // iniciando o serial


  // PIXEL //
  px_begin();


  // ESPNOW //
  WiFi.mode(WIFI_STA);
  
  // VERIFICANDO O ESPNOW //
  if(esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // REGISTRANDO O PEER //
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  //VERIFICANDO O PEER //
  if(esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // CALLBACK ESPNOW //
  esp_now_register_recv_cb(OnDataRecv);


  // RECEPTOR INFRAVERMELHO 
  IR.begin(35); // S2 VESPA pinout


  // MOTOR //
  motor.begin();
  motor.bip(5, 200, 2000);


  // BLUETOOTH //
  SerialBT.begin("FuriosaMini"); // nome do bluetooth
}

void loop() {

  if(!digitalRead(btn_pin)) {
    mod_op++;
    if(mod_op > mod_ps4) mod_op = mod_stp;

    switch(mod_op) {
      case mod_stp:
      px_fill(100, 0, 0);
      break;

      case mod_auto:
      px_fill(0, 100, 0);
      break;

      case mod_rc:
      px_fill(0, 0, 100);
      break;

      case mod_ps4:
      px_fill(100, 100, 0);
      break;
    }
    delay(300);
  }


  if(SerialBT.available()) {
    String received = SerialBT.readStringUntil('\n');
    received.trim();

    if(received == "madmax") {
      strategy = "madmax";
      SerialBT.println("Strategy set to madmax");
    }else if(received == "japan") {
      strategy = "japan";
      SerialBT.println("Strategy set to japan");
    }else if(received == "greatwallofchina") {
      strategy = "greatwallofchina";
      SerialBT.println("Strategy set to gwc");
    }else if(received == "ghoul") {
       strategy = "ghoul";
      SerialBT.println("Strategy set to ghoul");
    }else {
      SerialBT.println("Unknown strategy");
    }
  }


  switch(mod_op) {

    case mod_stp:
    motor.stop();
    break;

    case mod_auto: {
      // atualizaçao do estado // 
      IR.update();

      // robo preparado //
      if(IR.prepare()) {

        if(IR.available() && IR.read() == 1) {
          px_fill(0, 100, 0);
          delay(80);
          px_fill(0, 0, 0);
          delay(80);
        }


      // chamada pro robo começar //
      }else if(IR.start()) {
        //


      }else if(IR.on()) {

        if(strategy == "madmax") {
          furi_madmax();

        }else if(strategy == "japan") {
          furi_japan();

        }else if(strategy == "greatwallofchina") {
          furi_greatwallofchina();

        }else if(strategy == "ghoul") {
          furi_ghoul();

        }else {
          furi_pat();
        }


      }else if(IR.stop()){
        motor.stop();
        //rainbow(10);

      }else {
        motor.stop();
      }
    }
    break;


    case mod_rc:
    break;


    case mod_ps4:
    break;
  } // fecha switch
} // fecha loop