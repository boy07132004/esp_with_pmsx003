#include <PMserial.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <AsyncMqttClient.h>

#define MQTT_PORT 1883
#define MQTT_HOST IPAddress(192,168,0,100)

AsyncMqttClient mqttClient;
const uint8_t PMS_RX = D7, PMS_TX = D8;
SerialPM pms(PMSA003, PMS_RX, PMS_TX); // PMSx003, RX, TX

void setup()
{
  WiFi.begin("SSID","PASSWORD"); // Your wifi ssid and password
  Serial.begin(9600);
  while (!WiFi.isConnected()) delay(100);
  Serial.println(F("Booted"));
  pms.init();
  mqttClient.setServer(MQTT_HOST,MQTT_PORT);
  mqttClient.connect();
}

void loop()
{
  char msg[100];  
  while(true){
    pms.read();
    if (pms)
    {
      sprintf(msg,"PM1.0 %2d, PM2.5 %2d, PM10 %2d, N0.3 %4d, N0.5 %3d, N1.0 %2d, N2.5 %2d, N5.0 %2d, N10 %2d\n",
      pms.pm01, pms.pm25, pms.pm10, pms.n0p3, pms.n0p5, pms.n1p0, pms.n2p5, pms.n5p0, pms.n10p0);
    }
    mqttClient.publish("rand2",2,true,msg);
    delay(10000);
  }
}
