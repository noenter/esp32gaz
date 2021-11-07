#include "set.h"  //подключение баблиотек в отдельном файле set.h

void ICACHE_RAM_ATTR handleInterrupt() {
  ot.handleInterrupt();
}
void setup() {

  Serial.begin(115200);
  delay(10);
  ot.begin(handleInterrupt);
  Serial.println("esp32Tipografskaya");
  Serial.println("Start 4-FS");
  FS_init(); //Запускаем файловую систему
  Serial.println("Step7-FileConfig");
  configSetup = readFile("config.json", 4096);
  jsonWrite(configJson, "SSDP", jsonRead(configSetup, "SSDP"));
  Serial.println(configSetup);
  Serial.println("Start 1-WIFI");
  WIFIinit();   //Запускаем WIFI
  MDNS.begin(host);  //http://esp32.local
  Serial.println("mDNS responder started");
  Serial.println("Start 8-Time");
  Time_init();       // Получаем время из сети
  //  Serial.println("Start 3-SSDP");
  //  SSDP_init();       //Настраиваем и запускаем SSDP интерфейс
  Serial.println("Start 2-WebServer");
  HTTP_init();      //Настраиваем и запускаем HTTP интерфейс
  //Настраиваем и запускаем webSoket интерфейс
  Serial.println("Start 14-webSoket_init");
  webSoket_init();   //Настраиваем и запускаем webSoket интерфейс
  Serial.println("Start 13-sec_init");
  sec_init();       // Включаем Вывод времени и даты каждую секунду
//  Serial.println("Start 13-BME280(0X77)");
//  BME_init();       // Инициализация датчиков BME280
  Serial.println("Start 15-AHT10");
  AHT_init();       // Инициализация датчиков AHT10
//  Serial.println("Start 16-Датчики уровней");
//  level_init();     // Инициализация датчиков уровня воды
  Serial.println("Start 17-SMPT Отправки оповещения на почту");
  SMTP_Init();      // Подготовка сообщения на почту
  Serial.println("Start 18-Подготовка реле");
  relays_init();    // Подготовка реле к работе
}

void loop() {

  //Set/Get Boiler Status
  bool enableCentralHeating = true;
  bool enableHotWater = true;
  bool enableCooling = false;
  unsigned long response = ot.setBoilerStatus(enableCentralHeating, enableHotWater, enableCooling);
  OpenThermResponseStatus responseStatus = ot.getLastResponseStatus();
  if (responseStatus == OpenThermResponseStatus::SUCCESS) {
    Serial.println("Central Heating: " + String(ot.isCentralHeatingActive(response) ? "on" : "off"));
    Serial.println("Hot Water: " + String(ot.isHotWaterActive(response) ? "on" : "off"));
    Serial.println("Flame: " + String(ot.isFlameOn(response) ? "on" : "off"));
  }
  if (responseStatus == OpenThermResponseStatus::NONE) {
    Serial.println("Error: OpenTherm is not initialized");
  }
  else if (responseStatus == OpenThermResponseStatus::INVALID) {
    Serial.println("Error: Invalid response " + String(response, HEX));
  }
  else if (responseStatus == OpenThermResponseStatus::TIMEOUT) {
    Serial.println("Error: Response timeout");
  }

  //Set Boiler Temperature to 64 degrees C
  ot.setBoilerTemperature(64);

  //Get Boiler Temperature
  float ch_temperature = ot.getBoilerTemperature();
  Serial.println("CH temperature is " + String(ch_temperature) + " degrees C");

  //Set DHW setpoint to 40 degrees C
  ot.setDHWSetpoint(40);

  //Get DHW Temperature
  float dhw_temperature = ot.getDHWTemperature();
  Serial.println("DHW temperature is " + String(dhw_temperature) + " degrees C");

  Serial.println();

  ts.update(); //планировщик задач
  HTTP.handleClient(); // Работа Web сервера
  yield();
  webSocket.loop(); // Работа webSocket
}
