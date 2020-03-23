/**
   COVID-19 Turkiye & World Numbers
   show by 16x2 lcd matrix display
   Turkiye numbers include case - recovered -death
   World numbers  case - death
   created from basichttpsclient sample

    Created on: 22.03.2020 sunny sunday

*/

/*
 * scrolling display will add for all lines, 
 */

#include <Wire.h>
#include "rgb_lcd.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#include <WiFiClientSecureBearSSL.h>
byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};
byte frownie[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b00000,
  0b01110,
  0b10001
};
byte smiley[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};
byte armsDown[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b01010
};

byte armsUp[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b10101,
  0b01110,
  0b00100,
  0b00100,
  0b01010
};
// Fingerprint for covid19 api

//be carefull if you use antivirus with web protection you wil get wrong fingerprint

const uint8_t fingerprint[20] = {0xe5, 0x37, 0x76, 0x46, 0x01, 0xf6, 0x5d, 0xdf, 0x5d, 0x6a, 0x6c, 0x10, 0xde, 0xba, 0x4d, 0x9e, 0x8e, 0xc9, 0xd1, 0x49};

//‎e5 37 76 46 01 f6 5d df 5d 6a 6c 10 de ba 4d 9e 8e c9 d1 49

// Use arduinojson.org/v6/assistant to compute the capacity.
const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 1500;
DynamicJsonDocument doc(capacity);
rgb_lcd lcd;

ESP8266WiFiMulti WiFiMulti;

void setup() {

  Serial.begin(74880);
  // Serial.setDebugOutput(true);
  lcd.begin(16, 2);
  lcd.createChar(0, heart);
  lcd.createChar(1, smiley);
  lcd.createChar(2, frownie);
  lcd.createChar(3, armsDown);
  lcd.createChar(4, armsUp);
  lcd.clear();
  lcd.setCursor(0, 0);
  Serial.println();
  Serial.println();
  Serial.println();
  lcd.print("Corona Counter");
  lcd.setCursor(0,1);
  lcd.print("Turkey & World");

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("korona", "xxxxxxxxxxx");
}

void loop() {
  // wait for WiFi connection
  //WiFi.forceSleepWake();


  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Internet...");
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

    client->setFingerprint(fingerprint);

    HTTPClient https;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WEB...");
    Serial.print("[HTTPS] begin...\n");//https://covid19.mathdro.id/api/countries/turkey  
    if (https.begin(*client, "https://covid19.mathdro.id/api/countries/turkey")) {  // HTTPS

      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      int httpCode = https.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = https.getString();
          Serial.println(payload);
         
          // Parse JSON object
          DeserializationError error = deserializeJson(doc, payload);

          if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.c_str());
            return;
          }

          // Extract values
          lcd.clear();
          lcd.setCursor(0, 0);
          Serial.println(F("Response:"));
          Serial.println(doc["confirmed"]["value"].as<long>());
          lcd.print(doc["confirmed"]["value"].as<long>());
          lcd.write((unsigned char)3);          
          Serial.println(doc["recovered"]["value"].as<long>());
          lcd.print(doc["recovered"]["value"].as<long>());
          lcd.write((unsigned char)0);
          Serial.println(doc["deaths"]["value"].as<long>());
          lcd.print(doc["deaths"]["value"].as<long>());
          lcd.write((unsigned char)2);


        }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    }
    delay(199);
    //https://covid19.mathdro.id/api
    if (https.begin(*client, "https://covid19.mathdro.id/api")) {  // HTTPS

      Serial.print("[HTTPS] GET.2..\n");
      // start connection and send HTTP header
      int httpCode = https.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = https.getString();
          Serial.println(payload);
          DeserializationError error = deserializeJson(doc, payload);

          if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.c_str());
            return;
          }

          // Extract values
          lcd.setCursor(0, 1);
          Serial.println(F("Response:"));
          Serial.println(doc["confirmed"]["value"].as<long>());
          lcd.print(doc["confirmed"]["value"].as<long>());
          lcd.write((unsigned char)3);
          // Serial.println(doc["time"].as<long>());
          // Serial.println(doc["data"][0].as<float>(), 6);
          Serial.println(doc["recovered"]["value"].as<long>());
          Serial.println(doc["deaths"]["value"].as<long>());
          lcd.print(doc["deaths"]["value"].as<long>());
          lcd.write((unsigned char)2);

        }
      } else {
        Serial.printf("[HTTPS] GET..2. failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }

  Serial.println("Wait 40s before next round...");
  // WiFi.forceSleepBegin();


  delay(40000);
}
