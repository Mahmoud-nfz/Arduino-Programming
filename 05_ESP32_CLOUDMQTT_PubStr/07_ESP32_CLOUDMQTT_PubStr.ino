#include <WiFi.h>
#include <PubSubClient.h>


 // SSID and password to connect to Internet
const char* ssid = ........;
const char* password = .......;   

//MQTT Parameters
//For Public Free MQTT Server hivemq User and password not required: No specific user space
//MQTT Messages will be directed only based on TOPICS

const char* mqttServer = "broker.hivemq.com";//"z96e4d99.us-east-1.emqx.cloud";
const int mqttPort = 1883;


// Choisir une valeur personnalisée pour mqttClient 
// -> TOPICS auront le format mqqClient/.....
const char* mqttClient= .........;";

WiFiClient espClient;
PubSubClient client(espClient);

//const char* mqttUser = "......";
//const char* mqttPassword = "....";


char *TopicControl =(char*)malloc(50);
char *TopicStatus =(char*)malloc(50);


const int led = 12; //Blue LED
int ledState = 0;
 
int Wifi_ReConnect=0;
int Mqtt_ReConnect=0;



 
void setup() {
  //All leds OFF
    pinMode(led, OUTPUT);
    digitalWrite(led, HIGH);
    pinMode(13, OUTPUT);  digitalWrite(13, HIGH);
    pinMode(16, OUTPUT);  digitalWrite(16, HIGH);
  
  //Topics
    strcpy (TopicControl, mqttClient);
    strcat (TopicControl,"/Led"); // --> le Topic sera ainsi mqttClient/Led
    
    strcpy (TopicStatus, mqttClient);
    strcat (TopicStatus,"/Status"); // --> le Topic sera ainsi mqttClient/Status




  
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
 
  if (client.subscribe(TopicControl)) {
    Serial.print ("Suscribed to Topic: ");Serial.println(TopicControl);
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
  if (topicpl=='0'){   
  digitalWrite(led, HIGH); 
  ledState=0;
  Serial.println("LED OFF");
  }
  else{
  digitalWrite(led, LOW);
    ledState=1;
  Serial.println ("LED ON");
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

  /*
   Serial.print ("Beginning loop with Wifi  ");   
   Serial.print(Wifi_ReConnect);
   Serial.print (" Reconnexions and MQTT ");  
   Serial.print (Mqtt_ReConnect);
   Serial.println(" Reconnexions" );*/

  if (WiFi.status() != WL_CONNECTED) {
    Wifi_ReConnect++;
    Serial.println("WIFI CONNECTION LOST ... WAITING");
    reconnect();
      }

    char buff[2];
    snprintf (buff, 2, "%d", ledState);
    client.publish(TopicStatus, buff);

 
  client.loop();
delay(2000);
}
