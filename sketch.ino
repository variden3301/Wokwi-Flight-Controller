//

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

//Ultasonik sensör pin girişleri (echo ve trigger)
#define PIN_TRIG 3
#define PIN_ECHO 2


void setup() {
  Serial.begin(115200);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);


}


float distancefinder(){
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  int duration = pulseIn(PIN_ECHO, HIGH);
  return duration / 58;
}

void loop() {
  // Start a new measurement:
  int distance = distancefinder();
  Serial.println(distance);

  delay(1000);
}
