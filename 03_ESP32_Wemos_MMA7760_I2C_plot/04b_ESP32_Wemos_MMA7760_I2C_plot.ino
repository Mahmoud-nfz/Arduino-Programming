
#include <Wire.h>

// I2C ADDRESS
#define MMA_ADDRESS   0x4C

//HTS Registers Addresses
#define MMA_MODE_REG       0x07
#define MMA_OUTX_REG       0x00
#define MMA_OUTY_REG       0x01
#define MMA_OUTZ_REG       0x02


//I2C0 PINS
#define SDA_Pin 21
#define SCL_Pin 22

TwoWire ESP32Wemo_I2C = TwoWire(0);


void writeI2cReg(uint8_t RegAddr, uint8_t Value){
    
          ESP32Wemo_I2C.beginTransmission(MMA_ADDRESS);
          ESP32Wemo_I2C.write(RegAddr);
          ESP32Wemo_I2C.write(Value);
          if (ESP32Wemo_I2C.endTransmission(true)!=0){
            Serial.println ("problem writing to I2C device");
            exit(0);
          }
   }


uint8_t readI2cReg(uint8_t RegAddr){    
          ESP32Wemo_I2C.beginTransmission(MMA_ADDRESS);
          ESP32Wemo_I2C.write(RegAddr);
           if (ESP32Wemo_I2C.endTransmission(false)){ //if !=0
                Serial.println ("Problem writing without stop"); 
                exit(0);
            }
        ESP32Wemo_I2C.requestFrom(MMA_ADDRESS,0x01);
        return(ESP32Wemo_I2C.read());
   }

 
void setup() {
  // put your setup code here, to run once:

     Serial.begin(115200);
     while (!Serial);
     
     Serial.println("App Launched");       
     
     ESP32Wemo_I2C.begin(SDA_Pin, SCL_Pin, 400000);
      
     // enable MMA7760
         writeI2cReg(MMA_MODE_REG, 0x01);
          //Serial.println("MMA7760 ENABLED");       

                   
}

void loop() {
  // put your main code here, to run repeatedly:
       
  
          //Serial.println ("conversion OK ... Reading Accelerations");

          //Read Accelerations conversion results
          int8_t outX = (readI2cReg(MMA_OUTX_REG) <<2) ;
          float accelX = (outX/4)*1.5/32;

          
          int8_t outY = (readI2cReg(MMA_OUTY_REG)<<2)  ;
          float accelY = (outY/4)*1.5/32;         
          
          int8_t outZ = (readI2cReg(MMA_OUTZ_REG)<<2) ;
          float accelZ = (outZ/4)*1.5/32;   

 
          Serial.print ("Xaxis:");Serial.print(accelX);Serial.print(" ");
          Serial.print ("Yaxis:");Serial.print(accelY);Serial.print(" ");
          Serial.print ("Zaxis:");Serial.println(accelZ);
          
          delay(100);
}
