#include <Wire.h>
#include <ADXL345.h>
#include <Servo.h>

Servo servo1;  // create servo objects to control servos
Servo servo2;
Servo servo3;
Servo servo4;

ADXL345 adxl;  // variable adxl is an instance of the ADXL345 library

int x, y, z;
int mappedRawX, mappedRawY;
float xBaseline = 13;  // Calibration baseline for X axis
float yBaseline = -20;  // Calibration baseline for Y axis

void setup() {
  Serial.begin(9600);
  adxl.powerOn();
  servo1.attach(4);
  servo2.attach(5);
  servo3.attach(6);
  servo4.attach(7);

  // Calibration Phase
  for (int i = 0; i < 100; i++) {
    adxl.readAccel(&x, &y, &z);
    xBaseline += x;
    yBaseline += y;
    delay(10);  // Adjust delay as needed for calibration frequency
  }
  xBaseline /= 100.0;
  yBaseline /= 100.0;

  // Set initial positions of all servos to 90 degrees
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  servo4.write(90);
}

void loop() {
  adxl.readAccel(&x, &y, &z);  // read the accelerometer values and store them in variables x, y, z

  // Calibrate readings
  x -= xBaseline;
  y -= yBaseline;

  // Map calibrated accelerometer readings to servo positions for roll (servo1 and servo2) and pitch (servo3 and servo4)
  mappedRawX = map(x, -255, 255, -90, 90);  // Adjust -1000 and 1000 based on your actual accelerometer range
  mappedRawY = map(y, -255, 255, -90, 90);  // Adjust -1000 and 1000 based on your actual accelerometer range

  // Apply corrections for roll and pitch based on mappedRawX and mappedRawY
  int servo1Pos = constrain(90 + mappedRawX, 0, 180);  // Adjust servo position limits as needed
  int servo2Pos = constrain(90 - mappedRawX, 0, 180);  // Adjust servo position limits as needed
  int servo3Pos = constrain(90 + mappedRawY, 0, 180);  // Adjust servo position limits as needed
  int servo4Pos = constrain(90 - mappedRawY, 0, 180);  // Adjust servo position limits as needed

  servo1.write(servo1Pos);
  servo2.write(servo2Pos);
  servo3.write(servo3Pos);
  servo4.write(servo4Pos);

  // Print servo positions
  Serial.print("Servo 1 Position: ");
  Serial.print(servo1Pos);
  Serial.print("\t Servo 2 Position: ");
  Serial.print(servo2Pos);
  Serial.print("\t Servo 3 Position: ");
  Serial.print(servo3Pos);
  Serial.print("\t Servo 4 Position: ");
  Serial.println(servo4Pos);

  delay(100);  // Adjust delay as needed for desired control frequency
}
