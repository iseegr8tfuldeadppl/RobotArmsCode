#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


//#define ENA 14
#define IN1 D5
#define IN2 D6
#define IN3 D7
#define IN4 D8
//#define ENB 16

WiFiClient client;
ESP8266WebServer server(80);

const char* ssid = "Arm - MustShop Arduino";   //enter your wi-fi user id
const char* password = "88888888";  //enter the wi-fi password


Servo mouth, spine, tilt, bottom; //gate;
/*
int mouth_least = 105, mouth_most = 175;
int bottom_least = 0, bottom_most = 180;
int spine_least = 20, spine_most = 110;
int tilt_least = 40, tilt_most = 180;
int gate_least = 0, gate_most = 180;
*/

// serial communication variables
const int arrSize = 10;
String* angles = new String[arrSize]; // according to the largest string array we'll need (ex. ANGLES 90 90 90 90 90)


//int gate_least = 0;
//int gate_most = 180;
int mouth_least = 0;
int mouth_most = 180;
int bottom_least = 0;
int bottom_most = 180;
int spine_least = 0;
int spine_most = 180;
int tilt_least = 0;
int tilt_most = 180;

float bottom_angle= 90;
float spine_angle=90;
float tilt_angle=90;
float mouth_angle= 90;
//float gate_angle=90;
int stepp = 25;


unsigned int j = 0;
void getValues(String msg){
    j = 0;
    angles[0] = "";
    for (unsigned int i = 0; i < msg.length(); i++) {
        if(j>=arrSize)
          break;
        if(msg[i]==','){ j += 1; angles[j] = ""; }
        else angles[j] += msg[i];
    }
}

float mouth_stepp, bottom_stepp, spine_stepp, tilt_stepp;//, gate_stepp;

void go_to_coordinates(int m, int b, int s, int t, int Speed){ // , int g

  mouth_stepp = (m - mouth_angle) / Speed;
  bottom_stepp = (b - bottom_angle) / Speed;
  spine_stepp = (s - spine_angle) / Speed;
  tilt_stepp = (t - tilt_angle) / Speed;
  //gate_stepp = (g - gate_angle) / Speed;

  //long before = millis();
  for(int i= 0; i < Speed; i++){
    //Serial.println(String(mouth_angle) + " " + String(bottom_angle) + " " + String(spine_angle) + " " + String(tilt_angle) + " " + String(gate_angle));
    mouth_angle += mouth_stepp;
    mouth.write(mouth_angle);   
    bottom_angle += bottom_stepp;
    bottom.write(bottom_angle);
    spine_angle += spine_stepp;
    spine.write(spine_angle);
    tilt_angle += tilt_stepp;
    tilt.write(tilt_angle);
    //gate_angle += gate_stepp;
    //gate.write(gate_angle);
    //delay(delayy);
  }
  //Serial.println("after " + String(millis()-before));

  mouth_angle = m;
  spine_angle = s;
  tilt_angle = t;
  bottom_angle = b;
  //gate_angle = g;
  
  mouth.write(mouth_angle);
  bottom.write(bottom_angle);
  spine.write(spine_angle);
  tilt.write(tilt_angle);
  //gate.write(gate_angle);

}

void handleRoot(){
  server.send(200, "text/plain", "Ok, ONLINE");
}

long temp_bottom, temp_spine, temp_tilt, temp_mouth;//, temp_gate;
void handleMotion(){
  String msg = server.arg("angles");
  Serial.println("Received this " + String(msg));
  if(msg!=""){

    getValues(msg);
    
    temp_bottom = angles[0].toInt();
    if(isnan(temp_bottom)){
      server.send(200, "text/plain", "OOF, INVALID ANGLE " + String(angles[0]) + " FOR BOTTOM");
      return;
    }
    
    temp_spine = angles[1].toInt();
    if(isnan(temp_spine)){
      server.send(200, "text/plain", "OOF, INVALID ANGLE " + String(angles[1]) + " FOR SPINE");
      return;
    }
    
    temp_tilt = angles[2].toInt();
    if(isnan(temp_tilt)){
      server.send(200, "text/plain", "OOF, INVALID ANGLE " + String(angles[2]) + " FOR TILT");
      return;
    }

    // apply angle to mouth servo
    temp_mouth = angles[3].toInt();
    if(isnan(temp_mouth)){
      server.send(200, "text/plain", "OOF, INVALID ANGLE " + String(angles[3]) + " FOR MOUTH");
      return;
    }
    /*
    // apply angle to gate servo
    temp_gate = angles[4].toInt();
    if(isnan(temp_gate)){
      server.send(200, "text/plain", "OOF, INVALID ANGLE " + String(angles[4]) + " FOR GATE");
      return;
    }
    */

    // go where we actually want at a certain speed
    int Speed = 20;
    //go_to_coordinates(temp_mouth, temp_bottom, temp_spine, temp_tilt, Speed); // , temp_gate
    /*mouth_angle = temp_mouth;
    mouth.write(mouth_angle);
    
    bottom_angle = temp_bottom;
    bottom.write(bottom_angle);
    
    spine_angle = temp_spine;
    spine.write(spine_angle);
    
    tilt_angle = temp_tilt;
    tilt.write(tilt_angle);

    */
    mouth.writeMicroseconds(map(temp_mouth, 0, 180, 500, 2500));
    bottom.writeMicroseconds(map(temp_bottom, 0, 180, 500, 2500));
    spine.writeMicroseconds(map(temp_spine, 0, 180, 500, 2500));
    tilt.writeMicroseconds(map(temp_tilt, 0, 180, 500, 2500));

    if(angles[5]=="1"){ // forth
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    } else if(angles[6]=="1"){ // left
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    } else if(angles[7]=="1"){ // right
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    } else if(angles[8]=="1"){ // back
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    } else {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
    }
    // reply with success confirmation
    server.send(200, "text/plain", "Ok, UPDATED ANGLES TO " + String(temp_bottom) + " " + String(temp_spine) + " " + String(temp_tilt) + " " + String(temp_mouth)); //  + " " + String(angles[4]
  }
}

void handleCheck(){
    server.send(200, "text/plain", "Ok, ANGLES ARE " + String(temp_bottom) + " " + String(temp_spine) + " " + String(temp_tilt) + " " + String(temp_mouth)); //  + " " + String(angles[4]
  
}




void setup() {
  Serial.begin(115200);
  Serial.println("good");
  
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
  
  digitalWrite(LED_BUILTIN, HIGH);
  
  // put your setup code here, to run once:
  mouth.attach(D3); // 
  bottom.attach(D4); // 
  spine.attach(D1); //
  tilt.attach(D2); //
  //gate.attach(D7); //

  mouth.write(90);
  bottom.write(104);
  spine.write(15);
  tilt.write(148);
  //gate.write(gate_most);

  boolean result = WiFi.softAP(ssid, password);
  if(result == true){
    //Serial.println("Ready");
  } else {
    //Serial.println("Failed!");
  }

  server.on("/", handleRoot);
  server.on("/motion/", handleMotion);
  server.on("/getmotion/", handleCheck);
  server.begin();
  
}


void loop() {
  server.handleClient();
}
