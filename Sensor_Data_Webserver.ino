#include <WiFi.h>
#include <WebServer.h>
#include <Arduino.h>
#include <LoRa.h>


#define LORA_SS 15       // GPIO pin used for SPI chip select (SS)
#define LORA_RST 16       // GPIO pin used for Reset
#define LORA_DIO0 4      // GPIO pin used for DIO0 interrupt
const char *ssid = "TECOTECCN";
const char *password = "teccn123456";
WebServer server(80);


String dataToSend = "Hello, NhatMinh!"; // Chuỗi bạn muốn in lên server
String Label = "Welcome to Weather Station ";
String receivedData;
int count = 0;

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
  server.on("/Weather_Station", HTTP_GET, handlePrintData);
  server.begin();
  Serial.println("Wheather_Station server started");
  //................. Receive Data...................................//
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  String Sensor_Result =LoRa.readString();
}
void loop() {
  server.handleClient();
  // Receiver Data
  if (LoRa.parsePacket()) {
    receivedData = LoRa.readString(); // Lưu trữ dữ liệu trong biến receivedData
    Serial.println("Received data from LoRa: " + receivedData);
  } else {
    receivedData = "No Data Received From Sensor. Please check your connections again";
    Serial.println("No Data from LoRa.");
  }
  // Gọi hàm cập nhật dữ liệu trên trang web
    updateLoRaData(receivedData);
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
  // Update Result From Sensor
  page += "<p class='center-text'>Data From Sensor: ";
  page += "<p class='bold-text' style='text-align:center;font-size:18px;color: blue;'>" + receivedData + "</p>"; 
  //Reset page to update data
  page += "<script>";
  page += "setTimeout(function() {";
  page += "  location.reload();"; // Cập nhật trang sau 5 giây
  page += "}, 5000);"; // Thời gian cập nhật (5 giây)
  page += "</script>";
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
  page += "</script>";
    // Ket thuc Config Server
  page += "</div></body></html>";
  // In dữ liệu lên server
  server.send(200, "text/html", page);
}
//Func updateLoRaData
void updateLoRaData(String data) {
}
