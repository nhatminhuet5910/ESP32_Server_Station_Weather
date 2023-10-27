#include <SPI.h>
#include <LoRa.h>
 
#define LORA_SS 15       // GPIO pin used for SPI chip select (SS)
#define LORA_RST 16       // GPIO pin used for Reset
#define LORA_DIO0 4      // GPIO pin used for DIO0 interrupt

void setup() {
  Serial.begin(9600);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  // register the receive callback
  LoRa.onReceive(onReceive); // Call func onReceive when Receive data
  LoRa.receive(); // Waiting Data from Sender
}
void loop() {
  // do nothing
}
void onReceive(int packetSize) {
  Serial.print("Received packet '");
  if (packetSize == 0) return;
  // read packet
  while (LoRa.available()) {
    String message = LoRa.readString();

    Serial.print("Received message: ");
    Serial.println(message);
  }
  // print RSSI of packet
  Serial.print("' with RSSI ");
  Serial.println(LoRa.packetRssi());
}
