#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <motor_control.h>
#include <Kalman.h>

Kalman kalmanFilter;  

/* This driver reads raw data from the BNO055

   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3V DC
   Connect GROUND to common ground
   History
   =======
   2015/MAR/03  - First release (KTOWN)
*/

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28, &Wire);
MotorControl stepper = MotorControl(2,5,200);

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{
  Serial.begin(115200);
  while (!Serial) delay(10);  // wait for serial port to open!

  // Serial.println("Orientation Sensor Raw Data Test"); Serial.println("");

  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    // Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);

  }

  delay(1000);

  /* Display the current temperature */
  int8_t temp = bno.getTemp();
  // Serial.print("Current Temperature: ");
  // Serial.print(temp);
  // Serial.println(" C");
  // Serial.println("");
  stepper.setup();
  stepper.setSpeed(5);
  bno.setExtCrystalUse(true);

  // Serial.println("Calibration status values: 0=uncalibrated, 3=fully calibrated");


}

//Ignore this :)

double kalmanFilterF(double z){
  static const double R = 0.001; // measurement noise covariance
  static const double HT = 1.00; //measurement function
  //static double Q = 10; // motion noise
  static double P = 1.0; // uncertainty Covariance
  static double X_hat = 0.0; // initial estimated state 
  static double K = 0.0; // Kalman Gain
  static double I = 1.0; //identity matrix

  ///////// (P × HT)/((H×P×HT)+R)          
  K = (P*HT)/((HT*P*HT)+R); //Kalman Gain
  X_hat = X_hat + K*(z-HT*X_hat); //update the estimate with measurement

  P = (I - K*HT) * P; // updating uncertainty covariance

  return X_hat;
}




/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void)
{
 //  stepper.spinForRotation(1);

 // Serial.print(" Accel=");
 // Serial.print(accel, DEC);

  float prevAngle = 0.0f;
  unsigned long prevTime = millis();

  //calibration  time :)
 //stepper.spinForRotation(0);
  //delay(10000);
  int delayTime = BNO055_SAMPLERATE_DELAY_MS;
  //int delayTime = 3000;

  while(true){
    imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);

    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

    unsigned long currentTime = millis();
    float dt = (currentTime - prevTime) / 1000.0;  //  time since the calculation was last performe
    float accelX = accel.x();

    float kalmanX = kalmanFilter.getAngle(euler.x(), accelX, currentTime);

    /* Display the floating point data */
    stepper.spinForRotation(.5);


    /* Display the filtered data */
    Serial.print("IMU_X: ");
    Serial.print(euler.x());
    Serial.print(" FILTERED_X: ");
    Serial.print(kalmanX);
    Serial.println("\t\t");

    // Update previous state
    prevAngle = euler.x();
    prevTime = currentTime;
    
    delay(delayTime);


    euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

    currentTime = millis();
    dt = (currentTime - prevTime) / 1000.0;  // Convert to seconds
    accelX = accel.x(); 
    kalmanX = kalmanFilter.getAngle(euler.x(), accelX, currentTime);

    /* Display the floating point data */
    stepper.spinForRotation(-.5);

    /* Display the filtered data */
    Serial.print("IMU_X: ");
    Serial.print(euler.x());
    Serial.print(" FILTERED_X: ");
    Serial.print(kalmanX);
    Serial.println("\t\t");

    // Update previous state
    prevAngle = euler.x();
    prevTime = currentTime;
    delay(delayTime);




}
}

 