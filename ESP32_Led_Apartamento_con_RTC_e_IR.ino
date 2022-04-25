// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
//IRSend
#include <Arduino.h>
#include "PinDefinitionsAndMore.h"//Configuración y definición de pines
#include <IRremote.hpp>
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
const char* ssid = "wifi";
const char* password = "pass";
unsigned long previousMillis = 0;
unsigned long interval = 30000;
const char* PARAM_INPUT_1 = "relay";  
const char* PARAM_INPUT_2 = "state";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
<title>Control Apartamento</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    h1 {font-size: 1.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider {background-color: #2196F3}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
     .button {
        padding: 10px 20px;
        font-size: 24px;
        text-align: center;
        outline: none;
        color: #fff;
        background-color: #2196F3;
        border: none;
        border-radius: 68px;
        //box-shadow: 0 6px #999;
        cursor: pointer;
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        -webkit-tap-highlight-color: rgba(0,0,0,0);
      }  
      .button:hover {background-color: #1579c9}
      .button:active {
        border-radius: 68px;
        background-color: #1579c9;
        //box-shadow: 0 4px #666;
        transform: translateY(2px);
      }
  </style>
</head>
<body>
  <h2>Control Apartamento</h2>
  %BUTTONPLACEHOLDER%
  <br><br></br></br>
  <h1>Radio:</h1>
  <br><br>
  <button class="button" onmousedown="toggle1Checkbox('on');" ontouchstart="toggle1Checkbox('on');" >Source</button>
  <button class="button" onmousedown="toggle2Checkbox('on2');" ontouchstart="toggle2Checkbox('on2');" >VOL+</button>
  <button class="button" onmousedown="toggle3Checkbox('on3');" ontouchstart="toggle3Checkbox('on3');" >VOL-</button>
 <br><br>
  <h1>Tv:</h1>
  <br><br>
  <button class="button" onmousedown="toggle4Checkbox('on4');" ontouchstart="toggle4Checkbox('on4');" >On/Off</button>
  <button class="button" onmousedown="toggle5Checkbox('on5');" ontouchstart="toggle5Checkbox('on5');" >Source</button>
  <br><br>
  <button class="button" onmousedown="toggle6Checkbox('on6');" ontouchstart="toggle6Checkbox('on6');" >VOL+</button>
  <button class="button" onmousedown="toggle7Checkbox('on7');" ontouchstart="toggle7Checkbox('on7');" >VOL-</button>
  <button class="button" onmousedown="toggle8Checkbox('on8');" ontouchstart="toggle8Checkbox('on8');" >Mute</button>
  <br><br>
  <h1>Led Ventana:</h1>
  <br><br>
  <button class="button" onmousedown="toggle9Checkbox('on9');" ontouchstart="toggle9Checkbox('on9');" >On/Off</button>
  <button class="button" onmousedown="toggle10Checkbox('on10');" ontouchstart="toggle10Checkbox('on10');" >Auto</button>
  <br><br>
  <button class="button" onmousedown="toggle11Checkbox('on11');" ontouchstart="toggle11Checkbox('on11');" >Rojo</button>
  <button class="button" onmousedown="toggle12Checkbox('on12');" ontouchstart="toggle12Checkbox('on12');" >Verde</button>
  <button class="button" onmousedown="toggle13Checkbox('on13');" ontouchstart="toggle13Checkbox('on13');" >Azul</button>
   <script>
   function toggle1Checkbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
   }
       function toggle2Checkbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
       }
       function toggle3Checkbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
   }
   function toggle4Checkbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
       }
       function toggle5Checkbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
       }
       function toggle6Checkbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
       }
       function toggle7Checkbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
       }
       function toggle8Checkbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
       }
       function toggle9Checkbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
       }
       function toggle10Checkbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
       }
       function toggle11Checkbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
       }
       function toggle12Checkbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
       }
       function toggle13Checkbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
       }
       
function toggleCheckbox(element) {
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
//IRSend
IrSender.begin(); // Inicializamos el emisor infrarrojo
  
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
  // Send web page to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

    // Receive an HTTP GET request radio
  server.on("/on", HTTP_GET, [] (AsyncWebServerRequest *request) {
    IrSender.sendNEC(0xB54A58A7, 32);//source
    request->send(200, "text/plain", "ok");
  });

  // Receive an HTTP GET request radio
  server.on("/on2", HTTP_GET, [] (AsyncWebServerRequest *request) {
    IrSender.sendNEC(0xB54A50AF, 32); //vol+
    request->send(200, "text/plain", "ok");
  });

  // Receive an HTTP GET request radio
  server.on("/on3", HTTP_GET, [] (AsyncWebServerRequest *request) {
    IrSender.sendNEC(0xB54AD02F, 32); //vol-
    request->send(200, "text/plain", "ok");
  });
  // Receive an HTTP GET request  tv
  server.on("/on4", HTTP_GET, [] (AsyncWebServerRequest *request) {
    IrSender.sendSAMSUNG(0xE0E040BF, 32); //power
    request->send(200, "text/plain", "ok");
  });
  // Receive an HTTP GET request  tv
  server.on("/on5", HTTP_GET, [] (AsyncWebServerRequest *request) {
    IrSender.sendSAMSUNG(0xE0E0807F, 32); //source
    request->send(200, "text/plain", "ok");
  });
   // Receive an HTTP GET request  tv
  server.on("/on6", HTTP_GET, [] (AsyncWebServerRequest *request) {
    IrSender.sendSAMSUNG(0xE0E0E01F, 32); //vol+
    request->send(200, "text/plain", "ok");
  });
 // Receive an HTTP GET request  tv
  server.on("/on7", HTTP_GET, [] (AsyncWebServerRequest *request) {
    IrSender.sendSAMSUNG(0xE0E0D02F, 32); //vol-
    request->send(200, "text/plain", "ok");
  });
// Receive an HTTP GET request  tv
  server.on("/on8", HTTP_GET, [] (AsyncWebServerRequest *request) {
    IrSender.sendSAMSUNG(0xE0E0F00F, 32); //mute
    request->send(200, "text/plain", "ok");
  });
// Receive an HTTP GET request  cinta led ventana
  server.on("/on9", HTTP_GET, [] (AsyncWebServerRequest *request) {
    IrSender.sendNEC(0xFF02FD, 32); //power
    request->send(200, "text/plain", "ok");
  });
// Receive an HTTP GET request  cinta led ventana
  server.on("/on10", HTTP_GET, [] (AsyncWebServerRequest *request) {
    IrSender.sendNEC(0xFFF00F, 32); //auto
    request->send(200, "text/plain", "ok");
  });
  // Receive an HTTP GET request  cinta led ventana
  server.on("/on11", HTTP_GET, [] (AsyncWebServerRequest *request) {
    IrSender.sendNEC(0xFF1AE5, 32); //rojo
    request->send(200, "text/plain", "ok");
  });
  // Receive an HTTP GET request  cinta led ventana
  server.on("/on12", HTTP_GET, [] (AsyncWebServerRequest *request) {
    IrSender.sendNEC(0xFF9A65, 32); //verde
    request->send(200, "text/plain", "ok");
  });
   // Receive an HTTP GET request  cinta led ventana
  server.on("/on13", HTTP_GET, [] (AsyncWebServerRequest *request) {
    IrSender.sendNEC(0xFFA25D, 32); //azul
    request->send(200, "text/plain", "ok");
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
unsigned long currentMillis = millis();
  // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >=interval)) {
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    previousMillis = currentMillis;
  }
}
