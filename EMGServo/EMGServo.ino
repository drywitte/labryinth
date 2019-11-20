#include <Servo.h>

Servo servoRoll;  // create servo object to control a servo
Servo servoPitch;  // create servo object to control a servo


// Reading
int rollSensorPin = A0;
int pitchSensorPin = A1;

// Writing
int servoRollPin = 10;
int servoPitchPin = 11;

//int servoValue;

// Servo callibration
int stopped = 88;
int clockwise = 85;
int counterclockwise = 90;

// Kernelize values
int window_size = 5;

int roll_window[5];
int roll_window_filled = 0;

int pitch_window[5];
int pitch_window_filled = 0;



int bounds[] = {220, 400};

void setup() {
  servoPitch.attach(servoRollPin);  // attaches the servo on pin 10 to the servo object
  servoPitch.attach(servoPitchPin);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
}

void printSensorValue(int flexType) {
  
}

int getMovementDirection(int sensorPin, int bounds[]) {
  // Read EMG
  int movement;
  int sensorValue = analogRead(sensorPin);
  if (sensorValue < bounds[0]) {
    if (sensorPin == rollSensorPin) {
      Serial.println("Roll: no flex " + String(sensorValue));
    }
    else {
      Serial.println("                                   Pitch: no flex " + String(sensorValue));
    }
    
    movement = stopped;
  }
  else if (sensorValue < bounds[1]) {
    if (sensorPin == rollSensorPin) {
      Serial.println("Roll: half flex " + String(sensorValue));
    }
    else {
      Serial.println("                                   Pitch: half flex " + String(sensorValue));
    }
    movement = counterclockwise;
  }
  else {
    if (sensorPin == rollSensorPin) {
      Serial.println("Roll: full flex " + String(sensorValue));
    }
    else {
      Serial.println("                                   Pitch: full flex " + String(sensorValue));
    }
    movement = clockwise;
  }
  return movement;
}

int kernelize(int curr_movement, int window[], int* window_filled) {
  
  // Fill window
  if (window_filled < window_size) {
    // Fill window
    window[*window_filled] = curr_movement;
    *window_filled++;
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
    curr_movement = stopped;
  }
  if (clockwise_count > majority) {
    majority = clockwise_count;
    curr_movement = clockwise;
  }
  if (counterclockwise_count > majority) {
    majority = counterclockwise_count;
    curr_movement = counterclockwise;
  }

  return curr_movement;
}

void loop() {

  // Get current movement
  int roll_curr_movement = getMovementDirection(rollSensorPin, bounds);
  int pitch_curr_movement = getMovementDirection(pitchSensorPin, bounds);

  // Determine majority average of window
  int roll_maj_movement = kernelize(roll_curr_movement, roll_window, &roll_window_filled);
  int pitch_maj_movement = kernelize(pitch_curr_movement, pitch_window, &pitch_window_filled);

  if (roll_maj_movement == -1) {
    return;
  }
  if (pitch_maj_movement == -1) {
    return;
  }
  
  // Write to servo                 
  servoRoll.write(80/*roll_maj_movement*/); // TODO: THIS DOES NOT WORK
  servoPitch.write(pitch_maj_movement);
  delay(15);                           // waits for the servo to get there
}
