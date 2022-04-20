

#include <DFRobot_LIDAR07.h>
#include <FastLED.h>
#include <Wire.h>
#include <string.h>

#define NUM_LED 150
#define DATA_PIN 4
#define CLOCK_PIN 13
#define ONE_SET 15
#define echoPin 2
#define trigPin 3

DFROBOT_LIDAR07_IIC LIDAR07;

int sum = 0;
int duration;
int dummy_var = 0;
char walking_direction[12];  // SET TO "RightToLeft" or "LeftToRight"

void OrangeLight(bool* Area_first_time, int LEDset[], CRGB LEDS,int index_lower_bound, int index_upper_bound, int duration,int* previous_checkpoint_time);
void RedLight(int distance,int LEDset[],CRGB LEDS);
void GreenLight(int distance, int LEDset[],CRGB LEDS);

int LEDset_1[NUM_LED] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int LEDset_2[NUM_LED] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int distance_1;       // reading from sensor 1  // ASSIGN DISTANCE FROM SENSOR 1
int distance_2 = -1;  // reading from sensor 2 // ASSIGN DISTANCE FROM SENSOR 2

int current_distance_1 = 0;  // current reading from sensor 1
int current_distance_2 = 0;  // current reading from sensor 2

int previous_checkpoint_time = 5;  // Giving 5 seconds(assuming duration is in seconds) in advance so we
       
int* previous_checkpoint_time_pointer = &previous_checkpoint_time;


bool S1_first_time_1 = true;
bool S2_first_time_1 = true;
bool S3_first_time_1 = true;
bool* S1_first_time_1_pointer = &S1_first_time_1;
bool* S2_first_time_1_pointer = &S2_first_time_1;
bool* S3_first_time_1_pointer = &S3_first_time_1;

bool S1_first_time_2 = true;
bool S2_first_time_2 = true;
bool S3_first_time_2 = true;
bool* S1_first_time_2_pointer = &S1_first_time_2;
bool* S2_first_time_2_pointer = &S2_first_time_2;
bool* S3_first_time_2_pointer = &S3_first_time_2;

CRGB LEDS_1[NUM_LED];  // ASSIGN LEDSET 1
CRGB LEDS_2[NUM_LED];  // ASSIGN LEDSET 2

int start_point_clarification_count_1 =
    0;  // clarifying person location to initate the sequence of commands
int start_point_clarification_count_2 = 0;

int num_for_starting_timer = 0;

// 0-----SECTION3-----43----SECTION2------86----SECTION1------    AREA
// DISTRIBUTION FOR EACH LED SET
/*
 *  0,43,86 represent the bitset indexes where we read the time and measure the
 * time spent in previous section. This program assumes that the person is
 * walking from afar so section 1,2 and 3 will be triggered in order. There are
 * 3 different code blocks  each taking care of the section flash lighting. And
 * they have bool first time entrance mechanism to avoid triggering it twice, we
 * expect the walker to always move forward.
 *
 *
 *
 */

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(LEDS_1, NUM_LED);
  //  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  //  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(115200);
  while (!LIDAR07.begin()) {
    Serial.println("The sensor returned data validation error");
    delay(1000);
  }

  uint32_t version;
  version = LIDAR07.getVersion();
  Serial.print("VERSION: ");
  Serial.print((version >> 24) & 0xFF, HEX);
  Serial.print(".");
  Serial.print((version >> 16) & 0xFF, HEX);
  Serial.print(".");
  Serial.print((version >> 8) & 0xFF, HEX);
  Serial.print(".");
  Serial.println((version)&0xFF, HEX);

  LIDAR07.startFilter();

  // walking_direction = "RightToLeft";

  strcpy(walking_direction, "RightToLeft");
}

