#include <Servo.h>

Servo myServo;
int servo = 6;  // Declare the Arduino pin number 6 for the servo motor's PWM signal.

#define Pot A0

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  pinMode(Pot, INPUT);
  myServo.attach(servo);
}

void loop() {
  // put your main code here, to run repeatedly:
  float Pot_r = analogRead(Pot);
  Pot_r= map(Pot_r,0,1023,-5,80);

  Serial.println(Pot_r);
  // Serial.print(",");
  //  Serial.print('b');

     if(Serial.available() > 0){
    char data = Serial.read();
  
    if(data == '1'){
      myServo.writeMicroseconds(500);
    }
    else if(data == '2'){
      myServo.writeMicroseconds(1000);
    }
    else if(data == '3'){
      myServo.writeMicroseconds(1500);
    }
    else if(data == '4'){
      myServo.writeMicroseconds(2000);
    }
    else if(data == '5'){
      myServo.writeMicroseconds(2500);
    }
    else{
      Serial.println("Wrong Number");
    }
     }
}
