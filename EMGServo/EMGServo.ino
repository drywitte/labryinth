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

int calib_size = 5;

// Roll Servo callibration
//int roll_clockwise = 85;
//int roll_stopped = 91;
//int roll_counterclockwise = 97;
int roll_calib[5] = {82, 85, 91, 97, 100};

// Pitch Servo calibration
//int pitch_clockwise = 82;
//int pitch_stopped = 88;
//int pitch_counterclockwise = 93;
int pitch_calib[5] = {79, 82, 88, 93, 96};

// Kernelize values
int window_size = 1;

int roll_window[1];
int roll_window_filled = 0;

int pitch_window[1];
int pitch_window_filled = 0;


int bounds_size = 4;
int bounds[4] = {150, 220, 500, 600};

void setup() {
  servoPitch.attach(servoPitchPin);  // attaches the servo on pin 9 to the servo object
  servoRoll.attach(servoRollPin);  // attaches the servo on pin 10 to the servo object
  Serial.begin(9600);
}

void printSensorValue(int flexType) {
  
}

int getMovementDirection(int sensorPin, int bounds[]) {
  
  // Get calibration dependent on servos
//  int stopped, clockwise, counterclockwise;
  int* calib = calibration(sensorPin);
  
  // Read EMG
  int idx = 0;
  int sensorValue = analogRead(sensorPin);
  for (int i = 0; i < bounds_size; ++i) {
    if (sensorValue > bounds[i]) {
      idx = i+1;
    }
  }

  // Print Flex
  int mid = calib_size/2;
  if (sensorPin == rollSensorPin) {
      Serial.println("Roll: " + String(idx - mid) + " " + String(sensorValue));
    }
    else {
      Serial.println("                                   Pitch: " + String(idx - mid) + " " + String(sensorValue));
   }
  
  int movement = calib[idx];
  return movement;
//  
//  int mid = calib_size/2;
//  for (int i = 0; i < calib_size; i++) {
//
//  }
//  if (sensorValue < bounds[0]) {
//    if (sensorPin == rollSensorPin) {
//      
//    }
//    else {
//      
//    }
//    movement = calib[0];
//  }
//  else if (sensorValue < bounds[1]) {
//    if (sensorPin == rollSensorPin) {
//      Serial.println("Roll: stopped 0 " + String(sensorValue));
//    }
//    else {
//      Serial.println("                                   Pitch: stopped 0 " + String(sensorValue));
//    }
//    movement = calib[1];
//  }
//  else {
//    if (sensorPin == rollSensorPin) {
//      Serial.println("Roll: forward 1 " + String(sensorValue));
//    }
//    else {
//      Serial.println("                                   Pitch: forward 1 " + String(sensorValue));
//    }
//    movement = calib[2];
//  }
//  return movement;
}

int* calibration(int sensorPin) {
  if (sensorPin == rollSensorPin) {
    return roll_calib;
  }
  else {
    return pitch_calib;
  }
}

int kernelize(int curr_movement, int window[], int* window_filled, int sensorPin) {
  
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

  // Bucket sensor values into calibration types
  int* calib = calibration(sensorPin);
  int calib_counts[5] = {0, 0, 0, 0, 0};
  for (int i = 0; i < window_size; ++i) {
    for (int j = 0; j < calib_size; ++j) {
      if (window[i] == calib[j]) {
        calib_counts[j]++;
      }
    }
  }

  // Determine majority
  int majority = -1;
  int majority_movement = -1;
  for (int i = 0; i < calib_size; ++i) {
    if (calib_counts[i] > majority) {
      majority = calib_counts[i];
      majority_movement = calib[i];
    }
  }
  
//  if (stopped_count > majority) {
//    majority = stopped_count;
//    curr_movement = stopped;
//  }
//  if (clockwise_count > majority) {
//    majority = clockwise_count;
//    curr_movement = clockwise;
//  }
//  if (counterclockwise_count > majority) {
//    majority = counterclockwise_count;
//    curr_movement = counterclockwise;
//  }

  return majority_movement;
}

void loop() {

  // Get current movement
  int roll_curr_movement = getMovementDirection(rollSensorPin, bounds);
  int pitch_curr_movement = getMovementDirection(pitchSensorPin, bounds);

  // Determine majority average of window
  int roll_maj_movement = kernelize(roll_curr_movement, roll_window, &roll_window_filled, rollSensorPin);
  int pitch_maj_movement = kernelize(pitch_curr_movement, pitch_window, &pitch_window_filled, pitchSensorPin);

  if (roll_maj_movement == -1) {
    return;
  }
  if (pitch_maj_movement == -1) {
    return;
  }
  
  // Write to servo                 
  servoRoll.write(roll_maj_movement); 
  servoPitch.write(pitch_maj_movement);
  delay(15);                           // waits for the servo to get there
}