void loop() {
  // READING DISTANCE FUNCTIONS HERE
  distance_1 = calc_distance();

  Serial.print("ledset is ");
  //  Serial.println(LEDset_1);
  
  for (int i = 0; i < NUM_LED; i++) {
    if (LEDset_1[i] == 0) {
      LEDS_1[i] = CRGB::Black;
    }
  }

  if (!strcmp(walking_direction, "RightToLeft")) {
    if (num_for_starting_timer == 0) {  // STARTING TIMER, will executed once
      previous_checkpoint_time = 5;
      num_for_starting_timer = 1;
    }

    // never setting LEDset_1 to 0 or 1

    if (350 < distance_1 < 500) start_point_clarification_count_1++;

    if (start_point_clarification_count_1 >   3) {  
      if (current_distance_1 < distance_1 + 50) {
       
       GreenLight( distance_1,  LEDset_1, LEDS_1);
       

        current_distance_1 = distance_1;
      } else {
        RedLight( distance_1,LEDset_1 , LEDS_1);
      }

      OrangeLight(S1_first_time_1_pointer,  LEDset_1,  LEDS_1,  86, 150,  duration, previous_checkpoint_time_pointer);

      OrangeLight(S2_first_time_1_pointer,  LEDset_1,  LEDS_1,  42, 86,  duration, previous_checkpoint_time_pointer);

      OrangeLight(S3_first_time_1_pointer,  LEDset_1,  LEDS_1,  0, 43,  duration,  previous_checkpoint_time_pointer);

      // SENSOR 1 STUFF FINISHED, SENSOR 2 STUFF BELOW, PERSON IS EXPECTED TO BE
      // AROUND THE CORNER OF THE L SHAPE AT THIS POINT

      if (distance_2 < 150) start_point_clarification_count_2++;

     
      if (start_point_clarification_count_2 >  3) {  // PERSON LOCATION CLARIFIED, EXECUTING THE LIGHT COMMANDS

        if (current_distance_2 > distance_2 - 50) {
         
          GreenLight( distance_2,  LEDset_2, LEDS_2);
         
        current_distance_2 = distance_2;
        } else {
          
          RedLight( distance_2,  LEDset_2 , LEDS_2);
        }






        OrangeLight( S3_first_time_2_pointer,  LEDset_2,  LEDS_2,  0, 43,  duration,  previous_checkpoint_time_pointer);
        
        OrangeLight( S2_first_time_2_pointer,  LEDset_2,  LEDS_2,  42, 86,  duration,  previous_checkpoint_time_pointer);
        
        OrangeLight( S1_first_time_2_pointer,  LEDset_2,  LEDS_2,  86, 43,  duration,  previous_checkpoint_time_pointer);

        // AFTER X AMOUNT OF MINUTES PASSED, RESET THE PROGRAM COULD BE ADDED
        // HERE
      }
    }

  } else  // IF THE DIRECTION IS NOT FROM RIGHT TO LEFT THEN ITS LEFT TO RIGHT,
          // WE DO THE EXACT SAME STUFF IN REVERSE ORDER

  {
    if (num_for_starting_timer == 0) {  // STARTING TIMER, will executed once
      previous_checkpoint_time = 5;
      num_for_starting_timer = 1;
    }

    if (350 < distance_2 < 500) start_point_clarification_count_2++;

    if (start_point_clarification_count_2 > 3) {  // EXECUTING EVERYTHING HERE AFTER CLARIFYING PERSON LOCATION,
                                                  // expecting 3 readings that are in the range of the expected
                                                  // starting point

      if (current_distance_2 < distance_2 + 50) {
        GreenLight( distance_2,  LEDset_2, LEDS_2);
       

        current_distance_2 = distance_2;
      } else {
        
        RedLight( distance_2,  LEDset_2 , LEDS_2);
      
      }

      OrangeLight(S1_first_time_2_pointer,  LEDset_2,  LEDS_2,  86, 150,  duration, previous_checkpoint_time_pointer);
      
      OrangeLight(S2_first_time_2_pointer,  LEDset_2,  LEDS_2,  42, 86,  duration, previous_checkpoint_time_pointer);
      
      OrangeLight(S3_first_time_2_pointer,  LEDset_2,  LEDS_2,  0, 43,  duration, previous_checkpoint_time_pointer);

      // SENSOR 2 STUFF FINISHED, SENSOR 1 STUFF BELOW, PERSON IS EXPECTED TO BE
      // AROUND THE CORNER OF THE L SHAPE AT THIS POINT

      if (distance_1 < 150) start_point_clarification_count_1++;

      if (start_point_clarification_count_1 <  3) {  // PERSON LOCATION CLARIFIED, EXECUTING THE LIGHT COMMANDS

        if (current_distance_1 > distance_1 - 50) {
          
          GreenLight( distance_1,  LEDset_1, LEDS_1);
          
          current_distance_1 = distance_1;
        } else {

         
          RedLight( distance_1,  LEDset_1 , LEDS_1);
      
        }

        OrangeLight(S3_first_time_1_pointer,  LEDset_1,  LEDS_1,  0, 43,  duration,  previous_checkpoint_time_pointer);
        
        OrangeLight( S2_first_time_1_pointer,  LEDset_1,  LEDS_1,  42, 86,  duration,  previous_checkpoint_time_pointer);
        
        OrangeLight( S1_first_time_1_pointer,  LEDset_1,  LEDS_1,  86, 150,  duration,  previous_checkpoint_time_pointer);

        // AFTER X AMOUNT OF MINUTES PASSED, RESET THE PROGRAM COULD BE ADDED
        // HERE
      }
    }
  }

  for (int i = 0; i < 150; i++) {
    Serial.print(LEDset_1[i]);
  }
}

