#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/cos2048_int8.h> // table for Oscils to play
#include <Smooth.h>
#include <AutoMap.h> // maps unpredictable inputs to a range


// desired carrier frequency max and min, for AutoMap
const int MIN_CARRIER_FREQ = 22;
const int MAX_CARRIER_FREQ = 440;

// desired intensity max and min, for AutoMap, note they're inverted for reverse dynamics
const int MIN_INTENSITY = 700;
const int MAX_INTENSITY = 10;

// desired mod speed max and min, for AutoMap, note they're inverted for reverse dynamics
const int MIN_MOD_SPEED = 10000;
const int MAX_MOD_SPEED = 1;

AutoMap kMapCarrierFreq(0,1023,MIN_CARRIER_FREQ,MAX_CARRIER_FREQ);
AutoMap kMapIntensity(0,1023,MIN_INTENSITY,MAX_INTENSITY);
AutoMap kMapModSpeed(0,1023,MIN_MOD_SPEED,MAX_MOD_SPEED);

const int potPitchPin = A0;
const int potGainPin = A1;
const int potFilterPin = A2;
const int potDelayPin = A3;
const int potReverbPin = A4;

const int buttonA = 2; // Connect to digital pin 2
const int buttonAsharp = 3; // Connect to digital pin 3
const int buttonC = 4; // Connect to digital pin 4
const int buttonD = 5; // Connect to digital pin 5
const int buttonE = 6; // Connect to digital pin 6

const int audioOut = 9; // Audio output pin

Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCarrier(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aModulator(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, CONTROL_RATE> kIntensityMod(COS2048_DATA);

int pitch, gain, filterCutoff, delayTime, reverbTime;
int mod_ratio = 5; // brightness (harmonics)
long fm_intensity; // carries control info from updateControl to updateAudio

// smoothing for intensity to remove clicks on transitions
float smoothness = 0.95f;
Smooth <long> aSmoothIntensity(smoothness);


void setup() {
  startMozzi();
  pinMode(buttonA, INPUT_PULLUP);
  pinMode(buttonAsharp, INPUT_PULLUP);
  pinMode(buttonC, INPUT_PULLUP);
  pinMode(buttonD, INPUT_PULLUP);
  pinMode(buttonE, INPUT_PULLUP);
}

void updateControl() {

  pitch = mozziAnalogRead(potPitchPin);
  
  //Map pitch to carrier_freq

int carrier_freq = KMapCarrierFreq(pitch);


  //calculate the modulation frequency to stay in ratio
int mod_freq = carrier_freq * mod_ratio;
int pitch_calibrated = kMapIntensity(pitch);
// calculate the fm_intensity
fm_intensity = ((long)pitch_calibrated * (kIntensityMod.next()+128))>>8; // shift back to range after 8 bit mul

 // read the light dependent resistor on the speed Analog input pin
  int gain = mozziAnalogRead(potGainPin); // value is 0-1023
  float mod_speed = (float)kMapModSpeed(gain)/1000;





// set the FM oscillator frequencies
  aCarrier.setFreq(carrier_freq);
  aModulator.setFreq(mod_freq);


  gain = MozziAnalogRead(potGainPin);
  filterCutoff = MozziAnalogRead(potFilterPin);
  delayTime = MozziAnalogRead(potDelayPin);
  reverbTime = MozziAnalogRead(potReverbPin);
  kIntensityMod.setFreq(mod_speed);
}

AudioOutput_t updateAudio(){
  long modulation = aSmoothIntensity.next(fm_intensity) * aModulator.next();
  return MonoOutput::from8Bit(aCarrier.phMod(modulation));
}

void loop() {

/*

  int noteFrequency = 440; // Default to A
  if (digitalRead(buttonA) == LOW) {
    noteFrequency = 440; // A
  } else if (digitalRead(buttonAsharp) == LOW) {
    noteFrequency = 466; // A#
  } else if (digitalRead(buttonC) == LOW) {
    noteFrequency = 523; // C
  } else if (digitalRead(buttonD) == LOW) {
    noteFrequency = 587; // D
  } else if (digitalRead(buttonE) == LOW) {
    noteFrequency = 659; // E
  }

  waveform.setFreq(noteFrequency);

  while (true) {
    updateParams();
    waveform.setFreq(pitch);
    int out = waveform.next() * gain;
    audioHook();
  }*/

audioHook();

}
