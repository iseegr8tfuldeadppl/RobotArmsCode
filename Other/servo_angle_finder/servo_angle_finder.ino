#include <Servo.h>

Servo servo;
// mouth int least = 125, most = 170;
// floor  int least = 0, most = 180;
// spine  int least = 20, most = 110;
// tilt int least = 40, most = 160;
int least = 40, most = 160;
int angle=least;

void setup() { 
  // put your setup code here, to run once:
  servo.attach(9);
  servo.write(angle);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0){
    char msg = Serial.read();

    switch(msg){
      case '5':
        angle += 10;
        //if(angle>180)
        //  angle=180;
        servo.write(angle);
        Serial.println(angle);
        break;
      case '2':
        angle -= 10;
        //if(angle<0)
        //  angle=0;
        servo.write(angle);
        Serial.println(angle);
        break;
      case '1':
        angle = least;
        servo.write(angle);
        Serial.println(angle);
        break;
      case '3':
        angle = most;
        servo.write(angle);
        Serial.println(angle);
        break;
    }
  }
}