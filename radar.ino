#include <Servo.h>
#include <NewPing.h>

#define servoMin 600
#define servoMax 2400
#define servoStep 225
#define numberOfAngles 2 * (servoMax - servoMin) / servoStep
#define numberOfReadings 3

Servo radarServo;
int servoPos = servoMin;
int servoDirection = 1;
long lastServoMove = 0;

#define SONAR_NUM     2 // Number or sensors.
#define MAX_DISTANCE 400 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 30 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
//unsigned int cm[SONAR_NUM];         // Where the ping distances are stored.
//double degree[SONAR_NUM];
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.

long readingTime[numberOfAngles][numberOfReadings];
double readingValues[numberOfAngles][numberOfReadings];
double readingDegree[numberOfAngles];
long resultTime[numberOfAngles];
int readingIndex = 0;
int testIndex = 0;

unsigned long timeReset = millis();

NewPing sonar[SONAR_NUM] = {     // Sensor object array.
  NewPing(48, 49, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(50, 51, MAX_DISTANCE)
};

void setup() {
  Serial.begin(9600);
  radarServo.attach(2);
  //pingTimer[0] = millis() + 75;           // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  //for (uint8_t i = 1; i < SONAR_NUM; i++) // Set the starting time for each sensor.
    //pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
    
  //Serial.println(numberOfAngles);
    
  for (int i = 0; i < numberOfAngles; i++) {
    readingDegree[i] = (i * servoStep) / ((servoMax - servoMin) / 180.0);
    //Serial.println(readingDegree[i]);
  }
}

void loop() {
  //for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through all the sensors.
    //if (millis() >= pingTimer[i]) {         // Is it this sensor's time to ping?
      //pingTimer[i] += PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
      //if (i == 0 && currentSensor == SONAR_NUM - 1) oneSensorCycle(); // Sensor ping cycle complete, do something with the results.
      //sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
      //currentSensor = i;                          // Sensor being accessed.
      //cm[currentSensor] = 0;                      // Make distance zero in case there's no ping echo for this sensor.
      //sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
    //}
  //}
  //delay(1000);
  //if (millis() - timeReset > 500) {
    if (servoPos >= servoMax && servoDirection > 0) {
      servoDirection *= -1;
    } else if (servoPos <= servoMin && servoDirection < 0) {
      servoDirection *= -1;
    } 
    servoPos += (servoStep * servoDirection);
  
    radarServo.writeMicroseconds(servoPos);
    //timeReset = millis();
  //}
  delay(250);
  
  //for (int i = 0; i < numberOfAngles; i++) {
    //readingDegree[i] = (i * servoStep) / ((servoMax - servoMin) / 180.0);
    //Serial.println(readingDegree[i]);
  //}
  
  for (int i = 0; i < SONAR_NUM; i++) {
    unsigned int duration = sonar[i].ping_median(3);
    double distance;
    if (duration == 0){
      distance = 4;
    } else {
      distance = ((duration / 2) / 2910.0) + 0.01;
    }
    report(i, distance);
  }
  
  //double distance1, distance2;
    
  //unsigned int duration1 = sonar[0].ping_median(3);
  //unsigned int duration1 = sonar[0].ping_median(3);
  //if (duration1 == 0){
    //distance1 = 4;
  //} else {
    //distance1 = ((duration1/2) / 2910.0)+0.01;
  //}
  //Serial.print(degree);
  //Serial.print("-");
  //Serial.println(distance1);
  
  //unsigned int duration2 = sonar[1].ping_median(3);
    
  //if (duration2 == 0){
    //distance2 = 4;
  //} else {
    //distance2 = ((duration2/2) / 2910.0)+0.01;
  //}
  //double degree = (servoPos - servoMin) / ((servoMax - servoMin) / 180.0);
  
  
  
  //Serial.print(degree + 180);
  //Serial.print("-");
  //Serial.println(distance2);
  readingIndex++;
  if (readingIndex == numberOfReadings) {
    readingIndex = 0;
  } else {
    
  }
  Serial.println(readingIndex);
}

void echoCheck() { // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer()) {
    //cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
    //double degree = (millis() - timeReset) / 700.0 * 180.0;
    double degree = (servoPos - servoMin) / ((servoMax - servoMin) / 180.0);
    if (currentSensor == 1) {
      degree += 180;
    }
    //if (servoDirection < 0) {
    //  degree = 180 - degree;
    //}
    //degree[currentSensor] = degreeLocal;
    double distance = 0.0;
    distance = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM / 100.0;
    Serial.print(currentSensor);
    Serial.print(",");
    Serial.print(servoDirection);
    Serial.print(",");
    Serial.print(degree);
    Serial.print(",");
    Serial.println(distance);
  }
}

void report(int currentSensor, double distance) {
  //double degree = (servoPos - servoMin) / ((servoMax - servoMin) / 180.0);
  int degreeIndex = (servoPos - servoMin) / servoStep;
  
  if (currentSensor == 1) {
    degreeIndex += numberOfAngles / 2;
  }
  if (degreeIndex == numberOfAngles) {
    degreeIndex = 0;
  }
    //if (servoDirection < 0) {
    //  degree = 180 - degree;
    //}
    //degree[currentSensor] = degreeLocal;
    //double distance = 0.0;
    //distance = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM / 100.0
    
  
  
  
  
  readingTime[degreeIndex][readingIndex] = millis();
  readingValues[degreeIndex][readingIndex] = distance;
 
  //resultTime[numberOfAngles];
  
  
  Serial.print(currentSensor);
  Serial.print(",");
  Serial.print(servoDirection);
  Serial.print(",");
  Serial.print(readingDegree[degreeIndex]);
  Serial.print(",");
  Serial.println(distance);
}

//void oneSensorCycle() {
//  for (int i = 0; i < SONAR_NUM; i++) {
//    Serial.print(i);
//    Serial.print("-");
//    Serial.print(degree[i]);
//    Serial.print("-");
//    Serial.println(cm[i] / 100.0);
//  }
//}
