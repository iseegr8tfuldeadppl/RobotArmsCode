#include <Servo.h>

Servo mouth, bottom, tilt, spine;

//#define ENA 14
#define IN1 D5
#define IN2 D6
#define IN3 D7
#define IN4 D8
//#define ENB 16


#define MOUTH D1
#define BOTTOM D2
#define TILT D3
#define SPINE D4

void setup() {
  //pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  //pinMode(ENB, OUTPUT);

  //digitalWrite(ENA, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  //digitalWrite(ENB, HIGH);

  mouth.attach(MOUTH);
  bottom.attach(BOTTOM);
  tilt.attach(TILT);
  spine.attach(SPINE);
}

void loop() {
  for(int i=0; i<180; i++){
    mouth.write(i);
    bottom.write(i);
    tilt.write(i);
    spine.write(i);
    delay(1);
  }
  for(int i=0; i<180; i++){
    mouth.write(180-i);
    bottom.write(180-i);
    tilt.write(180-i);
    spine.write(180-i);
    delay(1);
  }
}
