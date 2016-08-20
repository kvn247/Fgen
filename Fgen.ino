/*
  basic heart waveform generator
  4 buttons to choose 4 different heart activities


 */

#include "Waveforms.h"

//penis penis

#define bradycardia 2000000/maxSamplesNum  // ~= 29.8bpm
#define tachycardia 2000                   // ~= 240bpm
#define resting 8000                       // ~= 61bpm 


const int button0 = 2, button1 = 3, button2 = 4, button3 = 5;
volatile int wave0 = 0, wave1 = 0;

int i, j= 0;
unsigned long peak1, peak2 = 0;
int sample;
int dead;


void setup() {
  Serial.begin(9600);
  analogWriteResolution(12);  // set the analog output resolution to 12 bit (4096 levels)
  analogReadResolution(12);   // set the analog input resolution to 12 bit 
  sample = resting;
  dead = 0;
  
  //interrupts to change the heart rate
  attachInterrupt(button0, changeToBrady, RISING);  
  attachInterrupt(button1, changeToTachy, RISING); 
  attachInterrupt(button2, changeToResting, RISING); 
  attachInterrupt(button3, changeToDead, RISING);
  
}

void loop() {
  // Read the the potentiometer and map the value  between the maximum and the minimum sample available
  // 1 Hz is the minimum freq for the complete wave
  // 170 Hz is the maximum freq for the complete wave. Measured considering the loop and the analogRead() time
  
//  sample = map(analogRead(A0), 0, 4095, 2000, oneHzSample);
//  sample = constrain(sample, 0, oneHzSample);

    
  if(!dead){
    analogWrite(DAC0, waveformsTable[wave0][i]);  // write the selected waveform on DAC0
  
    i++;
    if(i == maxSamplesNum){  // Reset the counter to repeat the wave
      i = 0;
      if(j == 0)
        j = 1;
      else 
        j = 0;  
    }
    
    //peak detection and heart rate
    if(j == 0)
      if(waveformsTable[wave1][i] == 0xfff){
        peak1 = micros();
     Serial.println((float)60000000/(peak1-peak2), 4);      
      }
        
     if(j==1)  
      if(waveformsTable[wave1][i] == 0xfff){
        peak2 = micros();
      Serial.println((float)60000000/(peak2-peak1), 4); 
      }
      
  
     
  
    delayMicroseconds(sample);  // Hold the sample value for the sample time
  }
}

// interrupt handlers

void changeToTachy() {
 sample = tachycardia;
}

void changeToBrady() {
 sample = bradycardia;
}

void changeToResting(){
 dead = 0;
 sample = resting; 
}

void changeToDead(){
  dead = 1; 
}
