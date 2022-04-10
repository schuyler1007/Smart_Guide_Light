#include <FastLED.h>

#define NUM_LED 150
#define DATA_PIN 4
#define CLOCK_PIN 13
#define ONE_SET 15
#define echoPin 2
#define trigPin 3

int arr[NUM_LED] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int dist;
int distance;
int duration;
int s_num;
int sum = 0;
int current_distance = 0;
int previous_checkpoint_time = 5; // Giving 5 seconds(assuming duration is in seconds) in advance so we dont have to rush things as soon as we upload the program 

// Section first time entrances

bool S1_first_time true;
bool S2_first_time true;
bool S3_first_time true;



CRGB leds[NUM_LED];

void setup() {
  
 
  FastLED.addLeds<NEOPIXEL, DATA_PIN> (leds, NUM_LED);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600);



}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i=0; i<150; i++){
    arr[i] = 0;
  }
  sum = 0;
  for (int i=0; i<20; i++){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;
    sum += distance;
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
  distance = sum/20;

  // setting up the bitset
  s_num = distance/3.3;
  Serial.println("s_num: ");
  Serial.println(s_num);
  for (int i=0; i<20; i++){
    arr[i+s_num] = 1;
  }



// 0-----SECTION3-----43----SECTION2------86----SECTION1------
/* 
 *  0,43,86 represent the bitset indexes where we read the time and measure the time spent in previous section. This program assumes that the person is walking from afar so section 1,2 and 3 will be triggered in order.
 *  There are 3 different code blocks  each taking care of the section flash lighting. And they have bool first time entrance mechanism to avoid triggering it twice, we expect the walker to always move forward.
 *  
 *  
 *  
 *  
 *  
 *  
 */
  

  if(current_distance < distance){ // REGULAR GREEN LIGHTING, we might add some constant to (current_distance < distance) equation to avoid misreads
      
      
      for (int i=0; i<150; i++){
    if (arr[i] == 1){
      leds[i] = CRGB::Green;
    }
    else{
      leds[i] = CRGB::Black;
    }
  }

    }
    
    else { // THE FLASH EFFECT IF THE WALKER WALKS BACK
      
      int flash_count = 0;
  
  while(flash_count < 10){
     for (int i=0; i<150; i++){
    if (arr[i] == 1){
      leds[i] = CRGB::Red;
    }
    else{
      leds[i] = CRGB::Black;
    }
  }
 
  for(int i = 0;i<150;i++){
    leds[i] ) CRGB::Black;
    
    }
    flash_count++;
     delay(500);
    }
      }

  if(S1_first_time == true ){ // Checking if the 86th index  and if so send orange flash lights if the person walked the area in more than 20 seconds 
    int check_it = 0;
    for(i = 0;i<20;i++){
      
      if(leds[76+i] == 1) check_it = 1;  // margin of +- 10 in case the walker moves too fast
      
      }
    
    
    if(check_it == 1){
      int time_difference = duration - previous_checkpoint_time; // time spent walking in that section

          if(time_difference > 20){ // if above 20 seconds

             int flash_count = 0;
  
  while(flash_count < 4){ // flash orange color slowly 3-4 times 
     for (int i=0; i<150; i++){
    if (i>= 86){
      leds[i] = CRGB::Orange;
    }
    else{
      leds[i] = CRGB::Black;
    }
  }
 
  for(int i = 0;i<150;i++){
    leds[i] ) CRGB::Black;
    
    }
    flash_count++;
     delay(1000);
    }   
            }
      
      }
    
    
    S1_first_time == false; // already entered once so do not enter this loop again
    previous_checkpoint_time = duration; // update checkpoint 
    }



      if(S2_first_time == true ){ // Same orange flashlight but for Section 2, index 43-86 
    int check_it = 0;
    for(i = 0;i<20;i++){
      
      if(leds[33+i] == 1) check_it = 1;  // margin of +- 10 in case the walker moves too fast
      
      }
    
    
    if(check_it == 1){
      int time_difference = duration - previous_checkpoint_time; // time spent walking in that section

          if(time_difference > 20){ // if above 20 seconds

             int flash_count = 0;
  
  while(flash_count < 4){ // flash orange color slowly 3-4 times 
     for (int i=0; i<150; i++){
    if ( 86 > i > 42){
      leds[i] = CRGB::Orange;
    }
    else{
      leds[i] = CRGB::Black;
    }
  }
 
  for(int i = 0;i<150;i++){
    leds[i] ) CRGB::Black;
    
    }
    flash_count++;
     delay(1000);
    }   
            }
            
      }
    
    S2_first_time == false;
    previous_checkpoint_time = duration; // update checkpoint 
    }

    



    if(S3_first_time == true ){ // Orange flashlight for section 3, index 0-43
    int check_it = 0;
    for(i = 0;i<20;i++){
      
      if(leds[i] == 1) check_it = 1;  
      
      }
    
    
    if(check_it == 1){
      int time_difference = duration - previous_checkpoint_time; // time spent walking in that section

          if(time_difference > 20){ // if above 20 seconds

             int flash_count = 0;
  
  while(flash_count < 4){ // flash orange color slowly 3-4 times 
     for (int i=0; i<150; i++){
    if ( 43 > i){
      leds[i] = CRGB::Orange;
    }
    else{
      leds[i] = CRGB::Black;
    }
  }
 
  for(int i = 0;i<150;i++){
    leds[i] ) CRGB::Black;
    
    }
    flash_count++;
     delay(1000);
    }   
            }
      
      
      
      }
    
    
    S3_first_time == false;
    previous_checkpoint_time = duration; // update checkpoint 
    }

    if(S3_first_time == false and (duration - previous_checkpoint_time > 20)){ // If every section have been passsed and significant amount of time have passed then reset the first times for new comers
      
      S1_first_time = true;
      S2_first_time = true;
      S3_first_time = true;      
      }


    
  
  FastLED.show();
  delay(500);
}



int calcDistance(){
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  return distance;
}
