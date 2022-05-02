#include <DFRobot_LIDAR07.h>
#include <FastLED.h>
#include <Wire.h>
#include <string.h>

#define NUM_LEDS 150
#define DATA_PIN 4
#define DIR_BUTTON_PIN 5
#define SET_BUTTON_PIN 6
#define INTERVAL_BUTTON_PIN 7
#define CLOCK_PIN 13
#define ONE_SET 15

CRGB leds[NUM_LEDS];

DFROBOT_LIDAR07_IIC LIDAR07;
int *arr = (int*) calloc(NUM_LEDS, 1);
int distance = 0, prev_distance = 0;
unsigned long millsec = 0, prev_millsec = 0;
int position_cnt = 0;
int s_num;
char mode = 'g';
char dir = 'r'; // r for right to left, l for left to right
int pre_dir = 0;
int dir_state = 0;
char setting = 'm'; // m for museum mode, d for light mode
int pre_setting = 0;
int setting_state = 0;
int interval = 5;
int pre_interval = 0;
int interval_state = 0;

// directional lights

uint8_t hue = 171; // this changes the color, you can look up hue to color conversions.


void setup() {
    // put your setup code here, to run once:
    pinMode(DIR_BUTTON_PIN, INPUT);
    pinMode(SET_BUTTON_PIN, INPUT);
    pinMode(INTERVAL_BUTTON_PIN, INPUT);
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    Serial.begin(115200);
    directional_lights(dir);
    while(!LIDAR07.begin()){
        Serial.println("The sensor returned data validation error");
        delay(1000);
    }
direectional_ligths(dir);
  
}

void loop() {
    for (int i=0; i<NUM_LEDS; i++){
        arr[i] = 0;
    }
    set_direction();
    set_setting();
    set_interval();
    // put your main code here, to run repeatedly:
    prev_distance = distance;
    distance = calc_distance();
    s_num = distance / 3.3;
    if (setting == 'm') {
        millsec = millis();
        check_position();
        check_mode();

        switch (mode) {
            case 'g':
                green_light();
                break;
            case 'r':
                red_light();
                break;
            case 'o':
                orange_light();
                break;
            default:
                green_light();
        }
    }
    else{
        white_light();
    }
}

void set_direction(){
    dir_state = digitalRead(DIR_BUTTON_PIN);
    if (dir_state == HIGH && pre_dir == 0){
        if (dir == 'r'){
            dir = 'l';
        }
        else{
            dir = 'r';
        }
        delay(100);
        pre_dir = 1;
    }
    else{
        pre_dir = 0;
    }
}

void set_setting(){
    setting_state = digitalRead(SET_BUTTON_PIN);
    if (setting_state == HIGH && pre_setting == 0){
        if (setting == 'm'){
            setting = 'd';
        }
        else{
            setting = 'm';
        }
        delay(100);
        pre_setting = 1;
    }
    else{
        pre_setting = 0;
    }
}

void set_interval(){
    interval_state = digitalRead(INTERVAL_BUTTON_PIN);
    if (interval_state == HIGH && pre_interval == 0){
        if (interval == 10){
            interval = 0;
        }
        interval++;
        delay(100);
        pre_interval = 1;
    }
    else{
        pre_interval = 0;
    }
}


void green_light(){
    if (dir == 'r'){
        for (int i=0; i<20; i++){
            arr[s_num-i] = 1;
        }
    }
    else{
        for (int i=0; i<20; i++){
            arr[i+s_num] = 1;
        }
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

void red_light(){
    if (dir == 'r'){
        for (int i=0; i<20; i++){
            arr[s_num+i] = 1;
        }
    }
    else{
        for (int i=0; i<20; i++){
            arr[s_num-i] = 1;
        }
    }
    for (int j=0; j<10; j++){
        for (int i=0; i<150; i++){
            if (arr[i] == 1){
                leds[i] = CRGB::Red;
            }
            else{
                leds[i] = CRGB::Black;
            }
        }
        FastLED.show();
        delay(25);
        for (int i=0; i<150; i++){
            leds[i] = CRGB::Black;
        }
        FastLED.show();
        delay(25);
    }
}

void orange_light(){
    for (int i=0; i<10; i++) {
        arr[s_num - i] = 1;
        arr[s_num + i] = 1;
    }
    for (int j=0; j<5; j++){
        for (int i=0; i<150; i++){
            if (arr[i] == 1){
                leds[i] = CRGB::Orange;
            }
            else{
                leds[i] = CRGB::Black;
            }
        }
        FastLED.show();
        delay(50);
        for (int i=0; i<150; i++){
            leds[i] = CRGB::Black;
        }
        FastLED.show();
        delay(50);
    }
}

void white_light(){
    if (dir == 'r'){
        for (int i=0; i<20; i++){
            arr[s_num-i] = 1;
        }
    }
    else{
        for (int i=0; i<20; i++){
            arr[i+s_num] = 1;
        }
    }
    for (int i=0; i<150; i++){
        if (arr[i] == 1){
            leds[i] = CRGB::White;
        }
        else{
            leds[i] = CRGB::Black;
        }
    }
    FastLED.show();
    delay(500);
}

void check_position(){
    // if it is staying at the same place (within 10cm), it needs to change the mode to orange
    // if it is not staying at the same place, it needs to reset the timer
    if (abs(distance - prev_distance) <= 5){
        return;
    }
    else{
        prev_millsec = millis();
        Serial.println("Resetting timer");
    }
}

void check_mode(){
    if (prev_distance - distance >= 20 && dir == 'r'){
        mode = 'r'; // when it should not be coming towards the sensor
    }
    else if (distance - prev_distance >= 20 && dir == 'l'){
        mode = 'r'; // when it should not be going away from the sensor
    }
    else if (millsec - prev_millsec >= interval*1000){
        mode = 'o';
    }
    else{
        mode = 'g';
    }
}

int calc_distance(){
    int errinfo;
    while(!LIDAR07.startMeasure()){
        Serial.print("Incorrect data was returned");
        delay(1000);
    }
    //Serial.print("Distance: ");
    //Serial.print(LIDAR07.getDistanceMM() / 10);
    ///Serial.println(" cm");
    delay(500);
    return (LIDAR07.getDistanceMM() / 10);
}













void directional_lights(char dir){
int current_time  = millis();

            while(millis()- current_time < 10000){

                uint8_t pos = map(beat8(20, 0), 0, 255, 0, NUM_LEDS -1); // for changing speed play around with the frequency value '20' in this line.
      if(dir == 'l'){  // IF  THE DIRECTION IS REVERSE CHANGE THIS TO 'l'
          leds[pos] = CHSV( hue, 200, 255);

          fadeToBlackBy(leds, NUM_LEDS,30);
   
          FastLED.show();
 
 

        
        }
        else{
          leds[NUM_LEDS-pos] = CHSV( hue, 200, 255);

          fadeToBlackBy(leds, NUM_LEDS,20);
   
          FastLED.show();
          
          
          }


            }

  
  }
