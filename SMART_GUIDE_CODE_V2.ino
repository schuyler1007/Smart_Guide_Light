#include <FastLED.h>

#define NUM_LED 150
#define DATA_PIN 4
#define CLOCK_PIN 13
#define ONE_SET 15
#define echoPin 2
#define trigPin 3

int sum = 0;
int duration;

string walking_direction; // SET TO "RightToLeft" or "LeftToRight"



int LEDset_1[NUM_LED] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int LEDset_2[NUM_LED] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


int distance_1; // reading from sensor 1  // ASSIGN DISTANCE FROM SENSOR 1
int distance_2; // reading from sensor 2 // ASSIGN DISTANCE FROM SENSOR 2

int current_distance_1 = 0;  // current reading from sensor 1
int current_distance_2 = 0; // current reading from sensor 2

int previous_checkpoint_time = 5; // Giving 5 seconds(assuming duration is in seconds) in advance so we dont have to rush things as soon as we upload the program 

// Section first time entrances

bool S1_first_time_1 true;
bool S2_first_time_1 true;
bool S3_first_time_1 true;

bool S1_first_time_2 true;
bool S2_first_time_2 true;
bool S3_first_time_2 true;


CRGB LEDS_1[NUM_LED]; // ASSIGN LEDSET 1 
CRGB LEDS_2[NUM_LED]; // ASSIGN LEDSET 2

int start_point_clarification_count_1 = 0; // clarifying person location to initate the sequence of commands 
int start_point_clarification_count_2 = 0;

int num_for_starting_timer = 0;


// 0-----SECTION3-----43----SECTION2------86----SECTION1------    AREA DISTRIBUTION FOR EACH LED SET 
/* 
 *  0,43,86 represent the bitset indexes where we read the time and measure the time spent in previous section. This program assumes that the person is walking from afar so section 1,2 and 3 will be triggered in order.
 *  There are 3 different code blocks  each taking care of the section flash lighting. And they have bool first time entrance mechanism to avoid triggering it twice, we expect the walker to always move forward.
 *  
 *  
 *  
 */



void setup() {

    FastLED.addLeds<NEOPIXEL, DATA_PIN> (leds, NUM_LED);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600);
  

walking_direction = "RightToLeft";

}






