/*
  Copyright 2018 Electrical Engineering Enterprise Group.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  ESP_GPS tracking using Firebase  is a sample that demo of the GPS Tracker module
  using GY-NEO6MV2 NEO-6M GPS Module to scan Location and then nodeMCU will send data
  to Firebase for map using Google MAP Api and shown on Web application.

  Dev.Jukrapun Chitpong
  facebook.com/eeeg.ch
  date Jan29, 2017
  Uo to date - Dec25, 2017
  Uo to date - Jan2,  2018

*/

#include <TinyGPS++.h>                                  // Tiny GPS Plus Library
#include <SoftwareSerial.h>                             // Software Serial Library so we can use other Pins for communication with the GPS module
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
#include <time.h>

#define YOUR_SSID "YOUR_SSID"
#define YOUR_PASS "YOUR_PASS"
#define FIREBASE_io "example.firebaseio.com"          // Select > Database 
#define FIREBASE_dataSecret "Database_Secrets"        // Project Overview > Project  Settings > SERVICE ACCOUNTS > Database Secrets 

const double Office_LAT = 13.9017211;                 // Your Home Latitude
const double Office_LNG = 100.5327769;                // Your Home Longitude

unsigned int localPort = 2390;
static const int RXPin = 12, TXPin = 13;              // Ublox 6m GPS module to pins 12 and 13
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;                                      // Create an Instance of the TinyGPS++ object called gps
SoftwareSerial ss(RXPin, TXPin);                      // The serial connection to the GPS device

boolean connectioWasAlive = true;

int Sat = 0;
int Minute = 0;
int Minuted = 0;
int Hour = 0;
int Day = 0;
int DayS = 0;
int A = 1;
int B = 0;
double LAT = 13.85905;
double LNG = 100.52071;

// Config time
int timezone = 7;       // Zone +7 for Thailand
char ntp_server1[20] = "time.navy.mi.th";
char ntp_server2[20] = "time.nist.gov";
char ntp_server3[20] = "time.uni.net.th";
int dst = 0;
String d, m, y, h, t;

unsigned long previousMillis = 0;
const long interval = 1000;

void setup() {

//  Serial.end();
//  Serial.begin(115200);
  delay(1);

  ss.begin(GPSBaud);                                    // Set Software Serial Comm Speed to 9600

  wifiMulti.addAP("i-Juk", "P@ssw0rd");
  wifiMulti.addAP("EEEG.CH", "UFI-IoT-2017");
  wifiMulti.addAP(YOUR_SSID, YOUR_PASS);

  while (wifiMulti.run() != WL_CONNECTED) {
//    Serial.println("WiFi not connected!");
    delay(1000);
  }
//  Serial.println("WiFi connected!");
  delay(500);

  /*---- Config Firebase ----*/
//  Firebase.begin("example.firebaseio.com", "token_or_secret");
  Firebase.begin(FIREBASE_io, FIREBASE_dataSecret);

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["OfficeLAT"] = LocLat1();
  root["OfficeLNG"] = LocLng1();

  Firebase.set("GPS", root);

  /*---- Config Time ----*/
  configTime(timezone * 3600, dst, ntp_server1, ntp_server2, ntp_server3);
//  Serial.println("Waiting for time");

}

void loop() {
  if (wifiMulti.run() != WL_CONNECTED) {
//    Serial.println("WiFi not connected!");
    delay(1000);
  } else {}

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    LAT = gps.location.lat();
    LNG = gps.location.lng();

    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["TrackLAT"] = LocLat11();
    root["TrackLNG"] = LocLng11();
    root["OfficeLAT"] = LocLat1();
    root["OfficeLNG"] = LocLng1();

//    A = (float)Firebase.get("Amount/A");
    if (A > 0) {
      A++;
      B = !B;

      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& rootC = jsonBuffer.createObject();
      rootC["A"] = A;
      rootC["B"] = B;
      Firebase.set("Amount", rootC);
    } else {}
    delay(100);

    /*------------------- Local_Time.SET to Firebase -------------------*/
    NowString();

    if (Minuted < Minute) {
      Firebase.set("TinyGPSPlusTIME/hour", Hour);
      Firebase.set("TinyGPSPlusTIME/minute", Minute);
      Firebase.set("TinyGPSPlusDAY/year", y.toInt());
      Firebase.set("TinyGPSPlusDAY/month", m.toInt());
      Firebase.set("TinyGPSPlusDAY/day", d.toInt());

      Firebase.set("GPS", root);
      
    } else {}
    yield();

    /*------------------- Serial -------------------
    Serial.print("Latitude  : ");
    Serial.print(LAT, 6);
    Serial.print("    Longitude : ");
    Serial.print(LNG, 6);
    Serial.print("    Time NTP  : ");*/
    delay(10);

    smartDelay(500);    // Run Procedure smartDelay

    Minuted = Minute;
    DayS = Day;
  }
}

