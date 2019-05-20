#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN1 10
#define PIN2 11
#define PIN3 12
#define PIN4 13
#define PIN5 14
#include <SPI.h>
#include <SD.h>
#include "TMRpcm.h"
#define SD_ChipSelectPin 4
const int numRows = 5;
const int numCols = 5;
int oldStartx;
int oldStarty;
int newStartx;
int newStarty;
int goalx=5;
int goaly=6;
const int rowPins[numRows] = {5, 6, 7, 8,9}; 
const int colPins[numCols] = {10, 11, 12, 13,14};
const int debounceTime = 20;
Adafruit_NeoPixel strip1;
Adafruit_NeoPixel strip2;
Adafruit_NeoPixel strip3;
Adafruit_NeoPixel strip4;
Adafruit_NeoPixel strip5;
Adafruit_NeoPixel ledLine;
Adafruit_NeoPixel pixelStrings[] = {
  strip1=Adafruit_NeoPixel(160, PIN1, NEO_GRB + NEO_KHZ800),
  strip2=Adafruit_NeoPixel(160, PIN2, NEO_GRB + NEO_KHZ800),
  strip3=Adafruit_NeoPixel(160, PIN3, NEO_GRB + NEO_KHZ800),
  strip4=Adafruit_NeoPixel(160, PIN4, NEO_GRB + NEO_KHZ800),
  strip5=Adafruit_NeoPixel(160, PIN5, NEO_GRB + NEO_KHZ800),
};
TMRpcm tmrpcm;
void setup() {
 //  pinMode(BUTTON1,INPUT);
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  tmrpcm.speakerPin=20;//Speaker Pin
  Serial.begin(9600);
  for(int i=0;i<5;i++){ //Initialize LED strip
        pixelStrings[i].begin();
        pixelStrings[i].show();
    }
  for(int row=0;row<numRows;row++){ //Initiazlize mats
    pinMode(rowPins, INPUT);
    digitalWrite(rowPins[row],LOW);
   }
  for(int column=0;column<numCols;column++){ //Initiazlize mats
    pinMode(colPins, INPUT);
    digitalWrite(colPins[column],LOW);
   }
  if(!SD.begin(SD_ChipSelectPin)) //SD card ready
  {
     Serial.println("SD fail");
     return;
   }
}
int costarray[3][2] = {};// initialize the container array
int XOption;
int YOption;
int num;// for next step
int num2; // for music
int num3;

int LedStaringPoint;
String musicName;// Store music name 
String musicName2;
void loop() {
  //control button PIN
  for(int column = 0; column < numCols; column++){
      for(int row = 0 ; row < numRows; row++){
           if(digitalRead(colPins[column]) == HIGH && digitalRead(rowPins[row])==HIGH){
                if(colPins[column]==5){
                    oldStartx=colPins[column];
                    oldStarty=rowPins[row];
                    costarray[0][0] = oldStartx+1;
                    costarray[0][1] = oldStarty;
                    costarray[1][0] = oldStartx;
                    costarray[1][1] = oldStarty+1;
                    costarray[2][0] = oldStartx+1;
                    costarray[2][1] = oldStarty+1;
                    num=random(0,2);// Generate the random number for next step
                    num2=random(1,3);// Generate the random number for music
                    XOption=costarray[num][0];
                    YOption=costarray[num][1];
                    tmrpcm.setVolume(6);
                    musicName=num2+String(".wav");
                    tmrpcm.play(musicName.c_str ());
                    for(int i=0; i<pixelStrings[column].numPixels(); i++) {
                             pixelStrings[column].setPixelColor((row-1)*32,(row*32),0, 0, 128);
                     }
                    pixelStrings[column].show();
                    Serial.println(String("position:")+colPins[column]+String("position:")+rowPins[row]);
                    return XOption,YOption;
                  }
                 if((rowPins[row] == YOption) && (colPins[column] == XOption)){
                    newStartx=colPins[column];
                    newStarty=rowPins[row];
                    LedStaringPoint=abs(newStarty-oldStarty)*160+abs(newStartx-oldStartx)*32;
                    costarray[0][0] = newStartx+1;
                    costarray[0][1] = newStarty;
                    costarray[1][0] = newStartx;
                    costarray[1][1] = newStarty+1;
                    costarray[2][0] = newStartx+1;
                    costarray[2][1] = newStarty+1;
                    num=random(0,2);
                    XOption=costarray[num][0];
                    YOption=costarray[num][1];
                    tmrpcm.setVolume(6);
                    // tmrpcm.play("001.wav");
                    musicName=num2+String(".wav");
                    tmrpcm.play(musicName.c_str ());
                    for(int i=0; i<pixelStrings[column].numPixels(); i++) {
                             pixelStrings[column].setPixelColor((row-1)*32,(row*32),0, 0, 128);
                     }
                    pixelStrings[column].show();
//                    rainbow(3);
                    Serial.println("Button1 1");
                    return XOption,YOption;
                  }
            }else{
                     num3=random(4,5); //generate noise sound
                     musicName2=num3+String(".wav");
                     tmrpcm.play(musicName2.c_str ());
                     for(int i=0; i<pixelStrings[column].numPixels(); i++) {
                             pixelStrings[column].setPixelColor((row-1)*32,(row*32),0, 0, 128);
                     }
                     pixelStrings[column].show();
                     delay(3);
                 }
             }
        }
  delay(1000);
}

//void colorWipe(uint32_t c, uint8_t wait) {
//  for(uint16_t i=0; i<strip.numPixels(); i++) {
//    strip.setPixelColor(i, c);
//    strip.show();
//    delay(wait);
//  }
//}
//void rainbow(uint8_t wait,strip) {
//  uint16_t i, j;
//  for(j=0; j<256; j++) {
//    for(i=0; i<strip.numPixels(); i++) {
//      strip.setPixelColor(i, 0, 0, 128);
//    }
//    strip.show();
//    delay(wait);
//  }
//}

// Slightly different, this makes the rainbow equally distributed throughout
//void rainbowCycle(uint8_t wait) {
//  uint16_t i, j;
//  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
//    for(i=0; i< strip.numPixels(); i++) {
//      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
//    }
//    strip.show();
//    delay(wait);
//  }
//}

//Theatre-style crawling lights.
//void theaterChase(uint32_t c, uint8_t wait) {
//  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
//    for (int q=0; q < 3; q++) {
//      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, c);    //turn every third pixel on
//      }
//      strip.show();
//      delay(wait);
//      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, 0);        //turn every third pixel off
//      }
//    }
//  }
//}

//Theatre-style crawling lights with rainbow effect
//void theaterChaseRainbow(uint8_t wait) {
//  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
//    for (int q=0; q < 3; q++) {
//      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
//      }
//      strip.show();
//
//      delay(wait);
//      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, 0);        //turn every third pixel off
//      }
//    }
//  }
//}
//uint32_t Wheel(byte WheelPos) {
//  WheelPos = 255 - WheelPos;
//  if(WheelPos < 85) {
//    return  pixelStrings[colPins[column]].Color(255 - WheelPos * 3, 0, WheelPos * 3);
//  }
//  if(WheelPos < 170) {
//    WheelPos -= 85;
//    return  pixelStrings[colPins[column]].Color(0, WheelPos * 3, 255 - WheelPos * 3);
//  }
//  WheelPos -= 170;
//  return  pixelStrings[colPins[column]].Color(WheelPos * 3, 255 - WheelPos * 3, 0);
//}
