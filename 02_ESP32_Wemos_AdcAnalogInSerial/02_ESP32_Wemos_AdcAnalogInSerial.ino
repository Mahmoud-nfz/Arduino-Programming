


// These constants won't change. They're used to give names to the pins used:
const int analogInPin = 4;  // Analog input pin that the potentiometer is attached to

int sensorValue = 0;        // value read from the pot
int resistorValue = 0;        // value output to the PWM (analog out)

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(115200);
    Serial.println("Analog Conversion launched. Turn the potentiometer");

}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  
  // map it to the range of the analog out:
  resistorValue = sensorValue*10000/4095;
  
  // print the results to the Serial Monitor:
  Serial.print("adcOutput = ");
  Serial.print(sensorValue);
  Serial.print("\t resistor = ");
  Serial.print(resistorValue);
  Serial.println("  OHM.\n");


  // wait 2000 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(2000);
}
