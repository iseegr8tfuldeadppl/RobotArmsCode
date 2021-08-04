#include <Servo.h>

Servo mouth, spine, tilt, bottom;
// mouth int least = 125, most = 170;
// floor  int least = 0, most = 180;
// spine  int least = 20, most = 110;
// tilt int least = 40, most = 160;
int mouth_least = 125, mouth_most = 170;
int bottom_least = 0, bottom_most = 180;
int spine_least = 20, spine_most = 110;
int tilt_least = 40, tilt_most = 160;
float mouth_angle=mouth_least;
float bottom_angle=bottom_least;
float spine_angle=spine_least;
float tilt_angle=tilt_least;

typedef struct {
  float m, b, s, t;
} coordinate;

typedef struct coordinatesStruct{
  int size;
  coordinate arr[10];
} coordinatesStruct;

coordinatesStruct coordinates;

void setup() {
  Serial.begin(9600);
  
  mouth.attach(3);
  bottom.attach(6);
  spine.attach(9);
  tilt.attach(10);

  mouth.write(mouth_least);
  bottom.write(bottom_least);
  spine.write(spine_least);
  tilt.write(tilt_least);

  coordinates.size = 0;
}

void loop() {
  if(Serial.available()>0){
    String message = Serial.readStringUntil('\n');

    if(message=="done"){
        for(int i=5; i>0; i--){
          Serial.println("Starting in " + String(i));
          delay(1000);
        }

        for(int i=0; i<coordinates.size;i++){
          Serial.print("Going to ");
          print_coordinates(coordinates.arr[i]);
          go_to_coordinates(coordinates.arr[i]);
        }

    } else {
      coordinate new_coord;
      new_coord.m = getValue(message, ' ', 0).toFloat();
      new_coord.b = getValue(message, ' ', 1).toFloat();
      new_coord.s = getValue(message, ' ', 2).toFloat();
      new_coord.t = getValue(message, ' ', 3).toFloat();
      print_coordinates(new_coord);
      coordinates.arr[coordinates.size].m = new_coord.m;
      coordinates.arr[coordinates.size].b = new_coord.b;
      coordinates.arr[coordinates.size].s = new_coord.s;
      coordinates.arr[coordinates.size++].t = new_coord.t;
    }

  }
}

void print_coordinates(coordinate coord){
  Serial.println("m:" + String(coord.m) + " b:" + String(coord.b) + " s:" + String(coord.s) + " t:" + String(coord.t));
}


String getValue(String data, char separator, int index)
{
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


void go_to_coordinates(coordinate coord){
  int mouth_direction = coord.m > mouth_angle ? 1: -1;
  int bottom_direction = coord.b > bottom_angle ? 1: -1;
  int spine_direction = coord.s > spine_angle ? 1: -1;
  int tilt_direction = coord.t > tilt_angle ? 1: -1;
  
  int mouth_difference = (coord.m-mouth_angle) * mouth_direction;
  int bottom_difference = (coord.b-bottom_angle) * bottom_direction;
  int spine_difference = (coord.s-spine_angle) * spine_direction;
  int tilt_difference = (coord.t-tilt_angle) * tilt_direction;
  int longest_difference = mouth_difference;
  if(bottom_difference>longest_difference)
    longest_difference = bottom_difference;
  else if(spine_difference>longest_difference)
    longest_difference = spine_difference;
  else if(tilt_difference>longest_difference)
    longest_difference = tilt_difference;


  float mouth_step = (coord.m - mouth_angle) * mouth_direction / longest_difference;
  float bottom_step = (coord.b - bottom_angle) * bottom_direction / longest_difference;
  float spine_step = (coord.s - spine_angle) * spine_direction / longest_difference;
  float tilt_step = (coord.t - tilt_angle) * tilt_direction / longest_difference;
  for(int i=0; i<longest_difference; i++){
    mouth_angle += mouth_step;
    mouth.write(mouth_angle);
    bottom_angle += bottom_step;
    bottom.write(bottom_angle);
    spine_angle += spine_step;
    spine.write(spine_angle);
    tilt_angle += tilt_step;
    tilt.write(tilt_angle);
    delay(3);
  }


}