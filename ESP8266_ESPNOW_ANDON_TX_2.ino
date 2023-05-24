/*
 * 
 * TRANSMISSOR TX
 * MAC PLACA TX: C8:C9:A3:54:E8:C1
 * 
 */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

#define LED_RX 5 //D1



// REPLACE WITH THE MAC RX
uint8_t broadcastAddress[] = {0xE8, 0x9F, 0x6D, 0x93, 0x51, 0x53};


typedef struct struct_message {
    int data;
} struct_message;

struct_message MSG_DATA;



// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("MENSAGEM RECEBIDA!");
    digitalWrite(LED_RX, LOW);
    
  }else{
    Serial.println("MENSAGEM NAO RECEBIDA!");
  }
}

// Callback when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&MSG_DATA, incomingData, sizeof(MSG_DATA));
  
}
 
void setup() {
  // Init Serial Monitor
  //Serial.begin(115200);
  MSG_DATA.data = 101;

  pinMode(LED_RX, OUTPUT);
  digitalWrite(LED_RX, HIGH);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    esp_now_register_send_cb(OnDataSent);
    esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
    esp_now_register_recv_cb(OnDataRecv);
}
 
void loop()
  {
      esp_now_send(broadcastAddress, (uint8_t *) &MSG_DATA, sizeof(MSG_DATA));
      delay(100);
  }
