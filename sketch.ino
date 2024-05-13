//MPU Gyroscope Libraries
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

//Ultasonik sensör pin girişleri (echo ve trigger)
#define PIN_TRIG 3
#define PIN_ECHO 2


//Fefine the mpu as mpu
Adafruit_MPU6050 mpu;
#define MPU_LED 25

//MPU5060 Event
sensors_event_t event;

//First stepper motor test pins
#define stepPin 31
#define dirPin 33

int currentStep = 0;

void setup() {
  //Communication rate
  Serial.begin(115200);
  //Ultrasonic setup
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  //stepper test
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  // Try to initialize the mpu sensor
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 initialized succesfully");
  pinMode(MPU_LED, OUTPUT);
  digitalWrite(MPU_LED, HIGH);
  
  
  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  //Stepper alignment
  digitalWrite(dirPin,HIGH);
  digitalWrite(stepPin,HIGH); 
  delayMicroseconds(2000); 
  digitalWrite(stepPin,LOW); 
  delayMicroseconds(2000);
  digitalWrite(stepPin,HIGH); 
  delayMicroseconds(2000); 
  digitalWrite(stepPin,LOW); 
  delayMicroseconds(2000);


}




float distancefinder(){
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  int duration = pulseIn(PIN_ECHO, HIGH);
  return duration / 58;
}

int stepperTest(int distanceData){
  
  // Makes 200 pulses for making one full cycle rotation
  
  int stepCount = 0;
  Serial.print("Distancedata: ");
  Serial.println(distanceData);
  Serial.print("CurrentStep: ");
  Serial.println(currentStep);
    if(currentStep < distanceData){
      Serial.println("currentStep < distanceData");
      digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
      for(int x = 0; x < distanceData; x++) {
        digitalWrite(stepPin,HIGH); 
        delayMicroseconds(2000); 
        digitalWrite(stepPin,LOW); 
        delayMicroseconds(2000); 
        stepCount++;
      } 
      currentStep = currentStep + stepCount;
      stepCount = 0;
  }
  else if(currentStep > distanceData)
  {
    
      digitalWrite(dirPin,LOW); //Changes the rotations direction
      for(int x = 0;  x < currentStep - distanceData; x++) {
      digitalWrite(stepPin,HIGH);
      delayMicroseconds(2000);
      digitalWrite(stepPin,LOW);
      delayMicroseconds(2000);
      stepCount++;
      } 
      currentStep = currentStep - stepCount;
      stepCount = 0;

  }

    
    delay(1000); // One second delay
  return currentStep;
}

void loop() {
  // Start a new measurement:
  int distance = distancefinder();
  Serial.println(distance);
  //Get sensor event for acceleration, rotation and temperature
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");
  stepperTest(distance);
  

  delay(1000);
}
