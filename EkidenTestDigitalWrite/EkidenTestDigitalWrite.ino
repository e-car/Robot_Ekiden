/*
 * 駅伝プログラム
 * digitalWrite HIGH, LOW
 * 
 */
const int pinL = 11;
const int pinR = 10;

const int pinMtrPWM1 = 6;
const int pinMtrPWM2 = 5;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pinL, OUTPUT);
  pinMode(pinR, OUTPUT);
  pinMode(pinMtrPWM1, OUTPUT);
  pinMode(pinMtrPWM2, OUTPUT);
}

void loop() {
  Serial.println("Start");
  digitalWrite(pinL, LOW);
  digitalWrite(pinR, LOW);
//  // put your main code here, to run repeatedly:
//  Serial.println("HIGH, LOW");
//  digitalWrite(pinL, HIGH);
//  digitalWrite(pinR, LOW);
//  delay(1000);
//
//  Serial.println("LOW, HIGH");
//  digitalWrite(pinL, LOW);
//  digitalWrite(pinR, HIGH);
//  delay(1000);
//
//  Serial.println("LOW, LOW");
//  digitalWrite(pinL, LOW);
//  digitalWrite(pinR, LOW);
//  delay(1000);
//
//  Serial.println("HIGH, HIGH");
//  digitalWrite(pinL, HIGH);
//  digitalWrite(pinR, HIGH);
//  delay(1000);
//
//  Serial.println("PWM1:ON, PWM2:ON");
//  analogWrite(pinMtrPWM1, 180);
//  analogWrite(pinMtrPWM2, 180);
//  delay(1000);
//
//  Serial.println("PWM1:OFF, PWM2:OFF");
//  analogWrite(pinMtrPWM1, 0);
//  analogWrite(pinMtrPWM2, 0);
////  digitalWrite(pinMtrPWM1, LOW);
////  digitalWrite(pinMtrPWM2, LOW);

  delay(1000);
}
