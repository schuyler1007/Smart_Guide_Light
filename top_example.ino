#include <FastLED.h>
#include <string.h>

// LEDs
#define NUM_LEDS 150
#define DATA_PIN 4
#define CLOCK_PIN 13
#define ONE_SET 15

// Sensors
#define echoPin 2
#define trigPin 3

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
int dist;
char on_off[15] = "0000000000";

CRGB leds[NUM_LEDS];



void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); 
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
}

void loop() {
  // put your main code here, to run repeatedly:
  dist = calcDistance();
  Serial.print("Dist: ");
  Serial.println(distance);
  delay(100);
  if (dist < 50){
    strcpy(on_off, "1000000000");
  }
  else if (dist < 100){
    strcpy(on_off, "1100000000");
  }
  else if (dist < 150){
    strcpy(on_off, "1110000000");
  }
  else if (dist < 200){
    strcpy(on_off, "0111000000");
  }
  else if (dist < 250){
    strcpy(on_off, "0011100000");
  }
  else if (dist < 300){
    strcpy(on_off, "0001110000");
  }
  else if (dist < 350){
    strcpy(on_off, "0000111000");
  }
  else if (dist < 400){
    strcpy(on_off, "0000011100");
  }
  else if (dist < 450){
    strcpy(on_off, "0000001110");
  }
  else if (dist < 500){
    strcpy(on_off, "0000000111");
  }
  delay(1000);
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

void lightup(int section){
  for (int i=section; i<section+ONE_SET; i++){
    leds[i] = CRGB::Green;
  }
  FastLED.show();
}

void lightdown(int section){
  // Now turn the LED off, then pause
  for (int i=section; i<section+ONE_SET; i++){
    leds[i] = CRGB::Black; 
  }
  FastLED.show();
}

void light_on_off(char on_off[10]){
  for (int i=0; i<10; i++){
    if (on_off[i] == '0'){
      lightdown(i);
    }
    else{
      lightup(i);
    }
  }
}
