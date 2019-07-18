#include "tron.h"

void Tron::Tron()
{
  
}

void Tron::~Tron()
{
  
}

void Tron::init(int LED_COUNT, int LED_PIN,  int SFX_TX, int SFX_RX, int SFX_RST)
{
  Audio_Serial = SoftwareSerial(SFX_TX, SFX_RX);
  SFX = Adafruit_Soundboard(&Audio_Serial, NULL, SFX_RST);

  Audio_Serial.begin(9600);

  // Set up BLE peripheral for Blynk
  blePeripheral.setLocalName("TRON_101");
  blePeripheral.setDeviceName("TRON_101");
  blePeripheral.setAppearance(384);
  
  Blynk.begin(blePeripheral, auth);
  blePeripheral.begin();
  
  // Start Neopixel strip
  pixels = Adafruit_NeoPixel(LED_COUNT, LED_PIN, WS2812_CHIPSET);
  pixels.begin();
  pixels.show();
}

void Tron::run()
{  
  // Poll BLE and run Blynk service
  blePeripheral.poll();
  Blynk.run();
  
  if (POWR == ON)
  {
    switch(ANIM)
    {
      case SOLID:
        Solid(COLORS[COLO]);
        break;
      case BREATH:
        Breath(COLORS[COLO]);
        break;
      case RAINBOW:
        Rainbow();
        break;  
      case THEATER_RAINBOW:
        TheaterChaseRainbow();
        break;
    }
  }
  else
    Solid(COLORS[BLACK_PXL]);
}

void Tron::init_blynk()
{
  for(int i = 0; i < STATUS_COUNT; i++)
  {
    if (i == Brightness) 
      Status[i] = DEFAULT_BRIGHT;
    else if (i == Speed)
      Status[i] = DEFAULT_FPS;
    else
      Status[i] = DEFAULT_VAL;
  }
  
  Blynk.virtualWrite(V0, Status[Power]);
  Blynk.virtualWrite(V1, Status[Red], Status[Grn], Status[Blu]);
  Blynk.virtualWrite(V2, Status[Brightness]);
  Blynk.virtualWrite(V3, Status[Speed]);
  Blynk.virtualWrite(V5, Status[Init]);
  Blynk.virtualWrite(V6, Status[Animation]);
  Blynk.virtualWrite(V7, Status[mColor]);

}

void Tron::fps()
{
  delay(1000 / Status[Speed]);
}

void Tron::fps_alt()
{
  delay(map(Status[Speed]/10, 1, 10, 10, 1));
}

void Tron::Solid(uint32_t c)
{
  for(uint16_t i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, c);
  }
  pixels.show();
}

void Tron::Breath(uint32_t c)
{
  for(int j = 0; j < 256; j++)
  {
    for(uint16_t i = 0; i < pixels.numPixels(); i++) 
      pixels.setPixelColor(i, c);
    
    pixels.show();
    pixels.setBrightness(j);
    fps_alt();
  }
  for(int j = 255; j > 0; j--)
  {
    for(uint16_t i = 0; i < pixels.numPixels(); i++) 
      pixels.setPixelColor(i, c);
    
    pixels.show();
    pixels.setBrightness(j);
    fps_alt();
  }
  
}
uint32_t Tron::wheel(int pos)
{
  if (pos < 85) {  
    return pixels.Color(pos * 3, 255 - pos * 3, 0);
  }
  else if ( pos < 170) {
    pos -= 85;
    return pixels.Color(255 - pos * 3, 0, pos * 3);
  }
  else {
    pos -=170;
    return pixels.Color(0, pos * 3, 255 - pos * 3);
  } 
}

void Tron::Rainbow()
{
  for (int i = 0; i < 256; i++)
  {
    for (int j = 0; j < LED_COUNT; j++)
    {
      pixels.setPixelColor(j, wheel((i+j) & 255));
    }
    pixels.show();
    fps_alt();
  }
}

void Tron::TheaterChaseRainbow()
{
  for (int i = 0; i < 256; i++)
  {
    for (int j = 0; j < 3; j++)
    {
       for (int k = 0; k < pixels.numPixels(); k+=3)
       {
           pixels.setPixelColor(j+k, wheel((i+k) % 255));
       }
       pixels.show();
       fps();
       for (int k = 0; k < pixels.numPixels(); k+=3)
       {
           pixels.setPixelColor(j+k, 0);
       }  
    }
  }
}

void Tron::Streak(uint32_t c)
{
  for (int i = 0; i < pixels.numPixels(); i++)
  {
    
  }
}

void Tron::ColorWipe(uint32_t c)
{
  
}

void Tron::flushInput() {
  // Read all available serial input to flush pending data.
  uint16_t timeoutloop = 0;
  while (timeoutloop++ < 40) 
  {
    while(Audio_Serial.available()) 
    {
      Audio_Serial.read();
      timeoutloop = 0;  // If char was received reset the timer
    }
    delay(1);
  }
}