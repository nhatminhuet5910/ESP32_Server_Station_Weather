#include <WiFi.h>
#include <WebServer.h>
#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
 
#define LORA_SS 15       // GPIO pin used for SPI chip select (SS)
#define LORA_RST 16       // GPIO pin used for Reset
#define LORA_DIO0 4      // GPIO pin used for DIO0 interrupt


const char *ssid = "Sau_kute T2";
const char *password = "123456789";
WebServer server(80);



String dataToSend = "Hello, NhatMinh!"; // Chuỗi bạn muốn in lên server
String Label = "Welcome to Wheather Station ";

void setup() {
  Serial.begin(9600);
//................. Connect to WiFi...................................//
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  }
  Serial.println("Connected to WiFi");
// Định nghĩa route để in dữ liệu lên server
  server.on("/Wheather_Station", HTTP_GET, handlePrintData);
  server.begin();
  Serial.println("Wheather_Station server started");
  //................. Receive Data...................................//
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
  server.handleClient();
  // Các tác vụ khác của bạn ở đây
}
void onReceive(int packetSize) {
  if (packetSize == 0) 
    return;
  // read packet
  while (LoRa.available()) {
    String message = LoRa.readString();
    Serial.println("Received message: ");
    Serial.println(message);
  }
}
void handlePrintData() {
  // Cau hinh webserver
  String page = "<html><head><style>";
  page +="<title>Weather Station</title>";
  page +="<link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.7.2/css/all.css' integrity='sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr' crossorigin='anonymous'>";
  page += "body {background-color: #aee4fc;}";
  page += ".content {background-color: #b4ddf0; padding: 40px; border: 1px solid #cccccc; border-radius: 5px; margin: 20px;}";
  page += ".content2 {background-color: #ffffff; padding: 40px; border: 1px solid #cccccc; border-radius: 5px; margin: 20px;}";
  page += ".bold-text {font-weight: bold; font-size: 24px;}";
  page += ".center-text {text-align: center;}";
  page += "</style></head><body>";
  page += "<p>";
  page += "<i class='fas fa-thermometer-half' style='color: #ca3517;'></i>";
  page += "<p>";
  page += "<div class='content'>";
  page += "<p class='bold-text' style='font-size: 5.0rem;text-align:center;font-size:24px;color: red;'>" + Label + "</p>"; 
  page += "<p class='center-text'>Current Date: <span id='Date'></span></p>";
  page += "<p class='center-text'>Current Time: <span id='Time'></span></p>";
   // Hien thi thoi gian Date
  page += "<script>";
  page += "function updateDateTime() {";
  page += "var now = new Date();";
  page += "var timeElement = document.getElementById('Time');";
  page += "var dateElement = document.getElementById('Date');";
  page += "timeElement.innerHTML = now.toLocaleTimeString();";
  page += "dateElement.innerHTML = now.toLocaleDateString();";
  page += "}";
  page += "setInterval(updateDateTime, 1000);";
  page += "</script>";
  page += "</div>";
  // Result From Sensor
  //String Sensor_Result = LoRa.readString();
  page += "<p class='center-text'>Calculation Result: " + dataToSend + "</p>";
    // Ket thuc Config Server
  page += "</div></body></html>";
  // In dữ liệu lên server
  server.send(200, "text/html", page);
}