void loop() {




// READING DISTANCE FUNCTIONS HERE 













  

  if(walking_direction = "RightToLeft"){

    if(num_for_starting_timer == 0) { // STARTING TIMER, will executed once
      previous_checkpoint_time = 5;
      num_for_starting_timer = 1;
      }

     
    
    if(3500 <  distance_1 < 5000) start_point_clarification_count_1++;

    if(start_point_clarification_count_1 > 3){ // EXECUTING EVERYTHING HERE AFTER CLARIFYING PERSON LOCATION, expecting 3 readings that are in the range of the expected starting point
      
      if(current_distance_1 < distance + 50){
        
        

           for (int i=0; i<150; i++){ // USUAL GREEN LIGHT BLOCK
    if (LEDset1r[i] == 1){
      LEDS_1[i] = CRGB::Green;
    }
    else{
      LEDS_1[i] = CRGB::Black;
    }
  }

        current_distance_1 = distance;
        }
        else {


            // RED FLASH EFFECT 
           int flash_count = 0;
  
  while(flash_count < 10){
     for (int i=0; i<150; i++){
    if (LEDset_1[i] == 1){
      LEDS_1[i] = CRGB::Red;
    }
    else{
      LEDS_1[i] = CRGB::Black;
    }
  }
 
  for(int i = 0;i<150;i++){
    LEDS_1[i] ) CRGB::Black;
    
    }
    flash_count++;
     delay(500);
    }
          
          
          }

          
if(S1_first_time_1 == true ){ // Checking if the 86th index  and if so send orange flash lights if the person walked the area in more than 20 seconds 
    int check_it = 0;
    for(i = 0;i<20;i++){
      
      if(LEDset_1[76+i] == 1) check_it = 1;  // margin of +- 10 in case the walker moves too fast
      
      }
    
    
    if(check_it == 1){
      int time_difference = duration - previous_checkpoint_time; // time spent walking in that section

          if(time_difference > 20){ // if above 20 seconds

             int flash_count = 0;
  
  while(flash_count < 4){ // flash orange color slowly 3-4 times 
     for (int i=0; i<150; i++){
    if (i>= 86){
      LEDS_1[i] = CRGB::Orange;
    }
    else{
      LEDS_1[i] = CRGB::Black;
    }
  }
 
  for(int i = 0;i<150;i++){
    LEDS_1[i] ) CRGB::Black;
    
    }
    flash_count++;
     delay(1000);
    }   
            }
      
      }
    
    
    S1_first_time_1 == false; // already entered once so do not enter this loop again
    previous_checkpoint_time = duration; // update checkpoint 
    }



    if(S2_first_time_1 == true ){ // Checking if the 86th index  and if so send orange flash lights if the person walked the area in more than 20 seconds 
    int check_it = 0;
    for(i = 0;i<20;i++){
      
      if(LEDset_1[33+i] == 1) check_it = 1;  // margin of +- 10 in case the walker moves too fast
      
      }
    
    
    if(check_it == 1){
      int time_difference = duration - previous_checkpoint_time; // time spent walking in that section

          if(time_difference > 20){ // if above 20 seconds

             int flash_count = 0;
  
  while(flash_count < 4){ // flash orange color slowly 3-4 times 
     for (int i=0; i<150; i++){
    if (86 > i > 42){
      LEDS_1[i] = CRGB::Orange;
    }
    else{
      LEDS_1[i] = CRGB::Black;
    }
  }
 
  for(int i = 0;i<150;i++){
    LEDS_1[i] ) CRGB::Black;
    
    }
    flash_count++;
     delay(1000);
    }   
            }
      
      }
    
    
    S2_first_time_1 == false; // already entered once so do not enter this loop again
    previous_checkpoint_time = duration; // update checkpoint 
    }

    

     if(S3_first_time_1 == true ){ // Checking if the 86th index  and if so send orange flash lights if the person walked the area in more than 20 seconds 
    int check_it = 0;
    for(i = 0;i<20;i++){
      
      if(LEDset_1[i] == 1) check_it = 1;  // margin of +- 10 in case the walker moves too fast, reason for having 33+i rather than 43+i
      
      }
    
    
    if(check_it == 1){
      int time_difference = duration - previous_checkpoint_time; // time spent walking in that section

          if(time_difference > 20){ // if above 20 seconds

             int flash_count = 0;
  
  while(flash_count < 4){ // flash orange color slowly 3-4 times 
     for (int i=0; i<150; i++){
    if ( i < 43){
      LEDS_1[i] = CRGB::Orange;
    }
    else{
      LEDS_1[i] = CRGB::Black;
    }
  }
 
  for(int i = 0;i<150;i++){
    LEDS_1[i] ) CRGB::Black;
    
    }
    flash_count++;
     delay(1000);
    }   
            }
      
      }
    
    
    S3_first_time_1 == false; // already entered once so do not enter this loop again
    previous_checkpoint_time = duration; // update checkpoint 
    }




     // SENSOR 1 STUFF FINISHED, SENSOR 2 STUFF BELOW, PERSON IS EXPECTED TO BE AROUND THE CORNER OF THE L SHAPE AT THIS POINT

    

      if( distance_2 < 1500) start_point_clarification_count_2++;


      if(start_point_clarification_count_2 < 3){// PERSON LOCATION CLARIFIED, EXECUTING THE LIGHT COMMANDS 
        
        
      if(current_distance_2  >  distance - 50){
        
        

           for (int i=0; i<150; i++){ // USUAL GREEN LIGHT BLOCK
    if (LEDset_2[i] == 1){
      LEDS_2[i] = CRGB::Green;
    }
    else{
      LEDS_2[i] = CRGB::Black;
    }
  }

        current_distance_2 = distance_2;
        }
        else {


            // RED FLASH EFFECT 
           int flash_count = 0;
  
  while(flash_count < 10){
     for (int i=0; i<150; i++){
    if (LEDset_2[i] == 1){
      LEDS_2[i] = CRGB::Red;
    }
    else{
      LEDS_2[i] = CRGB::Black;
    }
  }
 
  for(int i = 0;i<150;i++){
    LEDS_2[i] ) CRGB::Black;
    
    }
    flash_count++;
     delay(500);
    }
          
          
          }

          
