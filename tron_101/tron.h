#ifndef TRON_H
#define TRON_H


#include <SoftwareSerial.h>
#include <Adafruit_Soundboard.h>
#include <Adafruit_NeoPixel.h>

#define WS2812_CHIPSET      NEO_GRB + NEO_KHZ800

#define DEFAULT_VALUE     0

#define DEFAULT_BRIGHT    127
#define DEFAULT_FPS       60

#define SFX_RX 4
#define SFX_TX 5
#define SFX_RST 6

#define LED_PIN             7
#define LED_COUNT           50

#define ON  1
#define OFF 0



typedef enum
{
  Power     ,
  Init      ,
  Animation ,
  ColorType ,
  Brightness,
  Speed     ,
  SpeedGPS  ,
  zRed      ,
  zGrn      ,
  zBlu      ,
  STATUS_COUNT,
     
} _Status_Vals;

typedef enum
{
  SOLID = 0       ,
  BREATH          ,
  STREAK          ,
  STREAKGPS       ,
  COLORWIPE       ,
  THEATERCHASE    ,
  RAINBOW         ,
  RAINBOWSTRIP    ,
  RAINBOWBREATH   ,
  RAINBOWCOLORWIPE,
  THEATER_RAINBOW ,
  ANIMATION_COUNT ,
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
  COLOR_COUNT   ,  
} _Colors;

typedef enum
{
  iGlobal,
  jGlobal,
  kGlobal,
  LOOPVAR_COUNT
} _LoopVars;

class Tron
{
  
public:
  Tron();
  ~Tron();

  void init();
  void run();
  
  void init_LoopVars();
  void flushInput();

  void fps();
  void fps_alt();

  void SetStrip(uint32_t);

public:
  Adafruit_NeoPixel pixels = Adafruit_NeoPixel(LED_COUNT, LED_PIN, WS2812_CHIPSET);
  
  SoftwareSerial Audio_Serial = SoftwareSerial(SFX_TX, SFX_RX);
  Adafruit_Soundboard SFX = Adafruit_Soundboard(&Audio_Serial, NULL, SFX_RST);

public:
  int Status[STATUS_COUNT];
  int LoopVar[LOOPVAR_COUNT];

  uint32_t Color_List[COLOR_COUNT] = 
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

  int mCurrentAnimation;
  int mPreviousAnimation;
  
  int mBreathStatus;
  
public:
  // Animations
  uint32_t wheel(int);
  uint32_t getColor(int);

  void Solid(uint32_t);
  void Breath(uint32_t);
  void Streak(uint32_t);
  void StreakGPS(uint32_t);
  void ColorWipe(uint32_t);
  void TheaterChase(uint32_t);
  
  void Rainbow();
  void RainbowStrip();
  void RainbowBreath();
  void RainbowColorWipe();
  void TheaterChaseRainbow();
  
};

#endif
