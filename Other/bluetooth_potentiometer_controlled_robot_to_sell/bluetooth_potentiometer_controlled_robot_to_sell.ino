#include <Servo.h>

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

float mouth_angle=int((mouth_most-mouth_least)/2);
float bottom_angle=int((bottom_most-bottom_least)/2);
float spine_angle=int((spine_most-spine_least)/2);
float tilt_angle=int((tilt_most-tilt_most)/2);
float gate_angle=int((gate_most-gate_least)/2);
int stepp = 25;

float mouth_poten=0, bottom_poten=0, spine_poten=0, tilt_poten=0;

void check_if_potens_updated(){
  float new_mouth_poten = map(analogRead(A1), 0, 1023, 0, 180);
  float new_bottom_poten = map(analogRead(A3), 0, 1023, 0, 180);
  float new_spine_poten = map(analogRead(A5), 0, 1023, 0, 180);
  float new_tilt_poten = map(analogRead(A7), 0, 1023, 0, 180);

  if(new_mouth_poten!=mouth_poten){
    mouth_poten = new_mouth_poten;
    mouth.write(mouth_poten);
  }
  if(new_bottom_poten!=bottom_poten){
    bottom_poten = new_bottom_poten;
    bottom.write(bottom_poten);
  }
  if(new_spine_poten!=spine_poten){
    spine_poten = new_spine_poten;
    spine.write(spine_poten);
  }
  if(new_tilt_poten!=tilt_poten){
    tilt_poten = new_tilt_poten;
    tilt.write(tilt_poten);
  }
}

void setup() { 
  // put your setup code here, to run once:
  mouth.attach(6);
  bottom.attach(A0);
  spine.attach(A2);
  tilt.attach(9);

  check_if_potens_updated();

  Serial.begin(9600);
}



String getValue(String data, char separator, int index){
    int maxIndex = data.length() - 1;
    int j = 0;
    String chunkVal = "";

    for (int i = 0; i <= maxIndex && j <= index; i++)
    {
        chunkVal.concat(data[i]);

        if (data[i] == separator)
        {
            j++;

            if (j > index)
            {
                chunkVal.trim();
                return chunkVal;
            }

            chunkVal = "";
        }
        else if ((i == maxIndex) && (j < index)) {
            chunkVal = "";
            return chunkVal;
        }
    }   
}



void print_coordinates(){
  Serial.println("m: " + String(mouth_angle) + " b:" + String(bottom_angle) +
                " s:" + String(spine_angle) + " t:" + String(tilt_angle) + " g:" + String(gate_angle));
}

void go_to_coordinates(int m, int b, int s, int t, int g, int delayy){
  int mouth_direction = m > mouth_angle ? 1: -1;
  int bottom_direction = b > bottom_angle ? 1: -1;
  int spine_direction = s > spine_angle ? 1: -1;
  int tilt_direction = t > tilt_angle ? 1: -1;
  int gate_direction = g > gate_angle ? 1: -1;
  
  int mouth_difference = (m-mouth_angle) * mouth_direction;
  int bottom_difference = (b-bottom_angle) * bottom_direction;
  int spine_difference = (s-spine_angle) * spine_direction;
  int tilt_difference = (t-tilt_angle) * tilt_direction;
  int gate_difference = (g-gate_angle) * gate_direction;
  int longest_difference = mouth_difference;
  if(bottom_difference>longest_difference)
    longest_difference = bottom_difference;
  if(spine_difference>longest_difference)
    longest_difference = spine_difference;
  if(tilt_difference>longest_difference)
    longest_difference = tilt_difference;
  if(gate_difference>longest_difference)
    longest_difference = gate_difference;


  float mouth_stepp = (m - mouth_angle) / longest_difference;
  float bottom_stepp = (b - bottom_angle) / longest_difference;
  float spine_stepp = (s - spine_angle) / longest_difference;
  float tilt_stepp = (t - tilt_angle) / longest_difference;
  float gate_stepp = (g - gate_angle) / longest_difference;
  for(int i=0; i<longest_difference; i++){
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
    delay(delayy); // 12 good 19 was evenbetter
  }


}