if(S3_first_time_2 == true ){ // Checking if the 86th index  and if so send orange flash lights if the person walked the area in more than 20 seconds 
    int check_it = 0;
    for(i = 0;i<20;i++){
      
      if(LEDset_2[i] == 1) check_it = 1;  // margin of +- 10 in case the walker moves too fast
      
      }
    
    
    if(check_it == 1){
      int time_difference = duration - previous_checkpoint_time; // time spent walking in that section

          if(time_difference > 20){ // if above 20 seconds

             int flash_count = 0;
  
  while(flash_count < 4){ // flash orange color slowly 3-4 times 
     for (int i=0; i<150; i++){
    if (i < 43){
      LEDS_2[i] = CRGB::Orange;
    }
    else{
      LEDS_2[i] = CRGB::Black;
    }
  }
 
  for(int i = 0;i<150;i++){
    LEDS_2[i] ) CRGB::Black;
    
    }
    flash_count++;
     delay(1000);
    }   
            }
      
      }
    
    
    S3_first_time_2 == false; // already entered once so do not enter this loop again
    previous_checkpoint_time = duration; // update checkpoint 
    }



    if(S2_first_time_2 == true ){ // Checking if the 86th index  and if so send orange flash lights if the person walked the area in more than 20 seconds 
    int check_it = 0;
    for(i = 0;i<20;i++){
      
      if(LEDset_2[33+i] == 1) check_it = 1;  // margin of +- 10 in case the walker moves too fast
      
      }
    
    
    if(check_it == 1){
      int time_difference = duration - previous_checkpoint_time; // time spent walking in that section

          if(time_difference > 20){ // if above 20 seconds

             int flash_count = 0;
  
  while(flash_count < 4){ // flash orange color slowly 3-4 times 
     for (int i=0; i<150; i++){
    if (86 > i > 42){
      LEDS_2[i] = CRGB::Orange;
    }
    else{
      LEDS_2[i] = CRGB::Black;
    }
  }
 
  for(int i = 0;i<150;i++){
    LEDS_2[i] ) CRGB::Black;
    
    }
    flash_count++;
     delay(1000);
    }   
            }
      
      }
    
    
    S2_first_time_2 == false; // already entered once so do not enter this loop again
    previous_checkpoint_time = duration; // update checkpoint 
    }

    

     if(S1_first_time_2 == true ){ // Checking if the 86th index  and if so send orange flash lights if the person walked the area in more than 20 seconds 
    int check_it = 0;
    for(i = 0;i<20;i++){
      
      if(LEDset_2[76+i] == 1) check_it = 1;  // margin of +- 10 in case the walker moves too fast, reason for having 33+i rather than 43+i
      
      }
    
    
    if(check_it == 1){
      int time_difference = duration - previous_checkpoint_time; // time spent walking in that section

          if(time_difference > 20){ // if above 20 seconds

             int flash_count = 0;
  
  while(flash_count < 4){ // flash orange color slowly 3-4 times 
     for (int i=0; i<150; i++){
    if ( i > 76){
      LEDS_2[i] = CRGB::Orange;
    }
    else{
      LEDS_2[i] = CRGB::Black;
    }
  }
 
  for(int i = 0;i<150;i++){
    LEDS_2[i] ) CRGB::Black;
    
    }
    flash_count++;
     delay(1000);
    }   
            }
      
      }
    
    
    S1_first_time_2 == false; // already entered once so do not enter this loop again
    previous_checkpoint_time = duration; // update checkpoint 
    }




        
        
        





        
        
        
        
        
        
        
        
        
        // AFTER X AMOUNT OF MINUTES PASSED, RESET THE PROGRAM COULD BE ADDED HERE 
        
        
        }
    
          
             
      
      
      
      
      }
    
    
    
  
    
   
    }
    else // IF THE DIRECTION IS NOT FROM RIGHT TO LEFT THEN ITS LEFT TO RIGHT, WE DO THE EXACT SAME STUFF IN REVERSE ORDER 
    
    {
      
      
       if(num_for_starting_timer == 0) { // STARTING TIMER, will executed once
      previous_checkpoint_time = 5;
      num_for_starting_timer = 1;
      }

     
    
    if(3500 <  distance_2 < 5000) start_point_clarification_count_2++;

    if(start_point_clarification_count_2 > 3){ // EXECUTING EVERYTHING HERE AFTER CLARIFYING PERSON LOCATION, expecting 3 readings that are in the range of the expected starting point
      
      if(current_distance_2 < distance + 50){
        
        

           for (int i=0; i<150; i++){ // USUAL GREEN LIGHT BLOCK
    if (LEDset_2[i] == 1){
      LEDS_2[i] = CRGB::Green;
    }
    else{
      LEDS_2[i] = CRGB::Black;
    }
  }

        current_distance_2 = distance_2;
        }
        else {


            // RED FLASH EFFECT 
           int flash_count = 0;
  
  while(flash_count < 10){
     for (int i=0; i<150; i++){
    if (LEDset_2[i] == 1){
      LEDS_2[i] = CRGB::Red;
    }
    else{
      LEDS_2[i] = CRGB::Black;
    }
  }
 
  for(int i = 0;i<150;i++){
    LEDS_2[i] ) CRGB::Black;
    
    }
    flash_count++;
     delay(500);
    }
          
          
          }

          
if(S1_first_time_2 == true ){ // Checking if the 86th index  and if so send orange flash lights if the person walked the area in more than 20 seconds 
    int check_it = 0;
    for(i = 0;i<20;i++){
      
      if(LEDset_2[76+i] == 1) check_it = 1;  // margin of +- 10 in case the walker moves too fast
      
      }
    
    
    if(check_it == 1){
      int time_difference = duration - previous_checkpoint_time; // time spent walking in that section

          if(time_difference > 20){ // if above 20 seconds

             int flash_count = 0;
  
  while(flash_count < 4){ // flash orange color slowly 3-4 times 
     for (int i=0; i<150; i++){
    if (i>= 86){
      LEDS_2[i] = CRGB::Orange;
    }
    else{
      LEDS_2[i] = CRGB::Black;
    }
  }
 
  for(int i = 0;i<150;i++){
    LEDS_2[i] ) CRGB::Black;
    
    }
    flash_count++;
     delay(1000);
    }   
            }
      
      }
    
    
    S1_first_time_2 == false; // already entered once so do not enter this loop again
    previous_checkpoint_time = duration; // update checkpoint 
    }



    if(S2_first_time_2 == true ){ 
    int check_it = 0;
    for(i = 0;i<20;i++){
      
      if(LEDset_2[33+i] == 1) check_it = 1;  // margin of +- 10 in case the walker moves too fast
      
      }
    
    
    if(check_it == 1){
      int time_difference = duration - previous_checkpoint_time; // time spent walking in that section

          if(time_difference > 20){ // if above 20 seconds

             int flash_count = 0;
  
  while(flash_count < 4){ // flash orange color slowly 3-4 times 
     for (int i=0; i<150; i++){
    if (86 > i > 42){
      LEDS_2[i] = CRGB::Orange;
    }
    else{
      LEDS_2[i] = CRGB::Black;
    }
  }
 
  for(int i = 0;i<150;i++){
    LEDS_2[i] ) CRGB::Black;
    
    }
    flash_count++;
     delay(1000);
    }   
            }
      
      }
    
    
    S2_first_time_2 == false; // already entered once so do not enter this loop again
    previous_checkpoint_time = duration; // update checkpoint 
    }

    

     if(S3_first_time_2 == true ){ 
    int check_it = 0;
    for(i = 0;i<20;i++){
      
      if(LEDset_2[i] == 1) check_it = 1;  
      
      }
    
    
    if(check_it == 1){
      int time_difference = duration - previous_checkpoint_time; // time spent walking in that section

          if(time_difference > 20){ // if above 20 seconds

             int flash_count = 0;
  
  while(flash_count < 4){ // flash orange color slowly 3-4 times 
     for (int i=0; i<150; i++){
    if ( i < 43){
      LEDS_2[i] = CRGB::Orange;
    }
    else{
      LEDS_2[i] = CRGB::Black;
    }
  }
 
  for(int i = 0;i<150;i++){
    LEDS_2[i] ) CRGB::Black;
    
    }
    flash_count++;
     delay(1000);
    }   
            }
      
      }
    
    
    S3_first_time_2 == false; // already entered once so do not enter this loop again
    previous_checkpoint_time = duration; // update checkpoint 
    }




     // SENSOR 2 STUFF FINISHED, SENSOR 1 STUFF BELOW, PERSON IS EXPECTED TO BE AROUND THE CORNER OF THE L SHAPE AT THIS POINT

    

      if( distance_1 < 1500) start_point_clarification_count_1++;


      if(start_point_clarification_count_1 < 3){// PERSON LOCATION CLARIFIED, EXECUTING THE LIGHT COMMANDS 
        
        
      if(current_distance_1  >  distance - 50){
        
        

           for (int i=0; i<150; i++){ // USUAL GREEN LIGHT BLOCK
    if (LEDset_1[i] == 1){
      LEDS_1[i] = CRGB::Green;
    }
    else{
      LEDS_1[i] = CRGB::Black;
    }
  }

        current_distance_1 = distance_1;
        }
        else {


            // RED FLASH EFFECT 
           int flash_count = 0;
  
  while(flash_count < 10){
     for (int i=0; i<150; i++){
    if (LEDset_1[i] == 1){
      LEDS_1[i] = CRGB::Red;
    }
    else{
      LEDS_1[i] = CRGB::Black;
    }
  }
 
  for(int i = 0;i<150;i++){
    LEDS_1[i] ) CRGB::Black;
    
    }
    flash_count++;
     delay(500);
    }
          
          
          }

          
