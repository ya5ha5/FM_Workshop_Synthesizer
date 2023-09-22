/* 
Buttons for Sound

Yashas Shetty
2023




*/

#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/sin2048_int8.h> // sine table for oscillator

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);
//Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin2(SIN2048_DATA);

const int buttonPin4 = 4; 
const int buttonPin3 = 3;
const int buttonPin2 = 2;
const int buttonPin1 = 1;
const int buttonPin0 = 0;


int buttonState4 = 0;
int buttonState3 = 0;
int buttonState2 = 0;
int buttonState1 = 0;
int buttonState0 = 0;

// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 64 // Hz, powers of 2 are most reliable


void setup(){
  startMozzi(CONTROL_RATE); // :)
  //aSin.setFreq(440); // set the frequency
  pinMode(buttonPin4, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin0,INPUT_PULLUP);
}


void updateControl(){
  buttonState4 = digitalRead(buttonPin4);
  buttonState3 = digitalRead(buttonPin3);
  buttonState2 = digitalRead(buttonPin2);
  buttonState1 = digitalRead(buttonPin1);
  buttonState0 = digitalRead(buttonPin0);


if (buttonState0 == LOW) {
  aSin.setFreq(680);
  
  } else if
 (buttonState3 == LOW) {
  aSin.setFreq(780);
    
 }
else if
(buttonState4 == LOW)
{
  aSin.setFreq(480);
}

else if
(buttonState2 == LOW)
{
  aSin.setFreq(380);
}

else if
(buttonState1 == LOW)
{

  aSin.setFreq(280);
}

else if
(buttonState1 == HIGH && buttonState2 == HIGH && buttonState3 == HIGH && buttonState4 == HIGH && buttonState0 ==HIGH)
{

  aSin.setFreq(0);
}







}


AudioOutput_t updateAudio(){
  return MonoOutput::from8Bit(aSin.next()); // return an int signal centred around 0
}


void loop(){
  audioHook(); // required here
}