void loop() {

  check_if_potens_updated();
  
  // put your main code here, to run repeatedly:
  if(Serial.available()>0){
    String msg = Serial.readStringUntil('\n');

      if(msg=="0"){
        for(int i=5; i>0; i--){
          Serial.println("Starting in " + String(i));
          delay(1000);
        }int regular_speed = 15;
        int slow_speed = 50;
        int fast_speed = 1;
        int regular_delay = 50;
        for(int i=0; i<10; i++){
          Serial.println("doing loop " + String(i+1));
        go_to_coordinates(180, 75, 40, 105, 70, regular_speed);
        delay(regular_delay);
        go_to_coordinates(180, 75, 80, 65, 70, regular_speed);
        delay(regular_delay);
        go_to_coordinates(180, 75, 105, 30, 70, regular_speed);
        delay(regular_delay);
        go_to_coordinates(180, 75, 105, 30, 145, fast_speed);
        delay(500);
        go_to_coordinates(70, 75, 105, 30, 145, regular_speed);
        delay(regular_delay);
        go_to_coordinates(70, 75, 80, 65, 145, regular_speed);
        delay(regular_delay);
        go_to_coordinates(70, 75, 40, 105, 70, regular_speed);
        delay(regular_delay);
        go_to_coordinates(70, 45, 70, 145, 70, regular_speed);
        delay(regular_delay);
        go_to_coordinates(70, 38, 100, 120, 70, slow_speed);
        delay(regular_delay);
        go_to_coordinates(180, 38, 100, 120, 70, regular_speed);
        delay(regular_delay);
        go_to_coordinates(180, 45, 70, 145, 70, regular_speed);
        delay(regular_delay);
        }
      } else if(msg=="1"){
          stepp = 5;
      } else if(msg=="2"){
          stepp = 10;
      } else if(msg=="3"){
          stepp = 25;
      } else if(msg=="4"){
          stepp = 50;
      } else if(msg=="5"){
          stepp = 100;
      } else if(msg=="6"){
          stepp = 200;
      } else if(msg=="u") {
        mouth_angle += stepp;
        if(mouth_angle>mouth_most) mouth_angle=mouth_most;
        mouth.write(mouth_angle);
        print_coordinates();
      
      } else if(msg=="j"){
        mouth_angle -= stepp;
        if(mouth_angle<mouth_least) mouth_angle=mouth_least;
        mouth.write(mouth_angle);
        print_coordinates();

      } else if(msg=="y") {
        gate_angle += stepp;
        if(gate_angle>gate_most) gate_angle=gate_most;
        gate.write(gate_angle);
        print_coordinates();
      
      } else if(msg=="h"){
        gate_angle -= stepp;
        if(gate_angle<gate_least) gate_angle=gate_least;
        gate.write(gate_angle);
        print_coordinates();

      } else if(msg=="i"){
        bottom_angle += stepp;
        if(bottom_angle>bottom_most)
          bottom_angle=bottom_most;
        bottom.write(bottom_angle);
        print_coordinates();

      } else if(msg=="k"){
        bottom_angle -= stepp;
        if(bottom_angle<bottom_least)
          bottom_angle=bottom_least;
        bottom.write(bottom_angle);
        print_coordinates();
        
      } else if(msg=="o"){
        spine_angle += stepp;
        if(spine_angle>spine_most)
          spine_angle=spine_most;
        spine.write(spine_angle);
        print_coordinates();

      } else if(msg=="l"){
        spine_angle -= stepp;
        if(spine_angle<spine_least)
          spine_angle=spine_least;
        spine.write(spine_angle);
        print_coordinates();
        
      } else if(msg=="p"){
        tilt_angle += stepp;
        if(tilt_angle>tilt_most)
          tilt_angle=tilt_most;
        tilt.write(tilt_angle);
        print_coordinates();

      } else if(msg=="m"){
        tilt_angle -= stepp;
        if(tilt_angle<tilt_least)
          tilt_angle=tilt_least;
        tilt.write(tilt_angle);
        print_coordinates();

      } else {
        float m = getValue(msg, ' ', 0).toFloat();
        float b = getValue(msg, ' ', 1).toFloat();
        float s = getValue(msg, ' ', 2).toFloat();
        float t = getValue(msg, ' ', 3).toFloat();
        float g = getValue(msg, ' ', 4).toFloat();
        if(!(m==0 && b==0 && s==0 && t==0 && g==0)){
          go_to_coordinates(m, b, s, t, g, 19);
          print_coordinates();
        }
      }
  }
}