if(S3_first_time_1 == true ){ 
    int check_it = 0;
    for(i = 0;i<20;i++){
      
      if(LEDset_1[i] == 1) check_it = 1;  // margin of +- 10 in case the walker moves too fast
      
      }
    
    
    if(check_it == 1){
      int time_difference = duration - previous_checkpoint_time; // time spent walking in that section

          if(time_difference > 20){ // if above 20 seconds

             int flash_count = 0;
  
  while(flash_count < 4){ // flash orange color slowly 3-4 times 
     for (int i=0; i<150; i++){
    if (i < 43){
      LEDS_1[i] = CRGB::Orange;
    }
    else{
      LEDS_1[i] = CRGB::Black;
    }
  }
 
  for(int i = 0;i<150;i++){
    LEDS_1[i] ) CRGB::Black;
    
    }
    flash_count++;
     delay(1000);
    }   
            }
      
      }
    
    
    S3_first_time_1 == false; // already entered once so do not enter this loop again
    previous_checkpoint_time = duration; // update checkpoint 
    }



    if(S2_first_time_1 == true ){ 
    int check_it = 0;
    for(i = 0;i<20;i++){
      
      if(LEDset_1[33+i] == 1) check_it = 1;  // margin of +- 10 in case the walker moves too fast
      
      }
    
    
    if(check_it == 1){
      int time_difference = duration - previous_checkpoint_time; // time spent walking in that section

          if(time_difference > 20){ // if above 20 seconds

             int flash_count = 0;
  
  while(flash_count < 4){ // flash orange color slowly 3-4 times 
     for (int i=0; i<150; i++){
    if (86 > i > 42){
      LEDS_1[i] = CRGB::Orange;
    }
    else{
      LEDS_1[i] = CRGB::Black;
    }
  }
 
  for(int i = 0;i<150;i++){
    LEDS_1[i] ) CRGB::Black;
    
    }
    flash_count++;
     delay(1000);
    }   
            }
      
      }
    
    
    S2_first_time_1 == false; // already entered once so do not enter this loop again
    previous_checkpoint_time = duration; // update checkpoint 
    }

    

     if(S1_first_time_1 == true ){ // Checking if the 86th index  and if so send orange flash lights if the person walked the area in more than 20 seconds 
    int check_it = 0;
    for(i = 0;i<20;i++){
      
      if(LEDset_1[76+i] == 1) check_it = 1;  // margin of +- 10 in case the walker moves too fast, reason for having 76+i rather than 86+i
      
      }
    
    
    if(check_it == 1){
      int time_difference = duration - previous_checkpoint_time; // time spent walking in that section

          if(time_difference > 20){ // if above 20 seconds

             int flash_count = 0;
  
  while(flash_count < 4){ // flash orange color slowly 3-4 times 
     for (int i=0; i<150; i++){
    if ( i > 76){
      LEDS_1[i] = CRGB::Orange;
    }
    else{
      LEDS_1[i] = CRGB::Black;
    }
  }
 
  for(int i = 0;i<150;i++){
    LEDS_1[i] ) CRGB::Black;
    
    }
    flash_count++;
     delay(1000);
    }   
            }
      
      }
    
    
    S1_first_time_1 == false; // already entered once so do not enter this loop again
    previous_checkpoint_time = duration; // update checkpoint 
    }





        
        
        
        
        
        
        
        // AFTER X AMOUNT OF MINUTES PASSED, RESET THE PROGRAM COULD BE ADDED HERE 
        
        
        }
    
               
      }
    

      }










  

}
