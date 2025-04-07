#include <Servo.h>

// Color Sensor Pins
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

// L298N Motor Driver Pins
#define IN1 9
#define IN2 10
#define ENA 11

Servo servo1;
Servo servo2; // Second servo

int frequency = 0;
int color = 0;

int motorSpeed = 0;
int servo1Position = 90;  // Default servo1 position
int servo2Position = 90;  // Default servo2 position

void setup() {
  // Color Sensor Setup
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  // Motor Driver Setup
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  // Servo Setup
  servo1.attach(3);
  servo2.attach(5); // Attach second servo to pin 5

  // Frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  Serial.begin(9600);
}

void loop() {
  color = readColor(); // Read color from the sensor

  // Servo Position Based on Color
  switch (color) {
    case 1: // Red
      servo1Position = 50;
      break;
    case 3: // Green
      servo1Position = 100;
      servo2Position = 90; // Return servo2 to default position
      break;
    case 6: // Blue
      servo1Position = 175;
      servo2Position = 30; // Move servo2 when Blue detected
      break;
    default:
      servo1Position = 90;
      servo2Position = 90; // Default positions when no color detected
      break;
  }

  // Move servos to their respective positions
  servo1.write(servo1Position);
  servo2.write(servo2Position);

  // Serial Monitor Output
  Serial.print("Color: ");
  switch (color) {
    case 1: Serial.print("Red"); break;
    case 3: Serial.print("Green"); break;
    case 6: Serial.print("Blue"); break;
    default: Serial.print("None"); break;
  }
  Serial.print(" | ");

  Serial.print("Servo1 Position: ");
  Serial.print(servo1Position);
  Serial.print(" | ");

  Serial.print("Servo2 Position: ");
  Serial.println(servo2Position);

  delay(500); // Delay for stable readings
}

void runMotor(int speed) {
  analogWrite(ENA, speed);
  digitalWrite(IN1, speed > 0 ? HIGH : LOW);
  digitalWrite(IN2, speed > 0 ? LOW : HIGH);
}

int readColor() {
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  frequency = pulseIn(sensorOut, LOW);
  int R = frequency;
  delay(50);

  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  frequency = pulseIn(sensorOut, LOW);
  int G = frequency;
  delay(50);

  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  frequency = pulseIn(sensorOut, LOW);
  int B = frequency;
  delay(50);

  // Color detection logic
  if (R < 55 && R > 45 && G < 149 && G > 139) return 1; // Red
  if (R < 87 && R > 97 && G < 69 && G > 74) return 3;    // Green
  if (G < 119 && G > 129 && B < 62 && B > 72) return 6;   // Blue

  return 0; // No color detected
}