void GreenLight(int distance, int LEDset[],CRGB LEDS[]) {
  int s_num = distance/3.3;
  if(s_num < 20) s_num = s_num+20;
  for(int i = 0;i<20;i++){
    LEDset[s_num-20] = 1;
    
    }
  for (int i = 0; i < 150; i++) {  // USUAL GREEN LIGHT BLOCK
    if (LEDset[i] == 1) {
      LEDS[i] = CRGB::Green;
    } else {
      LEDS[i] = CRGB::Black;
    }
  }

}

void RedLight(int distance,int LEDset[],CRGB LEDS[]) {
  int flash_count = 0;
  int temp_s_num  = distance /3.3;
  int s_num;
  if(temp_s_num > 10) s_num = temp_s_num-10; // this is to avoid negative index values if the index is below 10
  else s_num = temp_s_num;
  
  for (int i = 0; i < 30; i++) {
    LEDset[s_num + i] = 1;
  }

  while (flash_count < 10) {
    for (int i = 0; i < 150; i++) {
      if (LEDset[i] == 1) {
        LEDS[i] = CRGB::Red;
      } else {
        LEDS[i] = CRGB::Black;
      }
    }

    for (int i = 0; i < 150; i++) {
      LEDS[i] = CRGB::Black;
    }
    flash_count++;
    delay(500);
  }
}

void OrangeLight(bool* Area_first_time, int LEDset[], CRGB LEDS[],int index_lower_bound, int index_upper_bound, int duration,int* previous_checkpoint_time) {
  int new_index_lower_bound;

      if (index_lower_bound > 10) new_index_lower_bound = index_lower_bound - 10;
      else new_index_lower_bound = index_lower_bound;

  if (*Area_first_time == true) {
    int check_it = 0;
    for (int i = 0; i < 20; i++) {
      if (LEDset[new_index_lower_bound + i] == 1)
        check_it = 1;  // margin of +- 10 in case the walker moves too fast
    }

    if (check_it == 1) {
      int time_difference = duration - *previous_checkpoint_time;  // time spent walking in that section

      if (time_difference > 20) {        // if above 20 seconds
        for (int i = 0; i < 150; i++) {  // reset
          LEDset[i] = 0;
        }

        int flash_count = 0;

        while (flash_count < 4) {  // flash orange color slowly 3-4 times
          for (int i = 0; i < 150; i++) {
            if (index_lower_bound < i < index_upper_bound) {
              LEDS[i] = CRGB::Orange;
            } else {
              LEDS[i] = CRGB::Black;
            }
          }

          for (int i = 0; i < 150; i++) {
            LEDS[i] = CRGB::Black;
          }
          flash_count++;
          delay(1000);
        }
      }
    }

    *Area_first_time ==  false;  // already entered once so do not enter this loop again
    *previous_checkpoint_time = duration;  // update checkpoint
  }
}

int calc_distance() {
  int errinfo;
  while (!LIDAR07.startMeasure()) {
    Serial.println("Incorrect data was returned");
    delay(1000);
  }
  Serial.print("Distance:");
  Serial.print(LIDAR07.getDistanceMM());
  Serial.println(" mm");
  Serial.print("Amplitude:");
  Serial.println(LIDAR07.getSignalAmplitude());
  delay(1000);
  return (LIDAR07.getDistanceMM());
}
