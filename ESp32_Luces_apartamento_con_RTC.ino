

// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
//rtc
#include <Wire.h>
#include "Sodaq_DS3231.h"
char DiaSemana[][4] = {"Dom", "Lun", "Mar", "Mie", "Jue", "Vie", "Sab"};
// La linea fija la fecha, hora y dia de la semana, se debe suprimir la linea en la segunda carga
// Ejemplo 2018 noviembre 11, 08:00:00  dia 6-Sabado (0=Dom, 1=Lun, 2=Mar, 3=Mie, 4=Jue, 5=Vie, 6=Sab)
//DateTime dt(2022, 04, 17, 12, 47, 55, 0);


// Set to true to define Relay as Normally Open (NO)
#define RELAY_NO    true

// Set number of relays
#define NUM_RELAYS  4

//IP Estatica
IPAddress ip(192,168,0,108);     
IPAddress gateway(192,168,0,1);   
IPAddress subnet(255,255,255,0);

// Assign each GPIO to a relay
int relayGPIOs[NUM_RELAYS] = {27, 14, 26, 25};
String relayNames[NUM_RELAYS] = { "Led Baja ", "Led Alta ", "Radio", "Jardin" };


// Replace with your network credentials
const char* ssid = "nombre";
const char* password = "pass";

const char* PARAM_INPUT_1 = "relay";  
const char* PARAM_INPUT_2 = "state";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider {background-color: #2196F3}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>Control Apartamento</h2>
  %BUTTONPLACEHOLDER%
<script>function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?relay="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/update?relay="+element.id+"&state=0", true); }
  xhr.send();
}</script>
</body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    String buttons ="";
    for(int i=1; i<=NUM_RELAYS; i++){
      String relayStateValue = relayState(i);
      buttons+= "<h4>" + relayNames[i-1] + "</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"" + String(i) + "\" "+ relayStateValue +"><span class=\"slider\"></span></label>";
    }
    return buttons;
  }
  return String();
}

String relayState(int numRelay){
  if(RELAY_NO){
    if(digitalRead(relayGPIOs[numRelay-1])){
      return "";
    }
    else {
      return "checked";
    }
  }
  else {
    if(digitalRead(relayGPIOs[numRelay-1])){
      return "checked";
    }
    else {
      return "";
    }
  }
  return "";
}

void setup(){
  //rtc
  pinMode(33, OUTPUT);
   digitalWrite(33, HIGH);
    rtc.begin();
  Wire.begin();

  // La linea fija la fecha, hora y dia de la semana, se debe suprimir la linea en la segunda carga
  //rtc.setDateTime(dt);
  
  // Serial port for debugging purposes
  Serial.begin(115200);
WiFi.config(ip, gateway, subnet);//Configuración ip estática
  // Set all relays to off when the program starts - if set to Normally Open (NO), the relay is off when you set the relay to HIGH
  for(int i=1; i<=NUM_RELAYS; i++){
    pinMode(relayGPIOs[i-1], OUTPUT);
    if(RELAY_NO){
      digitalWrite(relayGPIOs[i-1], HIGH);
    }
    else{
      digitalWrite(relayGPIOs[i-1], LOW);
    }
  }
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/update?relay=<inputMessage>&state=<inputMessage2>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    String inputMessage2;
    String inputParam2;
    // GET input1 value on <ESP_IP>/update?relay=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1) & request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      inputParam2 = PARAM_INPUT_2;
      if(RELAY_NO){
        Serial.print("NO ");
        digitalWrite(relayGPIOs[inputMessage.toInt()-1], !inputMessage2.toInt());
      }
      else{
        Serial.print("NC ");
        digitalWrite(relayGPIOs[inputMessage.toInt()-1], inputMessage2.toInt());
      }
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage + inputMessage2);
    request->send(200, "text/plain", "OK");
  });
  // Start server
  server.begin();
  
}
  
void loop() {

  DateTime now = rtc.now();
  Serial.print("Hora: ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.println(now.second());

  delay(1000);

  int hora    = now.hour();
  int minuto  = now.minute();
  int segundo = now.second();
  
  //////////////Regador manana//////////////////////////

  if (hora == 7 && minuto == 0) {
    Serial.print(" Regador encendido ");
    digitalWrite(33, LOW);
  }

  if (hora == 7 && minuto >= 1) {
    Serial.print(" Regador  apagado ");
    digitalWrite(33, HIGH);
  }

////////////// Regador noche //////////////////////
   if (hora == 22 && minuto == 0) {
    Serial.print(" Regador noche encendido ");
    digitalWrite(33, LOW);
  }

  if (hora == 22 && minuto >= 1) {
    Serial.print(" Regador noche  apagado ");
    digitalWrite(33, HIGH);
  }

}
