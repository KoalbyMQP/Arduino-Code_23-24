#include <Herkulex.h>

struct Motor{
  int hexID;
  int minPos;
  int maxPos;
  int homePos;
  int isHerk; // useless but kept for easy copy paste from ArduinoPoppy.h
};

int HERK = 1;

// Left Arm (Wrist to Shoulder)
Motor Left_Wrist_Abductor               = {26, -40, 130, 20, HERK};
Motor Left_Elbow                        = {1, -90, 120, -70, HERK};
Motor Left_Arm_Rotator                  = {2, -160, 100, -100, HERK};
Motor Left_Arm_Abductor                 = {3, -160, 20, 0, HERK};

// Right Arm (Wrist to Shoulder)
Motor Right_Wrist_Abductor              = {25, -135, 35, -20, HERK};
Motor Right_Elbow                       = {11, -70, 130, -60, HERK};
Motor Right_Arm_Rotator                 = {10, -160, 160, 25, HERK};
Motor Right_Arm_Abductor                = {6, -30, 80, 15, HERK}; // Limited by 0601 wire

// Chest/Neck (Top to Bottom, Left to Right, Front to Back)
Motor Top_Neck                          = {28, 35, 110, 90, HERK};
Motor Bottom_Neck                       = {27, -20, 160, 90, HERK};
Motor Left_Shoulder                     = {7, -160, 160, 0, HERK};
Motor Right_Shoulder                    = {15, -160, 90, -115, HERK};
Motor Front_Chest                       = {18, -155, -65, -112, HERK};
Motor Back_Chest                        = {17, -155, 20, -60, HERK};

// Pelvis (Top to Bottom, Left to Right, Front to Back
Motor Hips_Rotate_Upper_Body            = {19, -160, 120, -90, HERK};
Motor Hips_Lean_Side_To_Side            = {21, -160, 110, -90, HERK};
Motor Hips_Bend_Over                    = {22, -90, 40, 25, HERK};
Motor Left_Leg_Abductor_Front_To_Back   = {9, -20, 35, 4, HERK};
Motor Right_Leg_Abductor_Front_To_Back  = {8, -60, -5, -45, HERK};

// Left Leg (Foot to Hip)
Motor Left_Leg_Rotator                  = {14, 20, 160, 150, HERK};
Motor Left_Leg_Abductor_Side_To_Side    = {30, -130, 160, 110, HERK};
Motor Left_Knee                         = {12, -130, 0, -40, HERK};
Motor Left_Ankle                        = {13, -25, 50, 25, HERK};

// Right Leg (Foot to Hip
Motor Right_Leg_Rotator                 = {4, -160, 0, -150, HERK};
Motor Right_Leg_Abductor_Side_To_Side   = {31, -160, 110, -125, HERK};
`Motor Right_Knee                        = {20, -10, 120, 40, HERK};
`Motor Right_Ankle                       = {5, -40, 40, -20, HERK};
`
Motor motors[27] = {
  Left_Wrist_Abductor, Left_Elbow, Left_Arm_Rotator, Left_Arm_Abductor, 
  Right_Wrist_Abductor, Right_Elbow, Right_Arm_Rotator, Right_Arm_Abductor, 
  Top_Neck, Bottom_Neck, Left_Shoulder, Right_Shoulder, Front_Chest, Back_Chest, 
  Hips_Rotate_Upper_Body, Hips_Lean_Side_To_Side, Hips_Bend_Over, Left_Leg_Abductor_Front_To_Back, Right_Leg_Abductor_Front_To_Back, 
  Left_Ankle, Left_Knee, Left_Leg_Abductor_Side_To_Side, Left_Leg_Rotator, 
  Right_Ankle, Right_Knee, Right_Leg_Abductor_Side_To_Side, Right_Leg_Rotator
};

int off[50];
int angle = 0;
int index = 0;
Motor currMotor = motors[index];

//Checks which motors are attached and reading real values
void setup()  
{
  delay(2000);  //a delay to have time for serial monitor opening
  Serial.begin(115200);    // Open serial communications
  Herkulex.beginSerial1(115200); //open serial port 1 
  Serial.println("Begin");


  Herkulex.reboot(0xfe); //reboot first motor
  delay(500); 
  Herkulex.initialize(); //initialize motors
  delay(10);
  Herkulex.torqueOFF(0xfe);
  delay(10);
  Herkulex.clearError(0xfe);
 
  for (Motor m : motors) {
    Herkulex.torqueON(m.hexID);
  }

  Serial.print("Moving Motor ");
  Serial.print(currMotor.hexID);
  Serial.print(" to ");
  Serial.println(currMotor.homePos);
  delay(1000);
}

void loop() {
  // Herkulex.torqueON(currMotor.hexID);
  Herkulex.moveOneAngle(currMotor.hexID, currMotor.homePos, 1000, LED_GREEN);

  angle = Herkulex.getAngle(currMotor.hexID);

  if (off[currMotor.hexID] != 1 && (angle >= currMotor.maxPos || angle <= currMotor.minPos)) {
    Herkulex.torqueOFF(currMotor.hexID);
    Serial.print("WARNING Motor ");
    Serial.print(currMotor.hexID);
    Serial.print(" at position ");
    Serial.print(angle);
    Serial.println(" exceeded bounds and was turned off.");
    off[currMotor.hexID] = 1;
  }

  delay(10);
  Serial.println(Herkulex.getAngle(currMotor.hexID));
  Serial.println(off[currMotor.hexID]);

  if (Serial.available() != 0) {
    Serial.readString();

    index++;
    currMotor = motors[index];

    Serial.print("Moving Motor ");
    Serial.print(currMotor.hexID);
    Serial.print(" to ");
    Serial.println(currMotor.homePos);
    delay(1000);
  }
}