/*
  Melody
  Plays a melody
  circuit:
  - 8 ohm speaker on digital pin 8
  created 21 Jan 2010
  modified 30 Aug 2011
  by Tom Igoe
  This example code is in the public domain.
  http://www.arduino.cc/en/Tutorial/Tone
*/
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN 10
#include <SPI.h>
#include <SD.h>
#include "TMRpcm.h"
#define SD_ChipSelectPin 4
//int BUTTON1 = 6;//button PIN
//int LEDState = LOW;
const int numRows = 4;
const int numCols = 4;
const int LEDPin = 13;
const int rowPins[numRows] = {4, 5, 6, 7}; 
const int colPins[numCols] = {8, 9, 10, 11};
const int debounceTime = 20;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
TMRpcm tmrpcm;
void setup() {
 //  pinMode(BUTTON1,INPUT);
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  tmrpcm.speakerPin=9;//Speaker Pin
  Serial.begin(9600);
  for(int row=0;row<numRows;row++){
    pinMode(rowPins, INPUT);
    digitalWrite(rowPins[row],LOW);
   }
   for(int column=0;column<numCols;column++){
    pinMode(rowPins, INPUT);
    digitalWrite(colPins[column],LOW);
   }
   
  if(!SD.begin(SD_ChipSelectPin))
  {
     Serial.println("SD fail");
     return;
   }
}

void loop() {
  //control button PIN
  for(int column = 0; column < numCols; column++){
      for(int row = 0 ; row < numRows; row++){
           if(digitalRead(colPins[column]) == HIGH && digitalRead(rowPins[row])==HIGH){
               tmrpcm.setVolume(6);
               tmrpcm.play("001.wav");
               strip.begin();
               strip.show();
               rainbow(3);
               Serial.println("Button1 1");
            }else{
                theaterChase(strip.Color(0, 0, 0), 50);
                 }
             }
        }
  delay(1000);
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
      delay(wait);
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
