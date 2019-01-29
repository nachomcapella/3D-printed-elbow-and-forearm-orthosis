#include <ArduinoSort.h> //https://github.com/emilv/ArduinoSort/
#include <Servo.h>

int sensorValue1 = 0;
int sensorValue2 = 0;
int threshold1 = 50; //Mean of the top 25 readings of active biceps contraction.
int threshold2 = 300;  //Mean of the top 25 readings of active forearm contraction.
int difference1 = 0; //Difference between threshold1 and the mean of the top 25 readings of pasive biceps contraction.
int difference2 = 121; //Difference between threshold2 and the mean of the top 25 readings of pasive forearm contraction.
int window1low, window1high, window2low, window2high;
Servo servo1; //Servomotor controlling flexion/extension of the arm.
Servo servo2; //Servomotor controlling pronation/supination of the forearm.
bool servo1Rotated = false; //Used to tell if the servomotors angle is different from 0º.
bool servo2Rotated = false;
int servo1RotationAngle = 90; //Default rotation angles for the servomotors.
int servo2RotationAngle = 180;

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 bits per second.
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  window1low = threshold1 - difference1 / 2; //We especify a range of values for the muscle contraction to be detected.
  window1high = threshold1 + difference1 / 2;
  window2low = threshold2 - difference2 / 2;
  window2high = threshold2 + difference2 / 2;
  servo1.attach(11); //We declare the pins the servomotors are connected to.
  servo2.attach(10);
  servo1.write(0); //The servomotors start from a 0º position.
  servo2.write(0);
  servo1Rotated = false; //We change the booleans accordingly to the previous step.
  servo2Rotated = false;
  obtainTwoThresholds(); //This changes the threshold and window values automatically for a particular patient instead of using
  //the default ones.
  Serial.println("Starting the activity...");
  delay(3000);
  digitalWrite(13, LOW);
}

// the loop routine runs over and over again forever:
void loop() {
  // displayTwoSensorsSameTime(); //Used to identify the thresholds.
  // displayTwoSensorsThreshold(); //Verify that the two sensors can work simultaneously.
  // tryTwoServos(); //Verify that both servomotors respond correctly.
  // twoServosWithSensors(); //Use two sensors to control two servomotors. Main code.
  servoLedWithSensors(); //Instead of rotating one servo, it lights up a LED.
}
void obtainTwoThresholds() {
  int voluntarySensor1[25];
  int involuntarySensor1[25];
  int voluntarySensor2[25];
  int involuntarySensor2[25];
  int sensorReadings[700];
  int i;
  int j;
  digitalWrite(13, HIGH);
  Serial.println ("Please wait");
  delay(10000);
  digitalWrite(13, LOW);
  delay(500);
  Serial.print ("Starting voluntary bicep contraction readings in 3,");
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(500);
  Serial.print(" 2,");
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(500);
  Serial.print(" 1...");
  digitalWrite(13, HIGH);
  delay(1000);
  Serial.println(" 0!");
  digitalWrite(13, LOW);
  for (j = 0; j < 700; j++) {
    sensorValue1 = analogRead(A0);
    Serial.println(sensorValue1);
    sensorReadings[j] = sensorValue1;
    delay(10);
  }
  sortArray(sensorReadings, 700);
  Serial.println("Saving top 25 values...");
  for (i = 0; i < 25; i++) {
    voluntarySensor1[i] = sensorReadings[700 - i];
    Serial.println(voluntarySensor1[i]);
  }

  Serial.print ("Starting involuntary bicep contraction readings in 3,");
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(500);
  Serial.print(" 2,");
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(500);
  Serial.print(" 1...");
  digitalWrite(13, HIGH);
  delay(1000);
  Serial.println(" 0!");
  digitalWrite(13, LOW);
  for (j = 0; j < 700; j++) {
    sensorValue1 = analogRead(A0);
    Serial.println(sensorValue1);
    sensorReadings[j] = sensorValue1;
    delay(10);
  }
  sortArray(sensorReadings, 700);
  Serial.println("Saving top 25 values...");
  for (i = 0; i < 25; i++) {
    involuntarySensor1[i] = sensorReadings[700 - i];
    Serial.println(involuntarySensor1[i]);
  }

  Serial.print ("Starting voluntary forearm contraction readings in 3,");
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(500);
  Serial.print(" 2,");
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(500);
  Serial.print(" 1...");
  digitalWrite(13, HIGH);
  delay(1000);
  Serial.println(" 0!");
  digitalWrite(13, LOW);
  for (j = 0; j < 700; j++) {
    sensorValue1 = analogRead(A1);
    Serial.println(sensorValue2);
    sensorReadings[j] = sensorValue2;
    delay(10);
  }
  sortArray(sensorReadings, 700);
  Serial.println("Saving top 25 values...");
  for (i = 0; i < 25; i++) {
    voluntarySensor2[i] = sensorReadings[700 - i];
    Serial.println(voluntarySensor2[i]);
  }

  Serial.print ("Starting involuntary forearm contraction readings in 3,");
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(500);
  Serial.print(" 2,");
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(500);
  Serial.print(" 1...");
  digitalWrite(13, HIGH);
  delay(1000);
  Serial.println(" 0!");
  digitalWrite(13, LOW);
  for (j = 0; j < 700; j++) {
    sensorValue2 = analogRead(A1);
    Serial.println(sensorValue2);
    sensorReadings[j] = sensorValue2;
    delay(10);
  }
  sortArray(sensorReadings, 700);
  Serial.println("Saving top 25 values...");
  for (i = 0; i < 25; i++) {
    involuntarySensor2[i] = sensorReadings[700 - i];
    Serial.println(involuntarySensor2[i]);
  }

  Serial.println("Obtaining threshold values...");
  digitalWrite(13, HIGH);
  int sum = 0;
  for (i = 0; i < 25; i++) {
    sum = sum + voluntarySensor1[i];
  }
  threshold1 = sum / 25;
  sum = 0;
  digitalWrite(13, LOW);
  for (i = 0; i < 25; i++) {
    sum = sum + voluntarySensor2[i];
  }
  threshold2 = sum / 25;
  sum = 0;
  digitalWrite(13, HIGH);
  for (i = 0; i < 25; i++) {
    sum = sum + involuntarySensor1[i];
  }
  int threshold1passive = sum / 25;
  sum = 0;
  digitalWrite(13, LOW);
  for (i = 0; i < 25; i++) {
    sum = sum + involuntarySensor2[i];
  }
  int threshold2passive = sum / 25;
  int difference1 = threshold1 - threshold1passive;
  int difference2 = threshold2 - threshold2passive;
  window1low = threshold1 - difference1 / 2; //We especify a range of values for the muscle contraction to be detected.
  window1high = threshold1 + difference1 / 2;
  window2low = threshold2 - difference2 / 2;
  window2high = threshold2 + difference2 / 2;
  Serial.print("Threshold 1 (biceps): ");
  Serial.print(threshold1);
  Serial.print(" +-");
  Serial.println(difference1 / 2);
  Serial.print("Threshold 2 (forearm): ");
  Serial.print(threshold2);
  Serial.print(" +-");
  Serial.println(difference2 / 2);
  digitalWrite(13, HIGH);
}


