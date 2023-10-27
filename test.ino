#include <WiFi.h>
#include <WebServer.h>
#include <Arduino.h>

const char *ssid = "Sau_kute T2";
const char *password = "123456789";
WebServer server(80);


String dataToSend = "Hello, Server! \n Nhat Minh"; // Chuỗi bạn muốn in lên server

void setup() {
  Serial.begin(115200);

  // Kết nối vào mạng WiFi
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
}

void loop() {
  server.handleClient();
  // Các tác vụ khác của bạn ở đây
}
void handlePrintData() {
  String page = "<html><body><div style='text-align:center;font-size:24px;'>";
  page += dataToSend;
  page += "</div></body></html>";
  // In dữ liệu lên server
  server.send(200, "text/html", page);
}
