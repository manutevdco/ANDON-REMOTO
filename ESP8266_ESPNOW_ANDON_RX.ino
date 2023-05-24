/*
 *ALARME RX V1.0.1
 *STA MAC RX:
 *
  */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

#define LED_RX 5 //D1
#define LAMP 14 //D5
#define SIREN 13 //D7


#include <EasyButton.h>
#define BT_RST 12     //D6
EasyButton button_rst(BT_RST);


uint8_t broadcastAddress[] = {0xC8, 0xC9, 0xA3, 0x54, 0xE8, 0xC1};

bool alarm_on = false;

typedef struct struct_message {
    int data;
} struct_message;

struct_message MSG_DATA;
 
void setup() {
  Serial.begin(115200);
  
  pinMode(LED_RX, OUTPUT);
  digitalWrite(LED_RX, LOW);
  
  pinMode(LAMP, OUTPUT);
  digitalWrite(LAMP, HIGH);

  pinMode(SIREN, OUTPUT);
  digitalWrite(SIREN, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  //esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  esp_now_register_recv_cb(OnDataRecv);

  button_rst.begin();
  button_rst.onPressedFor(500, resetAlarm);
 
}
 
void loop() {
  button_rst.read();
  //digitalWrite(LED_RX, LOW);
  //digitalWrite(SIREN, LOW);
  
  if(alarm_on){
    digitalWrite(SIREN, HIGH);
    delay(500);
    digitalWrite(SIREN, LOW);
    alarm_on = false;
    }
  
  delay(50);


  
}

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&MSG_DATA, incomingData, sizeof(MSG_DATA));
  
  digitalWrite(LED_RX, !digitalRead(LED_RX));
  
  switch (MSG_DATA.data) {    
    case 101:
      alarm_on = true;
      digitalWrite(LAMP, LOW);
      Serial.println("Entrada 1: Acionada");
      break;
  }
}

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("MSG SENT SUCESS");
  }
  else{
    Serial.println("MSG SENT FAILED");
  }
}

void resetAlarm(){
  digitalWrite(LAMP, HIGH);
  digitalWrite(SIREN, LOW);
  alarm_on = false;
  Serial.println("ALARM DESATIVADO");
}