void servoLedWithSensors() {
  digitalWrite(13, LOW);
  // First servomotor:
  sensorValue1 = analogRead(A0);
  if (sensorValue1 > window1low) {
    Serial.print("S1: ");
    Serial.println(sensorValue1);
    digitalWrite(13, HIGH);
    delay(1000);
  }
  // Second servomotor:
  sensorValue2 = analogRead(A1);
  if (sensorValue2 > window2low) {
    Serial.print("S2: ");
    Serial.print(sensorValue2);
    if (servo2Rotated) {
      servo2.write(0);
      servo2Rotated = false;
      Serial.println(" Angle: 0º");
    } else {
      servo2.write(servo2RotationAngle);
      servo2Rotated = true;
      Serial.println(" Angle: default");
    }
    delay(1000);
  }
  delay(10);
}


void twoServosWithSensors() {
  digitalWrite(13, LOW);
  // First servomotor:
  sensorValue1 = analogRead(A0);
  if (sensorValue1 > window1low) {
    Serial.print("S1: ");
    Serial.print(sensorValue1);
    digitalWrite(13, HIGH);
    if (servo1Rotated) {
      servo1.write(0);
      servo1Rotated = false;
      Serial.println(" Angle: 0º");
    } else {
      servo1.write(servo1RotationAngle);
      servo1Rotated = true;
      Serial.println(" Angle: default");
    }
    delay(1000);
  }
  // Second servomotor:
  sensorValue2 = analogRead(A1);
  if (sensorValue2 > window2low) {
    Serial.print("S2: ");
    Serial.print(sensorValue2);
    digitalWrite(13, HIGH);
    if (servo2Rotated) {
      servo2.write(0);
      servo2Rotated = false;
      Serial.println(" Angle: 0º");
    } else {
      servo2.write(servo2RotationAngle);
      servo2Rotated = true;
      Serial.println(" Angle: default");
    }
    delay(1000);
  }
  delay(10);
}

void tryTwoServos() {
  digitalWrite(13, HIGH);
  servo1.write(90); //Rotates the servomotor to 90º.
  delay(1000);
  digitalWrite(13, LOW);
  servo1.write(0); //Rotates the servomotor back to 0º.
  delay(1000);
  digitalWrite(13, HIGH);
  servo2.write(90);
  delay(1000);
  digitalWrite(13, LOW);
  servo2.write(0);
  delay(1000);
}

void displayTwoSensorsThreshold() {
  digitalWrite(13, LOW);
  // read the input on analog pin 0:
  sensorValue1 = analogRead(A0);
  if (sensorValue1 > window1low) {
    Serial.print("S1: ");
    Serial.println(sensorValue1);
    digitalWrite(13, HIGH);
    delay(1000);
  }
  sensorValue2 = analogRead(A1);
  if (sensorValue2 > window2low) {
    Serial.print("S2: ");
    Serial.println(sensorValue2);
    digitalWrite(13, HIGH);
    delay(1000);
  }
  delay(10);
}

void displayTwoSensorsSameTime () {
  sensorValue1 = analogRead(A0);
  Serial.print(sensorValue1);
  sensorValue2 = analogRead(A1);
  Serial.print(" - ");
  Serial.println(sensorValue2);
  delay(10);
}
