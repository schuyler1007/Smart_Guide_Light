#include <FastLED.h>

#define NUM_LED 150
#define DATA_PIN 4
#define CLOCK_PIN 13
#define ONE_SET 15
#define echoPin 2
#define trigPin 3

// int arr[NUM_LED] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int *arr = (int*) calloc(NUM_LED, 1);

int dist;
int distance;
int duration;
int s_num;
int sum = 0;

CRGB leds[NUM_LED];

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, DATA_PIN> (leds, NUM_LED);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600);
}

void loop() {
    Serial.print("test calloc: ");
    Serial.println(arr[2]);

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
  
  s_num = distance/3.3;
  Serial.println("s_num: ");
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
