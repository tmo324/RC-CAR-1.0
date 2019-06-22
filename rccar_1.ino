#include "IRremote.h"
int receiver = 11;
IRrecv irrecv(receiver);
decode_results results;

int in1 = 8;
int in2 = 9;
int in3 = 7;
int in4 = 6;

int trigPin = 12;
int echoPin = 13;
float pingTime;
float targetDistance;
float speedOfSound = 776.5;

int redPin = 3;
int greenPin = 4;
int bluePin = 5;
boolean goingForward=false;

void setup()
{
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode");
  irrecv.enableIRIn();

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void loop()
{
  noTurn();
  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    translateIR();
    irrecv.resume(); // receive the next value
  }
  distanceCalc();
  if (targetDistance < 20 && goingForward==true)
    stopTheEngine();
}

void forward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  goingForward=true;
  setColor(0, 255, 94); // Green Color
}
void backward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
    goingForward=false;
  setColor(255, 255, 255); // Green Color
}

void stopTheEngine() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  setColor(255, 0, 0); // Green Color
}
void right() {
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void left() {
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
void noTurn() {
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void distanceCalc() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2000);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);

  pingTime = pulseIn(echoPin, HIGH);
  pingTime = pingTime / 1000000;
  pingTime = pingTime / 3600;
  targetDistance = speedOfSound * pingTime;
  targetDistance = targetDistance / 2;
  targetDistance = targetDistance * 63360;

  Serial.print("The Distance to Target is: ");
  Serial.print(targetDistance);
  Serial.println(" inches");
}

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}

void translateIR() {
  switch (results.value)
  {
    case 0xFF906F: Serial.println("up"); forward(); break;

    case 0xFFE01F: Serial.println("down"); backward(); break;
    case 0xF076C13B: Serial.println("down"); backward(); break;

    case 0x20FE4DBB: Serial.println("right"); right(); break;
    case 0xFFC23D: Serial.println("right"); right(); break;

    case 0xFF02FD: Serial.println("noTurn"); noTurn(); break;

    case 0x52A3D41F: Serial.println("left"); left(); break;
    case 0xFF22DD: Serial.println("left"); left(); break;

    case 0xFF6897: Serial.println("0"); stopTheEngine(); break;
    case 0xC101E57B: Serial.println("0"); stopTheEngine(); break;

    default:
      Serial.println(" other button   ");
  }
  delay(100); // Do not get immediate repeat
}
