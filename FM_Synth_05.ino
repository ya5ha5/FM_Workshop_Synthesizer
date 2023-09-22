/*
A simple synthesizer for workshops/



Yashas Shetty
2023




*/

#include <MozziGuts.h>
#include <Oscil.h> // oscillator
#include <tables/cos2048_int8.h> // table for Oscils to play
#include <AutoMap.h> // maps unpredictable inputs to a range
#include <tables/sin2048_int8.h> // sine table for oscillator

// All constant buttons for Notes

const int buttonA = 0; // Connect to digital pin 2
const int buttonAsharp = 2; // Connect to digital pin 3
const int buttonC = 3; // Connect to digital pin 4
const int buttonD = 4; // Connect to digital pin 5
const int buttonE = 5; // Connect to digital pin 6



// desired carrier frequency max and min, for AutoMap
const int MIN_CARRIER_FREQ = 22;
const int MAX_CARRIER_FREQ = 440;

// desired intensity max and min, for AutoMap, note they're inverted for reverse dynamics
const int MIN_INTENSITY = 700;
const int MAX_INTENSITY = 10;

AutoMap kMapCarrierFreq(0,1023,MIN_CARRIER_FREQ,MAX_CARRIER_FREQ);
AutoMap kMapIntensity(0,1023,MIN_INTENSITY,MAX_INTENSITY);

const int KNOB_PIN = 0; // set the input for the knob to analog pin 0
const int LDR_PIN = 1; // set the input for the LDR to analog pin 1
const int GAIN_KNOB = 2;

Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCarrier(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aModulator(COS2048_DATA);
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);


int mod_ratio = 3; // harmonics
long fm_intensity; // carries control info from updateControl() to updateAudio()
byte gain = 255; // Initial Gain Value

void setup(){
  //Serial.begin(9600); // for Teensy 3.1, beware printout can cause glitches
  Serial.begin(115200); // set up the Serial output so we can look at the piezo values // set up the Serial output for debugging
  startMozzi(); // :))
}


void updateControl(){

// Change Note Frequencies according to button pressed

if (buttonA == LOW) {
    Serial.println("Button 1 is pressed");
    aSin.setFreq(440);
  } else {
    Serial.println("Button 1 is not pressed");
    aSin.setFreq(0);
  }

  if (buttonAsharp == LOW) {
    Serial.println("Button 2 is pressed");
     aSin.setFreq(466);
  } else {
    Serial.println("Button 2 is not pressed");
    aSin.setFreq(0);
  }

  if (buttonC == LOW) {
    Serial.println("Button 3 is pressed");
    aSin.setFreq(523);
  } else {
    Serial.println("Button 3 is not pressed");
    aSin.setFreq(0);
  }

  if (buttonD == LOW) {
    Serial.println("Button 4 is pressed");
    aSin.setFreq(587);
  } else {
    Serial.println("Button 4 is not pressed");
    aSin.setFreq(0);
  }

  if (buttonE == LOW) {
    Serial.println("Button 5 is pressed");
    aSin.setFreq(659);
  } else {
    Serial.println("Button 5 is not pressed");
    aSin.setFreq(0);
  }

// read the knob
  int knob_value = mozziAnalogRead(KNOB_PIN); // value is 0-1023
int gain_value = mozziAnalogRead(GAIN_KNOB);
gain = map(gain_value,0,1023,0,255);
  // map the knob to carrier frequency
  int carrier_freq = kMapCarrierFreq(knob_value);

  //calculate the modulation frequency to stay in ratio
  int mod_freq = carrier_freq * mod_ratio;

  // set the FM oscillator frequencies to the calculated values
  aCarrier.setFreq(carrier_freq);
  aModulator.setFreq(mod_freq);

  // read the light dependent resistor on the Analog input pin
  int light_level= mozziAnalogRead(LDR_PIN); // value is 0-1023

  // print the value to the Serial monitor for debugging
  Serial.print("light_level = ");
  Serial.print(light_level);
  Serial.print("\t"); // prints a tab

  fm_intensity = kMapIntensity(light_level);

  Serial.print("fm_intensity = ");
  Serial.print(fm_intensity);
  Serial.println(); // print a carraige return for the next line of debugging info



}






AudioOutput_t updateAudio(){
//  long modulation = fm_intensity * aModulator.next();
 // return MonoOutput::from8Bit(aCarrier.phMod(modulation)); // phMod does the FM
return MonoOutput::from8Bit(aSin.next() * gain); // return an int signal centred around 0



}


void loop(){
  audioHook();
}
