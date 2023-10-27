#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>

#define DHTPIN 3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

Adafruit_BMP085 bmp;
int rainSensorPin = A0;


void setup() {
  Serial.begin(9600);
  dht.begin();
  bmp.begin();
  pinMode(rainSensorPin, INPUT);
  while (!Serial);
  Serial.println("LoRa Sender");
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}
void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  float pressure = bmp.readPressure() / 100.0F;

  int rainValue = analogRead(rainSensorPin);
  Serial.print("Sending packet: ");
  Serial.println();
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.print("Rain Sensor Value: ");
  if(rainValue >1000){
    Serial.println("Trời ko có mưa!!");
    
  }if(rainValue <700 && rainValue >350){
    Serial.println("Trời sắp mưa!!");
  }
  if(rainValue < 350){
    Serial.println("Trời mưa");
  }
  Serial.println(rainValue);
  Serial.println("-------------------");
  // send packet
  LoRa.beginPacket();
  LoRa.print("Temperature: ");
  LoRa.print(temperature);
  LoRa.println(" °C");

  LoRa.print("Humidity: ");
  LoRa.print(humidity);
  LoRa.println(" %");

  LoRa.print("Pressure: ");
  LoRa.print(pressure);
  LoRa.println(" hPa");

  LoRa.print("Rain Sensor Value: ");
  LoRa.println(rainValue);
  
  LoRa.endPacket();

  delay(2000);
}
