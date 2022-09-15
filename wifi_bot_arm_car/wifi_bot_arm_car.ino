<#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


WiFiClient client;
ESP8266WebServer server(80);

const char* ssid = "Arm - MustShop Arduino";   //enter your wi-fi user id
const char* password = "88888888";  //enter the wi-fi password


Servo mouth, spine, tilt, bottom, gate;
/*
int mouth_least = 105, mouth_most = 175;
int bottom_least = 0, bottom_most = 180;
int spine_least = 20, spine_most = 110;
int tilt_least = 40, tilt_most = 180;
int gate_least = 0, gate_most = 180;
*/

int gate_least = 0;
int gate_most = 180;
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
float gate_angle=90;
int stepp = 25;


const int arrSize = 10;
String* getValues(String &msg){

    String* arr = new String[arrSize]; // according to the largest string array we'll need (ex. ANGLES 90 90 90 90 90)
    //for(int v=0; v<arrSize; v++){
    //    arr[v] = "";
    //}
    
    // if string is empty return
    if(msg==""){
        //Serial.println("OOF, EMPTY");
        return arr;
    }

    unsigned int j = 0;
    for (unsigned int i = 0; i < msg.length(); i++) {
        if(msg[i]==',')
            j += 1;
        else
            arr[j] += msg[i];
    }

    return arr;
}

float mouth_stepp, bottom_stepp, spine_stepp, tilt_stepp, gate_stepp;

void go_to_coordinates(int m, int b, int s, int t, int g, int Speed){

  mouth_stepp = (m - mouth_angle) / Speed;
  bottom_stepp = (b - bottom_angle) / Speed;
  spine_stepp = (s - spine_angle) / Speed;
  tilt_stepp = (t - tilt_angle) / Speed;
  gate_stepp = (g - gate_angle) / Speed;

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
    gate_angle += gate_stepp;
    gate.write(gate_angle);
    //delay(delayy);
  }
  //Serial.println("after " + String(millis()-before));

  mouth_angle = m;
  spine_angle = s;
  tilt_angle = t;
  bottom_angle = b;
  gate_angle = g;
  
  mouth.write(mouth_angle);
  bottom.write(bottom_angle);
  spine.write(spine_angle);
  tilt.write(tilt_angle);
  gate.write(gate_angle);

}

void handleRoot(){
  server.send(200, "text/plain", "Ok, ONLINE");
}

long temp_bottom, temp_spine, temp_tilt, temp_mouth, temp_gate;
void handleMotion(){
  String msg = server.arg("angles");
  if(msg!=""){
    String* angles = getValues(msg);
    
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

    // apply angle to gate servo
    temp_gate = angles[4].toInt();
    if(isnan(temp_gate)){
      server.send(200, "text/plain", "OOF, INVALID ANGLE " + String(angles[4]) + " FOR GATE");
      return;
    }

    // go where we actually want at a certain speed
    int Speed = 20;
    go_to_coordinates(temp_mouth, temp_bottom, temp_spine, temp_tilt, temp_gate, Speed);

    // reply with success confirmation
    server.send(200, "text/plain", "Ok, UPDATED ANGLES TO " + String(angles[0]) + " " + String(angles[1]) + " " + String(angles[2]) + " " + String(angles[3]) + " " + String(angles[4]));
  }
}




void setup() {
  
  digitalWrite(LED_BUILTIN, HIGH);
  
  // put your setup code here, to run once:
  mouth.attach(D3); // GPIO0 D3
  bottom.attach(D4); // GPIO2 D4
  spine.attach(D5); // GPIO14 D5
  tilt.attach(D6); // GPIO12 D6
  gate.attach(D7); // GPIO13 D7

  //mouth.write(mouth_most);
  //bottom.write(bottom_most);
  //spine.write(spine_most);
  //tilt.write(tilt_most);
  //gate.write(gate_most);

  boolean result = WiFi.softAP(ssid, password);
  if(result == true){
    //Serial.println("Ready");
  } else {
    //Serial.println("Failed!");
  }

  server.on("/", handleRoot);
  server.on("/motion/", handleMotion);
  server.begin();
  
}


void loop() {
  server.handleClient();
}
