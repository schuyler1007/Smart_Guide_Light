#include <DFRobot_LIDAR07.h>
#include <FastLED.h>
#include <Wire.h>
#include <string.h>

#define NUM_LEDS 150
#define DATA_PIN 4
#define CLOCK_PIN 13
#define ONE_SET 15

CRGB leds[NUM_LEDS];

DFROBOT_LIDAR07_IIC LIDAR07;
int *arr = (int*) calloc(NUM_LEDS, 1);


void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); 
  Serial.begin(115200);
  while(!LIDAR07.begin()){
    Serial.println("The sensor returned data validation error");
    delay(1000);
  }
}

void loop() {
  for (int i=0; i<NUM_LEDS; i++){
    arr[i] = 0;
   }
  // put your main code here, to run repeatedly:
  int distance = calc_distance();
  int s_num = distance / 3.3;
  Serial.print("s_num: ");
  Serial.println(s_num);
  for (int i=0; i<20; i++){
    arr[i+s_num] = 1;
  }
  for (int i=0; i<150; i++){
    if (arr[i] == 1){
      leds[i] = CRGB::Green;
    }
    else{
      leds[i] = CRGB::Black;
    }
  }
  FastLED.show();
  delay(500);
}

int calc_distance(){
  int errinfo;
  while(!LIDAR07.startMeasure()){
    Serial.print("Incorrect data was returned");
    delay(1000);
  }
  Serial.print("Distance: ");
  Serial.print(LIDAR07.getDistanceMM() / 10);
  Serial.println(" cm");
  delay(500);
  return (LIDAR07.getDistanceMM() / 10);
}
