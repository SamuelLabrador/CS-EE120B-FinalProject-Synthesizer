#include <MIDI.h>  // Add Midi Library
#define LED 13    

MIDI_CREATE_DEFAULT_INSTANCE();

unsigned char mostRecentNote = 0x00;

void setup() {
  pinMode (LED, OUTPUT); 
  MIDI.begin(MIDI_CHANNEL_OMNI); 
  MIDI.setHandleNoteOn(MyHandleNoteOn); 
  MIDI.setHandleNoteOff(MyHandleNoteOff);   
}

void loop() { 
  MIDI.read();
}

void MyHandleNoteOn(byte channel, byte pitch, byte velocity) { 
  digitalWrite(LED,HIGH);  //Turn LED on
  if(mostRecentNote == 0){
    Serial.write(pitch);
    mostRecentNote = pitch;
  }
}

void MyHandleNoteOff(byte channel, byte pitch, byte velocity) { 
  digitalWrite(LED,LOW);  //Turn LED off
  if(pitch == mostRecentNote){ //comment 
    Serial.write(0x00);    //this is for sending my guy
    mostRecentNote = 0;  //cinmmentary
  }
  
}

