#include "header.h"
#include "BluetoothSerial.h"
#include <Ticker.h>

//#define INTERRUPT 1000
#define PIN_34 34
#define A0 34

long sumControlPl1 = 0;
long sumControlPl2 = 0;
bool filterFlag = true;

BluetoothSerial SerialBT;
String msg;
Ticker ticker;
unsigned long timer;

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_SRV_OPEN_EVT) {
    Serial.println("Client Connected");
    
    Serial.println("Client Connected has address:");

    for (int i = 0; i < 6; i++) {
      Serial.printf("%02X", param->srv_open.rem_bda[i]);
      if (i < 5) {
        Serial.print(":");
      }
    }
    Serial.println("--------------");
  }
}

void setup() {
  Serial.begin(115200);

  SerialBT.register_callback(callback);

  if (!SerialBT.begin("ESP32")) {
    Serial.println("An error occurred initializing Bluetooth");
  }
  else {
    Serial.println("Bluetooth initialized");
  }
  analogReadResolution(10);
}

void loop() {
  if (SerialBT.available()) {
    char ch = SerialBT.read();
    Serial.println(ch);
    //Serial.write(SerialBT.read());
    if(ch == 'L') localGame();
    if(ch == 'B') bossGame();
    delay(10);
  }
  
  //  Serial.println(analogRead(PIN_34));
  //  delay(100);
  Serial.write(A0);
  Serial.write(map(analogRead(A0), 0, 1023, 0 , 255));
}

void localGame(){
  SerialBT.print("The game will start in 3...");
  delay(900);
  SerialBT.print("2...");
  delay(900);
  SerialBT.print("1...");
  delay(900);
  SerialBT.print("START!");
  delay(600);
  SerialBT.print("start game");
  delay(50);
  timer = millis();

  short dataPl1 = 0;
  while (millis() - timer < 10000)
  {
    //SerialBT.print(analogRead(PIN_34));
    // dataPl1 = filterData(analogRead(PIN_34), dataPl1);
    dataPl1 = random(700,999);
    short dataPl2 = random(160,999);
    sumControlPl1 += dataPl1;
    sumControlPl2 += dataPl2;
    String sendData = String("<") + dataPl1 + "," + dataPl2 + ">";
   
    SerialBT.print(sendData);
    Serial.println(sendData);

    delay(300);
  }
  SerialBT.print("FINISH!");
  Serial.print("Контрольная сумма игрока №1: ");
  Serial.println(sumControlPl1);
  Serial.print("Контрольная сумма игрока №2: ");
  Serial.println(sumControlPl2);
  sumControlPl1 = 0;
  sumControlPl2 = 0;
}

void bossGame(){
  SerialBT.print("The game will start in 3...");
  delay(900);
  SerialBT.print("2...");
  delay(900);
  SerialBT.print("1...");
  delay(900);
  SerialBT.print("START!");
  delay(600);
  SerialBT.print("start game");
  delay(50);
  timer = millis();

  long dataPl1 = 0;
  while (millis() - timer < 20000)
  {
    //SerialBT.print(analogRead(PIN_34));
    // dataPl1 = filterData(analogRead(PIN_34), dataPl1);
    dataPl1 = random(700,999);
    sumControlPl1 += dataPl1;
    String sendData = String("<") + dataPl1 + ">";
   
    SerialBT.print(sendData);
    Serial.println(sendData);

    delay(300);
  }
  SerialBT.print("FINISH!");
  Serial.print("Контрольная сумма игрока №1: ");
  Serial.println(sumControlPl1);
  sumControlPl1 = 0;
}

short filterData(short sensorData, short oldData){
  if (sensorData < 50 || sensorData > 999)
  {
    return 999;
  }
  if (sensorData < 100)
  {
    return 160;
  }
  
  if (sensorData > 500 && sensorData < 650 && filterFlag)
  {
    filterFlag = true;
    return sensorData - 100; 
  }
  return sensorData;
}