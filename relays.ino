void relays_init(){
  pinMode(relay1pin, OUTPUT);
  pinMode(relay2pin, OUTPUT);
  String rt = "OFF";
  jsonWrite(configJson, "r1_Name", r1_Name);
  jsonWrite(configJson, "r2_Name", r2_Name);
  digitalWrite(relay1pin, HIGH);
  digitalWrite(relay2pin, HIGH);
  
}

void relay1(){
  r1 = !r1;
  //r1_Name = HTTP.arg(r1); // Получаем значение ssdp из запроса сохраняем в глобальной переменной
  //saveConfig();                 // Функция сохранения данных во Flash пока пустая
  //HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
  //Serial.println(r1);
  if (r1 == true){
  digitalWrite(relay1pin, LOW);
  r1_Name = "ON";
  SoketData ("r1_Name", r1_Name, jsonRead(configJson,"r1_Name"));
  jsonWrite(configJson, "r1_Name", r1_Name);
  //Serial.println(r1_Name);
  //servo.write(s1);
  //saveConfig();                 // Функция сохранения данных во Flash пока пустая
  HTTP.send(200, "text/plain", "OK");} // отправляем ответ о выполнении
  else{
  digitalWrite(relay1pin, HIGH);
  r1_Name = "OFF";
  SoketData ("r1_Name", r1_Name, jsonRead(configJson,"r1_Name"));
  jsonWrite(configJson, "r1_Name", r1_Name);
  //Serial.println(r1_Name);
  //servo.write(0);
  //saveConfig();                 // Функция сохранения данных во Flash пока пустая
  HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
  }}

void relay2 (){
  r2 = !r2;
  if (r2 == true){
  digitalWrite(relay2pin, LOW);
  r2_Name = "ON";
  SoketData ("r2_Name", r2_Name, jsonRead(configJson,"r2_Name"));
  jsonWrite(configJson, "r2_Name", r2_Name);
  HTTP.send(200, "text/plain", "OK");} // отправляем ответ о выполнении
  else{
  digitalWrite(relay2pin, HIGH);
  r2_Name = "OFF";
  SoketData ("r2_Name", r2_Name, jsonRead(configJson,"r2_Name"));
  jsonWrite(configJson, "r2_Name", r2_Name);
    HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
  }}
