/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/faq/how-to-control-speed-of-servo-motor
 */

#include <Servo.h>
#include <math.h>

#define PIN_TRIG  12  // sonar sensor TRIGGER
#define PIN_ECHO  13  // sonar sensor ECHO
#define PIN_SERVO 10  // servo motor

#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 25      // sampling interval (unit: msec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 10.0 // minimum distance to be measured (unit: mm)
#define _DIST_MAX 350.0   // maximum distance to be measured (unit: mm)
#define _DUTY_MIN 1700//90
#define _DUTY_MAX 3200//180

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL) // coefficent to convert duration to distance
Servo myServo;
unsigned long MOVING_TIME = 3000; // moving time is 3 seconds
unsigned long moveStartTime;
float _SET_DUTY = 0;
int _Distance = _DIST_MAX - _DIST_MIN ; 
unsigned long last_sampling_time;  
float dist_prev = _DIST_MAX;
float norm ;
float mapped;
float sigmoid(float distance){
  return 1.0/(1.0+exp(-distance));
}
float fuc(float distance){
  return ((_DUTY_MAX-_DUTY_MIN)/(_DIST_MAX-_DIST_MIN))*(distance-_DIST_MIN);
}
void setup() {
  pinMode(PIN_TRIG, OUTPUT);    // sonar TRIGGER
  pinMode(PIN_ECHO, INPUT);     // sonar ECHO
  digitalWrite(PIN_TRIG, LOW);  // turn-off Sonar 

  myServo.attach(PIN_SERVO);
  moveStartTime = millis(); // start moving
  myServo.write(0); // Set position
}

void loop() {
  float  dist_raw, dist_filtered;
  if (millis() < last_sampling_time + INTERVAL)
    return;
  dist_raw = USS_measure(PIN_TRIG,PIN_ECHO);
  if ((dist_raw == 0.0) || (dist_raw > _DIST_MAX)) {
      dist_filtered = dist_prev;
  } else if (dist_raw < _DIST_MIN) {
      dist_filtered = dist_prev;
  } else {    // In desired Range
      dist_filtered = dist_raw;
      dist_prev = dist_raw;
  } 
  if (dist_filtered <= 50.0) {
    myServo.writeMicroseconds(_DUTY_MIN);
    for(int i=0; i<(_DUTY_MAX-_DUTY_MIN);i++){
      myServo.writeMicroseconds(_DUTY_MIN+i);
      delay(10);
    }
  }else if (dist_filtered >= 250.0){
    myServo.writeMicroseconds(_DUTY_MAX);
  }else{
    norm = (dist_filtered - _DIST_MIN)/(_DIST_MAX-_DIST_MIN);
    mapped = sigmoid((norm-0.5)*7);
    _SET_DUTY = _DUTY_MIN+(_DUTY_MAX-_DUTY_MIN)*mapped;
    //_SET_DUTY = _DUTY_MIN + func(dist_filtered);
    myServo.writeMicroseconds(_SET_DUTY); 
  }
  last_sampling_time += INTERVAL; 
}
float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm
}



