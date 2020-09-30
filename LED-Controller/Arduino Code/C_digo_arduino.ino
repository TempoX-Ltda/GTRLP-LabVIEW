/*
 * SerialReceiveMultipleFields sketch
 * This code expects a message in the format: 12,345,678
 * This code requires a newline character to indicate the end of the data
 * Set the serial monitor to send newline characters
 */
#include <FastLED.h>

// How many leds are in the strip?
#define NUM_LEDS 144

#define DATA_PIN 3

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

const int NUMBER_OF_FIELDS = 4; // how many comma separated fields we expect
int fieldIndex = 0;            // the current field being received
int values[NUMBER_OF_FIELDS];   // array holding values for all the fields



void setup()
{
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);

  for(int LedNum = 0; LedNum < NUM_LEDS; LedNum = LedNum + 1) {
      leds[LedNum] = CRGB::White; 
  }
  FastLED.show();
  
  Serial.begin(9600); // Initialize serial port to send and receive at 9600 baud
}

void loop()
{
  if( Serial.available())
  {
    char ch = Serial.read();
    if(ch >= '0' && ch <= '9') // is this an ascii digit between 0 and 9?
    {
      // yes, accumulate the value
      values[fieldIndex] = (values[fieldIndex] * 10) + (ch - '0'); 
    }
    else if (ch == ',')  // comma is our separator, so move on to the next field
    {
      if(fieldIndex < NUMBER_OF_FIELDS-1)
        fieldIndex++;   // increment field index
    }
    else
    {
      // any character not a digit or comma ends the acquisition of fields
      // in this example it's the newline character sent by the Serial Monitor
      Serial.print( fieldIndex +1);
      Serial.println(" fields received:");

      for(int LedNum = 0; LedNum < NUM_LEDS; LedNum = LedNum + 1) {
        leds[LedNum] = CRGB(values[1], values[2], values[3]);
      }

      FastLED.setBrightness(values[0]);
      
      FastLED.show();
      for(int i=0; i <= fieldIndex; i++)
      {
        Serial.println(values[i]);
        
        values[i] = 0; // set the values to zero, ready for the next message
      }
      fieldIndex = 0;  // ready to start over
    }
  }
}