String LocLat11() {
  String LocLat11 = "";
  LocLat11 += String(LAT, 6);

  return LocLat11;
}

String LocLng11() {
  String LocLng11 = "";
  LocLng11 += String(LNG, 6);

  return LocLng11;
}

String LocLat1() {
  String LocLat1 = "";
  LocLat1 += String(Office_LAT, 6);

  return LocLat1;
}

String LocLng1() {
  String LocLng1 = "";
  LocLng1 += String(Office_LNG, 6);

  return LocLng1;
}

String NowString() {
  time_t now = time(nullptr);
  struct tm* newtime = localtime(&now);

  String tmpNow = "";
  tmpNow += String(newtime->tm_year + 1900);
  tmpNow += "-";
  tmpNow += String(newtime->tm_mon + 1);
  tmpNow += "-";
  tmpNow += String(newtime->tm_mday);
  tmpNow += " ";
  tmpNow += String(newtime->tm_hour);
  tmpNow += ":";
  tmpNow += String(newtime->tm_min);
  tmpNow += ":";
  tmpNow += String(newtime->tm_sec);

  y = String(newtime->tm_year + 1900);
  //  Serial.print(y);
  m = String(newtime->tm_mon + 1);
  //  Serial.print(m);
  d = String(newtime->tm_mday);
  //  Serial.print(d);
  Hour = String(newtime->tm_hour).toInt();
  //  Serial.print(Hour);
  Minute = String(newtime->tm_min).toInt();
  //  Serial.println(Minute);

//  Serial.println(tmpNow);
  return tmpNow;

}

static void smartDelay(unsigned long ms) {               // This custom version of delay() ensures that the gps object is being "fed".
  unsigned long start = millis();
  do {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

/*Credit
   http://www.instructables.com/id/ESP8266-12e-With-GPS-OLED-Display/?ALLSTEPS
   https://github.com/mkconer/ESP8266_GPS
   http://arduiniana.org/libraries/tinygpsplus/

*/

/*Ref.TinyPlus++
    Serial.println(gps.location.lat(), 6); // Latitude in degrees (double)
    Serial.println(gps.location.lng(), 6); // Longitude in degrees (double)
    Serial.print(gps.location.rawLat().negative ? "-" : "+");
    Serial.println(gps.location.rawLat().deg); // Raw latitude in whole degrees
    Serial.println(gps.location.rawLat().billionths);// ... and billionths (u16/u32)
    Serial.print(gps.location.rawLng().negative ? "-" : "+");
    Serial.println(gps.location.rawLng().deg); // Raw longitude in whole degrees
    Serial.println(gps.location.rawLng().billionths);// ... and billionths (u16/u32)
    Serial.println(gps.date.value()); // Raw date in DDMMYY format (u32)
    Serial.println(gps.date.year()); // Year (2000+) (u16)
    Serial.println(gps.date.month()); // Month (1-12) (u8)
    Serial.println(gps.date.day()); // Day (1-31) (u8)
    Serial.println(gps.time.value()); // Raw time in HHMMSSCC format (u32)
    Serial.println(gps.time.hour()); // Hour (0-23) (u8)
    Serial.println(gps.time.minute()); // Minute (0-59) (u8)
    Serial.println(gps.time.second()); // Second (0-59) (u8)
    Serial.println(gps.time.centisecond()); // 100ths of a second (0-99) (u8)
    Serial.println(gps.speed.value()); // Raw speed in 100ths of a knot (i32)
    Serial.println(gps.speed.knots()); // Speed in knots (double)
    Serial.println(gps.speed.mph()); // Speed in miles per hour (double)
    Serial.println(gps.speed.mps()); // Speed in meters per second (double)
    Serial.println(gps.speed.kmph()); // Speed in kilometers per hour (double)
    Serial.println(gps.course.value()); // Raw course in 100ths of a degree (i32)
    Serial.println(gps.course.deg()); // Course in degrees (double)
    Serial.println(gps.altitude.value()); // Raw altitude in centimeters (i32)
    Serial.println(gps.altitude.meters()); // Altitude in meters (double)
    Serial.println(gps.altitude.miles()); // Altitude in miles (double)
    Serial.println(gps.altitude.kilometers()); // Altitude in kilometers (double)
    Serial.println(gps.altitude.feet()); // Altitude in feet (double)
    Serial.println(gps.satellites.value()); // Number of satellites in use (u32)
    Serial.println(gps.hdop.value()); // Horizontal Dim. of Precision (100ths-i32)

*/


