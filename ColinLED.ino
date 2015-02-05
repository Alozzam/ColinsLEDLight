

#include <TinyWireM.h>
#include <TinyRTClib.h>
// For debug, uncomment serial code, use a FTDI Friend with its RX pin connected to Pin 3 
// You will need a terminal program (such as freeware PuTTY for Windows) set to the
// USB port of the FTDI friend at 9600 baud.  Uncomment out Serial commands to see what's up
//#include <SendOnlySoftwareSerial.h>  // See http://forum.arduino.cc/index.php?topic=112013.0

#define R_PIN 0
#define G_PIN 1
#define B_PIN 2
#define NUMPIXELS 3


uint32_t currentColor;


uint32_t animTimer;
uint32_t colorFadeTimer;
uint32_t fadedColor;
uint8_t colorFadeSpeed = 1;

// store color values in program memory for blend sweep

uint32_t colors[] = {
  0x00FF0000,0x00FF0055,0x00FF00BB,0x00FF00FF,0x00BB00FF,0x005500FF,0x000000FF,0x000055FF,0x0000BBFF,0x0000FFFF,
  0x0000FFBB, 0x0000FF55 
};


void setup() {
 
   pinMode(R_PIN, OUTPUT);// sets the pin as output
    pinMode(G_PIN, OUTPUT);// sets the pin as output
     pinMode(B_PIN, OUTPUT);// sets the pin as output
}



void loop() {   
  
   for (int i = 0; i<NUMPIXELS; i++)
  {
   
     analogWrite(i, 0);  //0 to 255
  }
  
  
  
   colorFadeSpeed = 21;
     
  
  
  draw(currentColor);
  
  delay(25); // was 250. Caused occasional hesitations in changing of seconds.
  
  animTimer += 1;
  if(animTimer > 0x00FFFFFF)
  {
  animTimer = 0;
  }
  
  
  currentColor = getColorFade(colorFadeTimer );
  
  
 colorFadeTimer+=colorFadeSpeed;
if(colorFadeTimer>= 0x00000600)
{
colorFadeTimer= 0;
}

   
}



void draw(uint32_t color)
{
analogWrite(R_PIN, (uint8_t) (color >> 16));
analogWrite(G_PIN, (uint8_t) (color >> 8));
analogWrite(B_PIN, 255 );
}

uint8_t colorWipeCounter = 0;



uint32_t getColorFade(uint32_t counter)
{
uint32_t  normalizedCounter = counter& 0x000000FF; //remove the 'stage'
  
if(counter< 0x00000100){ //stage 1 - ramp up blue from purple
return (normalizedCounter& 0x000000FF) | 0x00FF0000;
 
}else if(counter< 0x00000200){//stage 2 - ramp down the red for pure blue

return  0x000000FF | ( (0x00FF0000 - normalizedCounter<< 16)) ;
  
}else if(counter< 0x00000300){//stage 3 - ramp up the green for teal

return  ((normalizedCounter<<8) & 0x0000FF00) | 0x000000FF;
  
}else if(counter< 0x00000400){//stage 4 - ramp down the blue for pure green

return  0x0000FF00 | ((0x000000FF - normalizedCounter)) ;

}else if(counter< 0x00000500){//stage 5 - ramp up the red for yellow

return  ((normalizedCounter<<16) & 0x00FF0000) | 0x0000FF00;

}else if(counter< 0x00000600){//stage 6 - ramp down the green for pure red

return  0x00FF0000 | (0x0000FF00 & (0x0000FF00 - (normalizedCounter<< 8))) ;

}

}
