
#include "ESPAsyncWebServer.h"

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include "FS.h"
#elif defined ESP32
#include <WiFi.h>
#include "SPIFFS.h"
#endif



/* Set Access Point Parameters. */
const char *ssid1 = "smartdevice";  //Choisir un SSID pour le ESP32 fonctionnant comme AP
//const char *password1 = APPSK;*/withou PWD

//Set http user/pwd
const char* http_username = "usr";
const char* http_password = "pwd";


// Replace with your network credentials
const char* ssid = "smartdevice";
const char* password = "smartdevice";


// Set LED GPIO
const int GreenLed = 12;//12, 13, 16 for Wemos
const int BlueLed = 13;
// Stores LED state
String ledState;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Replaces placeholder with LED state value
String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(GreenLed)){
      ledState = "GREEN";
    }
    else{
      ledState = "BLUE";
    }
    Serial.println(ledState);
    return ledState;
  }
  return String();
}
 
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(GreenLed, OUTPUT);
    pinMode(BlueLed, OUTPUT);
    
    digitalWrite(GreenLed, LOW);   
    digitalWrite(BlueLed, HIGH);     


  Serial.println("Application launched");

  // Initialize SPIFFS
    bool ok = SPIFFS.begin();

  if(!ok){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  nodemcu_WifiClient();

  //nodemcu_AP();
  


  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        if(request->authenticate(http_username, http_password)==false)
      return request->requestAuthentication();
      else
    request->send(SPIFFS, "/index.html", "text/html", false, processor);//String()
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Route to set GPIO 12 to H (Green Led on)
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(GreenLed, HIGH);  
    digitalWrite(BlueLed, LOW);    
    request->send(SPIFFS, "/index.html", "text/html", false, processor);//String()
  });
  
  // Route to set GPIO to LOW
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(GreenLed, LOW);   
    digitalWrite(BlueLed, HIGH);     
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Start server
  server.begin();
}

void nodemcu_AP(void){
  //Setting Device as Access Point
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid1);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
}

void nodemcu_WifiClient(void){
  //Connecting To WIFI Access point
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());
  
    digitalWrite(GreenLed, HIGH);   
    digitalWrite(BlueLed, LOW);     

}

 
void loop(){
  
}
