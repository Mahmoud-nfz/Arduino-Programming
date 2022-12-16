

// Set LED GPIO
#define LED_Blue 12
#define LED_Green 13
#define LED_Red 16

#define Button_Center 17
 
void setup(){
  // put your setup code here, to run once:
  Serial.begin(115200);

  // initialize the 3 digital pins (LEDS RGB).
  pinMode(LED_Blue, OUTPUT);
  pinMode(LED_Green, OUTPUT);
  pinMode(LED_Red, OUTPUT);
  pinMode (Button_Center, INPUT);

  //THE Three LEDS OFF. (For the RGB Leds: HIGH=OFF, LOW=ON)
  digitalWrite(LED_Blue, HIGH);   
  digitalWrite(LED_Green, HIGH);  
  digitalWrite(LED_Red, HIGH);   
  
  Serial.println("Configuration complete. RGB LEDS OFF. Press Button!!");
    delay(1000);//wait 1 sec
 
}

void SetLeds_OneByOne(void)
{
    //Blue Led ON For 1 sec
    digitalWrite(LED_Blue, LOW);
    delay(1000);

    //Red Led ON for 1 sec
    digitalWrite(LED_Blue, HIGH);
    digitalWrite(LED_Red, LOW);
    delay(1000);

    //Green led ON for 1 sec
    digitalWrite(LED_Red, HIGH);
    digitalWrite(LED_Green, LOW);
    delay(1000);

    //All Leds ON/ White for 1 sec
    digitalWrite(LED_Blue, LOW);   
    digitalWrite(LED_Red, LOW);   
    delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
 
      if(digitalRead(Button_Center)){
      Serial.println("Button Pressed -> Leds ON = White");
      SetLeds_OneByOne();
      } else  {
      //Serial.println("Button Released-> Leds OFF");
      digitalWrite(LED_Blue, HIGH);   
      digitalWrite(LED_Green, HIGH);  
      digitalWrite(LED_Red, HIGH);   
      }
      delay(500);
}
