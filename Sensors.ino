// Обработка датчиков температуры AHT --- BME ---
void AHT_init() {
  myAHT1.begin();
    jsonWrite(configJson, "t1", myAHT1.readTemperature());  // отправить температуру в configJson
    jsonWrite(configJson, "h1", myAHT1.readHumidity());        // отправить влажность в configJson

    ts.add(3, 30000, [&](void*) { // Запустим задачу 0 с интервалом test
      String t1 = String(myAHT1.readTemperature(),1);
      String h1 = (String)myAHT1.readHumidity();
      SoketData ("t1", t1, jsonRead(configJson,"t1"));
      SoketData ("h1", h1, jsonRead(configJson,"h1"));
      jsonWrite(configJson, "t1", t1);   // отправить температуру в configJson
      jsonWrite(configJson, "h1", h1);         // отправить влажность в configJson      
      //Serial.print(".");
    }, nullptr, true);
  }

// -----------------  Вывод времени и даты в /config.live.json каждую секунду
void sec_init() {
  ts.add(1, 30000, [&](void*) { // Запустим задачу 1 с интервалом 1000ms
    // поместим данные для web страницы в json строку configJson
    // Будем вызывать эту функцию каждый раз при запросе /config.live.json
    // jsonWrite(строка, "ключ", значение_число); Так можно дабавить или обнавить json значение ключа в строке
    // jsonWrite(строка, "ключ", "значение_текст");
    String timeS = GetTime();
    String date = GetDate();
    SoketData ("time", timeS, jsonRead(configJson,"time"));
    SoketData ("date", date, jsonRead(configJson,"date"));
    jsonWrite(configJson, "time", timeS); // отправить время в configJson
    jsonWrite(configJson, "date", date); // отправить дату в configJson
  }, nullptr, true);
}
