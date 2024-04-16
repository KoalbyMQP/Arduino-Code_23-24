#include <Herkulex.h>

//Checks which motors are attached and reading real values
void setup()  
{
  delay(2000);  //a delay to have time for serial monitor opening
  Serial.begin(115200);    // Open serial communications
  Serial.println("Begin");
  Herkulex.beginSerial1(115200); //open serial port 1  

  for(int i =0;i<0xFE;i++)
    Herkulex.reboot(i); //reboot first motor

  delay(500); 
  Herkulex.initialize(); //initialize motors
}

void loop(){
  int count = 0;
  for(int i =0;i<0xFE;i++){
    int angle = Herkulex.getAngle(6, false); // Assume 0201, just see if the motor is hooked up
    if(Herkulex.stat(6) ==0){
      Serial.print("Got servo ");
      Serial.print(6);
      Serial.print(" Angle: ");
      Serial.println(angle);
      // Herkulex.clearError(i);
      Herkulex.setLed(6,LED_BLUE);
      Herkulex.torqueOFF(6);
      count++;
    }
  }
  Serial.print("Found ");
  Serial.print(count);
  Serial.println(" motors.");
  Serial.println("Cycle");
  delay(1000);
}