#ifndef TRON_H
#define TRON_H

#include <BlynkSimpleCurieBLE.h>
#include <CurieBLE.h>
#include <SoftwareSerial.h>
#include <Adafruit_Soundboard.h>
#include <Adafruit_NeoPixel.h>

#define WS2812_CHIPSET      NEO_GRB + NEO_KHZ800

#define DEFAULT_VALUE     0

#define DEFAULT_BRIGHT    127
#define DEFAULT_FPS       60

#define ON  1
#define OFF 0

typedef enum
{
  Power     ,
  Init      ,
  Animation ,
  mColor    ,
  Brightness,
  Speed     ,
  zRed      ,
  zGrn      ,
  zBlu      ,
  Red       ,
  Grn       ,
  Blu       ,
  STATUS_COUNT,
     
} _Status_Vals;

typedef enum
{
  SOLID   = 0,
  BREATH  = 1,
  RAINBOW = 2,
  THEATER_RAINBOW = 3,
  
} _Animation;

typedef enum
{
  CUSTOM_PXL = 0,
  BLACK_PXL     ,
  RED_PXL       ,
  ORANGE_PXL    ,
  YELLOW_PXL    ,
  GREEN_PXL     ,
  CYAN_PXL      ,
  BLUE_PXL      ,
  MAGENTA_PXL   ,
  PURPLE_PXL    ,
  WHITE_PXL     , 
  
} _Colors;

class Tron
{
  
public:
  Tron();
  ~Tron();

  void init();
  void run();
  
  void init_blynk();

private:
  Adafruit_NeoPixel pixels;
  BLEPeripheral blePeripheral;
  SoftwareSerial Audio_Serial;
  Adafruit_Soundboard SFX;

public:
  int Status[STATUS_COUNT];
  uint32_t Color_List[] = 
  {
    pixels.Color(0  , 0  , 0  ),
    pixels.Color(0  , 0  , 0  ),
    pixels.Color(255, 0  , 0  ),
    pixels.Color(255, 100, 0  ),
    pixels.Color(255, 255, 0  ),
    pixels.Color(0  , 255, 0  ),
    pixels.Color(0  , 255, 255),
    pixels.Color(0  , 0  , 255),
    pixels.Color(255, 0  , 255),
    pixels.Color(127, 0  , 255),
    pixels.Color(255, 255, 255),
  };

public:
  // Animations
  uint32_t wheel(int);
  void Solid(uint32_t);
  void Breath(uint32_t);
  void Streak(uint32_t);
  void StreakGPS(uint32_t);
  void ColorWipe(uint32_t);
  void BreathRGB();
  
  void Rainbow();
  void RainbowBreath();
  void TheaterChaseRainbow();
  
};

#endif
