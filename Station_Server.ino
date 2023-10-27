#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <DHT.h>

const char *ssid = "Sau_kute T2";
const char *password = "123456789";

WebServer server(80);
DHT dht(26, DHT11);

void handleRoot() {
  char msg[1500];

  snprintf(msg, 1500,
           "<html>\
<head>\
    <link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.7.2/css/all.css' integrity='sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr' crossorigin='anonymous'>\
    <title>Weather Station</title>\
    <style>\
        html {\
            font-family: Arial;\
            display: inline-block;\
            margin: 5px auto;\
            text-align: center;\
        }\
        body {\
            background-color: #aee4fc;\
            margin: 0;\
            padding: 100px;\
        }\
         h2 {\
            color: red;\
        }\
          .date-container {\
            display: inline-flex;\
            align-items: center;\
        }\
    </style>\
    <script> \
    function updateDateTime() { \
      const now = new Date(); \
      const currentDate = now.toLocaleDateString(); \
      document.querySelector('#date').textContent = currentDate; \
      const currentTime = now.toLocaleTimeString(); \
      document.querySelector('#time').textContent = currentTime; \
    } \
    setInterval(updateDateTime, 1000); \
  </script> \
</head>\
<body>\
    <h2>Weather Station</h2>\
    <div  class='date-container'>\
        <h3>Date:</h3>\
        <p id='date'></p>\
    </div>\
    <br>\
    <div class='date-container'>\
        <h3>Time:</h3>\
        <p id='time'></p>\
    </div>\
    <div>\
        <h3>Temperature:</h3>\
        <p>%.2f &#x2103;</p>\
        <h3>Humidity:</h3>\
        <p>%.2f &#x25;</p>\
    </div>\
    <div>\
        <h3>Rainfall:</h3>\
        <p>%.2f mm</p>\
         <h3>Pressure:</h3>\
        <p>%.2f hPa</p>\
    </div>\
</body>\
</html>",
           readDHTTemperature(), readDHTHumidity()
          );
  server.send(200, "text/html", msg);
}

void setup(void) {

  Serial.begin(115200);
  dht.begin();
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  server.on("/", handleRoot);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}


float readDHTTemperature() {
  // Sensor readings may also be up to 2 seconds
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return 6;
  }
  else {
    Serial.println(t);
    return t;
  }
}

float readDHTHumidity() {
  // Sensor readings may also be up to 2 seconds
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return 9;
  }
  else {
    Serial.println(h);
    return h;
  }
}
