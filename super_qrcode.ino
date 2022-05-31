#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32QRCodeReader.h>
#include "AsyncUDP.h"
#include <ArduinoJson.h>
//change this code with your own informations
#define ssid "POCOPHONE"
#define password "Aco142857"

String serializeShoppingList;
StaticJsonDocument<1024> shoppingList;
AsyncUDP udp;

ESP32QRCodeReader reader(CAMERA_MODEL_AI_THINKER);
struct QRCodeData qrCodeData;
#define RXp2 12
#define TXp2 13
bool isConnected = false;
bool a = true;

int delay_message_ard = 50;

void setup()
{
  //serial use to communicate with arduino.
  Serial.begin(115200, SERIAL_8N1, RXp2, TXp2);
  //serial use to receive message from arduino.
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  
  Serial.println();
  //wi-fi setup
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed");
    while (1) {
      delay(1000);
    }
  }
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed");
    while (1) {
      delay(1000);
    }
  }
  //UDP asychronous listner that take shopoping list from the app using JSON format.
  if (udp.listen(1234)) {
    Serial.print("UDP Listening on IP: ");
    Serial.println(WiFi.localIP());
    udp.onPacket([](AsyncUDPPacket packet) {
      Serial.print((const char*)packet.data());
      Serial.println();
      if (serializeShoppingList == "end") {
        shoppingList.clear();
        serializeShoppingList = (const char*)packet.data();
        Serial.println(serializeShoppingList);
        deserializeJson(shoppingList, (const char*)packet.data());
        udp.broadcast("received");

        send_arduino("forward");
      }
    });
  }
  reader.setup();
  Serial.println("Setup QRCode Reader");
  serializeShoppingList = "";
  reader.begin();
  Serial.println("Begin QR Code reader");

  delay(1000);
}
//handshacke protocole with arduino
void send_arduino(String message) {
  Serial.println("outer");
  Serial2.readString();
  while (Serial2.readString() != "received") {
    Serial.println("inner");
    Serial.println(message);
    delay(200);
  }
}

void loop()
{
  //flush the last qr code that have been reed
  reader.receiveQrCode(&qrCodeData, 100);
  if (serializeShoppingList.length() > 2) {
    while (a) {
      if (reader.receiveQrCode(&qrCodeData, 100))
      {

        Serial.println("Found QRCode");
        if (qrCodeData.valid)
        {
          Serial.print("Payload: ");
          Serial.println((const char *)qrCodeData.payload);
          Serial.println((int)shoppingList[(const char *)qrCodeData.payload]);
          //Code if the qr code is in the shoppingList
          if (shoppingList.containsKey((const char *)qrCodeData.payload)) {
            Serial.println("Item is in the list");
            Serial.println((const char *)qrCodeData.payload);

            int temp = (int)shoppingList[(const char *)qrCodeData.payload];
            Serial.println((int)shoppingList[(const char *)qrCodeData.payload]);
            shoppingList.remove((const char *)qrCodeData.payload);
            udp.broadcast((const char *)qrCodeData.payload);
            send_arduino("grab" + temp);
          }
          else {
            send_arduino("forward");
          }
          serializeShoppingList = "";
        serializeJson(shoppingList, serializeShoppingList);
        Serial.println(serializeShoppingList);
        //Qr code present at the charging station
        if ((const char *)qrCodeData.payload == "end") {
          serializeShoppingList = "";
          shoppingList.clear();
          udp.broadcast("end");
          send_arduino("reset");
          a = false;
        }
        }
        
      }
    }
    a = true;
  }

  delay(300);

}
