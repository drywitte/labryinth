#include <Servo.h>

Servo myservo;  // create servo object to control a servo

// Reading
int sensorPin = A0;
int sensorValue;

// Writing
int servoPin = 9;
int servoValue;

// Servo callibration
int stopped = 88;
int clockwise = 85;
int counterclockwise = 90;

// Kernelize values
int window_size = 5;
int window[5];
int window_filled = 0;

void setup() {
  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
}

int getMovementDirection() {
  // Read EMG
  int movement;
  sensorValue = analogRead(sensorPin);
  if (sensorValue < 220) {
    Serial.println("no flex " + String(sensorValue));
    movement = stopped;
  }
  else if (sensorValue < 400) {
    Serial.println("half flex " + String(sensorValue));
    movement = counterclockwise;
  }
  else {
    Serial.println("full flex " + String(sensorValue));
    movement = clockwise;
  }
  return movement;
}

int kernelize(int curr_movement) {
  
  // Fill window
  if (window_filled < window_size) {
    // Fill window
    window[window_filled] = curr_movement;
    window_filled++;
    return -1;
  }

  // Updating window values
  for (int i = 0; i < window_size-1; ++i) {
    window[i] = window[i+1];
  }
  window[window_size-1] = curr_movement;

  // Count to determine majority of window
  int stopped_count = 0;
  int clockwise_count = 0;
  int counterclockwise_count = 0;
  for (int i = 0; i < window_size; ++i) {
    if (window[i] == stopped) {
      stopped_count++;
    }
    else if (window[i] == clockwise) {
      clockwise_count++;
    }
    else {
      counterclockwise_count++;
    }
  }

  // Determine majority
  int majority = -1;
  if (stopped_count > majority) {
    majority = stopped_count;
    movement = stopped;
  }
  if (clockwise_count > majority) {
    majority = clockwise_count;
    movement = clockwise;
  }
  if (counterclockwise_count > majority) {
    majority = counterclockwise_count;
    movement = counterclockwise;
  }

  return movement;
}

void loop() {

  // Get current movement
  int curr_movement = getMovementDirection();

  // Determine majority average of window
  int maj_movement = kernelize(curr_movement);
  if (maj_movement == -1) {
    return;
  }
  
  // Write to servo                 
  myservo.write(maj_movement);
  delay(15);                           // waits for the servo to get there
}
