#include <WiFi.h>
#include <PubSubClient.h>

 
const char* ssid = "Nokia31P"; //AP for Internet Access
const char* password = "azertyaa";   


//MQTT Parameters
//For Public Free MQTT Server hivemq User and password not required: No specific user space
//MQTT Messages will be directed only based on TOPICS
const char* mqttServer = "broker.hivemq.com";//"z96e4d99.us-east-1.emqx.cloud";
const int mqttPort = 1883;

// Choisir une valeur personnalisée pour mqttClient 
// -> TOPICS auront le format mqqClient/.....
const char* mqttClient= "iotDeviceEsp32"; //Do not modify. Else, must change the topic in Amazon Lambda Function

WiFiClient espClient;
PubSubClient client(espClient);

char *TopicAction =(char*)malloc(50);

// Set LED GPIO
#define LED_Blue 12
#define LED_Green 13
#define LED_Red 16

int Wifi_ReConnect=0;
int Mqtt_ReConnect=0;


 
void setup() {
  //Topics
  strcpy (TopicAction, mqttClient);
  strcat (TopicAction,"/Action"); // --> le Topic sera ainsi mqttClient/Led
    
  // initialize the 3 digital pins (LEDS RGB).
  pinMode(LED_Blue, OUTPUT);
  pinMode(LED_Green, OUTPUT);
  pinMode(LED_Red, OUTPUT);
  
  digitalWrite(LED_Blue,HIGH);
  digitalWrite(LED_Green,HIGH);
  digitalWrite(LED_Red,HIGH);
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

 connectmqtttopic();
}



void connectmqtttopic(){

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
  Serial.println("Connecting to MQTT...");
    if (client.connect(mqttClient )) {//in this case no user, no password are needed
      Serial.println("connected");  
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);      
    }
  }
 
  if (client.subscribe(TopicAction)) {
    Serial.print ("Suscribed to Topic: ");
    Serial.println(TopicAction);
  }   
}



 void callback(char* topic, byte* payload, unsigned int length) {

 char topicpl;
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.println((char)payload[i]);
    topicpl=(char)payload[i];
  }  
  String allpayload= String( (char*) payload);
  String thepayload = allpayload.substring(0,length);
  Serial.println(thepayload);
    if (thepayload.equals("REDOF"))  {
    digitalWrite(LED_Red,HIGH);  
    Serial.println("RED LED OFF");}
    
    if (thepayload.equals("REDON"))  {
    digitalWrite(LED_Red,LOW);  }

    if (thepayload.equals("REDTO"))  {
      byte togl = digitalRead(LED_Red); 
    digitalWrite(LED_Red,!togl);  }
    
   if (thepayload.equals("BLUOF"))  {
    digitalWrite(LED_Blue,HIGH);  }
    
    if (thepayload.equals("BLUON"))  {
    digitalWrite(LED_Blue,LOW);  }

    if (thepayload.equals("BLUTO"))  {
      byte togl = digitalRead(LED_Blue); 
    digitalWrite(LED_Blue,!togl);  }
 
   
   if (thepayload.equals("GRNOF"))  {
    digitalWrite(LED_Green,HIGH);  }
    
    if (thepayload.equals("GRNON"))  {
    digitalWrite(LED_Green,LOW);  }

    if (thepayload.equals("GRNTO"))  {
      byte togl = digitalRead(LED_Green); 
    digitalWrite(LED_Green,!togl);  }
 

  if (thepayload.equals("ALLOF"))  {
    digitalWrite(LED_Green,HIGH); 
    digitalWrite(LED_Blue,HIGH);  
    digitalWrite(LED_Red,HIGH);  }
    
    if (thepayload.equals("ALLON"))  {
    digitalWrite(LED_Green,LOW);
    digitalWrite(LED_Blue,LOW);
    digitalWrite(LED_Red,LOW);  }

    if (thepayload.equals("ALLTO"))  {
      byte togl = digitalRead(LED_Green); 
    digitalWrite(LED_Green,!togl);  
          togl = digitalRead(LED_Blue); 
    digitalWrite(LED_Blue,!togl);  
          togl = digitalRead(LED_Red); 
    digitalWrite(LED_Red,!togl);  
    }
 



 
  Serial.println();
  Serial.println("-----------------------");

}



void reconnect(){
  WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
    Serial.println("WAITING WIFI CONNECTION .....");
    delay(1000);        
    }
      connectmqtttopic();
  }
    
  
 
void loop() {

  
   Serial.print ("Beginning loop with Wifi  ");
   Serial.print(Wifi_ReConnect);
   Serial.print (" Reconnexions and MQTT ");
   Serial.print (Mqtt_ReConnect);
   Serial.println(" Reconnexions" );

  if (WiFi.status() != WL_CONNECTED) {
    Wifi_ReConnect++;
    Serial.println("WIFI CONNECTION LOST ... WAITING");
    reconnect();
      }

 
  client.loop();
  
delay(2000);
}
