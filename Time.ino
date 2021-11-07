#include <time.h>               //Содержится в пакете.  Видео с уроком http://esp8266-arduinoide.ru/step8-timeupdate/
void Time_init() {
  HTTP.on("/Time", handle_Time);     // Синхронизировать время устройства по запросу вида /Time
  HTTP.on("/timezone", handle_time_zone);    // Установка времянной зоны по запросу вида http://192.168.0.101/timezone?timezone=3
  timeSynch(jsonReadtoInt(configSetup, "timezone"));
}
void timeSynch(int zone){
  if (WiFi.status() == WL_CONNECTED) {
    // Настройка соединения с NTP сервером
    configTime(zone * 3600, 0, "pool.ntp.org");
    int i = 0;
    Serial.println("\nWaiting for time");
    while (!time(nullptr) && i < 10) {
      Serial.print(".");
      i++;
      delay(1000);
    }
    Serial.println("");
    Serial.println("ITime Ready!");
    Serial.println(GetTime());
    Serial.println(GetDate());
  }
}
// Установка параметров времянной зоны по запросу вида http://192.168.0.101/timezone?timezone=3
void handle_time_zone() {
   jsonWrite(configSetup, "timezone", (int)HTTP.arg("timezone").toInt()); // Получаем значение timezone из запроса конвертируем в int сохраняем
  saveConfig();
  HTTP.send(200, "text/plain", "OK");
}

void handle_Time(){
  timeSynch(jsonReadtoInt(configSetup, "timezone"));
  HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
  }

// Получение текущего времени
String GetTime() {
  struct tm timeinfo;
  getLocalTime(&timeinfo);
  hour = timeinfo.tm_hour ;
  minut = timeinfo.tm_min;
  sec = timeinfo.tm_sec;
  String Time = String(hour) + ":" + String(minut); // Строка для результатов времени
  return Time; // Возврашаем полученное время
}

// Получение даты
String GetDate() {
 struct tm timeinfo;
  getLocalTime(&timeinfo);
  year = timeinfo.tm_year + 1900;
  mon = timeinfo.tm_mon + 1;
  day = timeinfo.tm_mday;
  String Data = String(day) + "." + String(mon) + "." + String(year); // Строка для результатов времени
 return Data; // Возврашаем полученную дату
}
