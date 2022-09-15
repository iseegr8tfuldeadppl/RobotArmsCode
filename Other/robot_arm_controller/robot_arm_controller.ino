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

float mouth_angle=mouth_least;
float bottom_angle=bottom_least;
float spine_angle=spine_least;
float tilt_angle=tilt_most;
float gate_angle=gate_least;
int step = 25;

void setup() { 
  // put your setup code here, to run once:
  mouth.attach(3);
  bottom.attach(5);
  spine.attach(6);
  tilt.attach(9);
  gate.attach(10);

  mouth.write(mouth_least);
  bottom.write(bottom_least);
  spine.write(spine_least);
  tilt.write(tilt_least);
  gate.write(gate_least);

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


  float mouth_step = (m - mouth_angle) / longest_difference;
  float bottom_step = (b - bottom_angle) / longest_difference;
  float spine_step = (s - spine_angle) / longest_difference;
  float tilt_step = (t - tilt_angle) / longest_difference;
  float gate_step = (g - gate_angle) / longest_difference;
  for(int i=0; i<longest_difference; i++){
    mouth_angle += mouth_step;
    mouth.write(mouth_angle);
    bottom_angle += bottom_step;
    bottom.write(bottom_angle);
    spine_angle += spine_step;
    spine.write(spine_angle);
    tilt_angle += tilt_step;
    tilt.write(tilt_angle);
    gate_angle += gate_step;
    gate.write(gate_angle);
    delay(delayy); // 12 good 19 was evenbetter
  }


}

void loop() {
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
          step = 5;
      } else if(msg=="2"){
          step = 10;
      } else if(msg=="3"){
          step = 25;
      } else if(msg=="4"){
          step = 50;
      } else if(msg=="5"){
          step = 100;
      } else if(msg=="6"){
          step = 200;
      } else if(msg=="u") {
        mouth_angle += step;
        if(mouth_angle>mouth_most) mouth_angle=mouth_most;
        mouth.write(mouth_angle);
        print_coordinates();
      
      } else if(msg=="j"){
        mouth_angle -= step;
        if(mouth_angle<mouth_least) mouth_angle=mouth_least;
        mouth.write(mouth_angle);
        print_coordinates();

      } else if(msg=="y") {
        gate_angle += step;
        if(gate_angle>gate_most) gate_angle=gate_most;
        gate.write(gate_angle);
        print_coordinates();
      
      } else if(msg=="h"){
        gate_angle -= step;
        if(gate_angle<gate_least) gate_angle=gate_least;
        gate.write(gate_angle);
        print_coordinates();

      } else if(msg=="i"){
        bottom_angle += step;
        if(bottom_angle>bottom_most)
          bottom_angle=bottom_most;
        bottom.write(bottom_angle);
        print_coordinates();

      } else if(msg=="k"){
        bottom_angle -= step;
        if(bottom_angle<bottom_least)
          bottom_angle=bottom_least;
        bottom.write(bottom_angle);
        print_coordinates();
        
      } else if(msg=="o"){
        spine_angle += step;
        if(spine_angle>spine_most)
          spine_angle=spine_most;
        spine.write(spine_angle);
        print_coordinates();

      } else if(msg=="l"){
        spine_angle -= step;
        if(spine_angle<spine_least)
          spine_angle=spine_least;
        spine.write(spine_angle);
        print_coordinates();
        
      } else if(msg=="p"){
        tilt_angle += step;
        if(tilt_angle>tilt_most)
          tilt_angle=tilt_most;
        tilt.write(tilt_angle);
        print_coordinates();

      } else if(msg=="m"){
        tilt_angle -= step;
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
