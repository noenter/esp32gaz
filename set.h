#include <Arduino.h>
#include <WiFi.h>        //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step1-wifi
#include <WebServer.h>   //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step2-webserver
#include <WiFiClient.h>
//#include <ESP32SSDP.h>   конфликт с библиотекой OpenTherm.h
#include <FS.h>                 //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step4-fswebserver
#include <ESPmDNS.h>
#include <Update.h>
#include <ArduinoJson.h>        //Установить из менеджера библиотек. https://arduinojson.org/
#include <TickerScheduler.h>         //https://github.com/Toshik/TickerScheduler Видео с уроком http://esp8266-arduinoide.ru/step8-timeupdate/
#include <WebSocketsServer.h>    //https://github.com/Links2004/arduinoWebSockets 
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <AHT10.h>

#include "ESP32_MailClient.h"
#define emailSenderAccount    "esp32home76@gmail.com"    
#define emailSenderPassword   "Esp32987654321"
#define emailRecipient        "serov-online@mail.ru"      //адрес на который отравлять письма
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "ESP32 У меня что-то случилось" // тема письма
SMTPData smtpData;

#define FILESYSTEM SPIFFS
#define FORMAT_FILESYSTEM false

#include <SPIFFS.h>

#include <OpenTherm.h>

const int inPin = 21;  //for Arduino, 4 for ESP8266 (D2), 21 for ESP32
const int outPin = 22; //for Arduino, 5 for ESP8266 (D1), 22 for ESP32
OpenTherm ot(inPin, outPin);


int year, mon, day;
int hour, minut, sec;

const int lLow = 36;
const int lHigh = 34;
const int lWar = 35;
int lSel = 4000;
int linfo = 0;
int lLcount = 0;
int lHcount = 0;
int lWcount = 0;
bool l1 = false;
bool l2 = false;
bool l3 = false;

const int pumppin = 27;
const int relay1pin = 32;
const int relay2pin = 33;
bool pumpstat = false;

boolean r1 = false;
boolean r2 = false;
String r1_Name = "OFF";
String r2_Name = "OFF";
String pump_Name = "OFF";

const char* host = "esp32";

const char* serverIndex =
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
   "<input type='file' name='update'>"
        "<input type='submit' value='Update'>"
    "</form>"
 "<div id='prg'>progress: 0%</div>"
 "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')"
 "},"
 "error: function (a, b, c) {"
 "}"
 "});"
 "});"
 "</script>";

WebServer HTTP(80);   // Web интерфейс для устройства
File fsUploadFile;   // Для файловой системы
TickerScheduler ts(4);   //Планировщик задач (Число задач)
WebSocketsServer webSocket = WebSocketsServer(81);   // Объект для  webSocket

Adafruit_BME280 bme1;
AHT10 myAHT1(AHT10_ADDRESS_0X39);

String configSetup = "{}"; // данные для config.setup.json
String configJson = "{}";  // данные для config.live.json
