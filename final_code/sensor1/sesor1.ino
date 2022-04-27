#include <DFRobot_LIDAR07.h>
#include <FastLED.h>
#include <Wire.h>
#include <string.h>

#define NUM_LEDS 150
#define DATA_PIN 4
#define DIR_BUTTON_PIN 5
#define SET_BUTTON_PIN 6
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
char setting = 'm'; // m for museum mode, d for light mode
int interval = 10;


void setup() {
    // put your setup code here, to run once:
    pinMode(DIR_BUTTON_PIN, INPUT);
    pinMode(SET_BUTTON_PIN, INPUT);
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
    prev_distance = distance;
    distance = calc_distance();
    millsec = millis();
    s_num = distance / 3.3;
    //Serial.print("s_num: ");
    //Serial.println(s_num);
    //check_direction();
    check_position();
    //check_duration();
    check_mode();

    switch (mode){
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
    //Serial.print("prev_distance: ");
    //Serial.println(prev_distance);

}

void set_direction(){
    if (digitalRead(DIR_BUTTON_PIN) == HIGH){
        dir = 'r';
    }
    else{
        dir = 'l';
    }
}

void set_setting(){
    if (digitalRead(SET_BUTTON_PIN) == HIGH){
        dir = 'm';
    }
    else{
        dir = 'd';
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
