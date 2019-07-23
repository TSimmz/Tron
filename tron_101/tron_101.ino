//======================================================
//
//
//
//
//
//
//
//======================================================

#include "tron.h"

//======================================================
// Adafruit Audio SFX board set up

#define SFX_RX 4
#define SFX_TX 5
#define SFX_RST 6
//======================================================

//======================================================
// Blynk/BLE set up

//char auth[] = "RhyEuIrXcuzWXikp8Pq2lJlQox1JTkoZ";

//======================================================

//======================================================
// Adafruit Neopixel set up

#define LED_PIN             7
#define LED_COUNT           50

//======================================================

Tron MyTron = Tron();

//======================================================
// Blynk virtual pin protocols

// Power button
BLYNK_WRITE(V0)
{
  MyTron.Status[Power] = param.asInt();
}

// zeRGBa Color picker
BLYNK_WRITE(V1)
{
  MyTron.Status[zRed] = param[0].asInt();
  MyTron.Status[zGrn] = param[1].asInt();
  MyTron.Status[zBlu] = param[2].asInt();
  MyTron.Color_List[CUSTOM_PXL] = MyTron.pixels.Color(MyTron.Status[zRed], MyTron.Status[zGrn], MyTron.Status[zBlu]);
}

// Brightness slider
BLYNK_WRITE(V2)
{
  MyTron.pixels.setBrightness(param.asInt());
}

// Speed slider
BLYNK_WRITE(V3)
{
  MyTron.Status[Speed] = param.asInt();
}

// Initialize button
BLYNK_WRITE(V5)
{
  MyTron.Status[Init] = param.asInt();
  if (MyTron.Status[Init])
  {
    MyTron.init_blynk();
    Blynk.virtualWrite(V5, 0);
  }
}

// Animation drop-down menu
BLYNK_WRITE(V6)
{
  MyTron.Status[Animation] = param.asInt() - 1;
}

// Color drop-down menu
BLYNK_WRITE(V7)
{
  MyTron.Status[ColorType] = param.asInt() - 1;
}

//================= Sound FX buttons =================
BLYNK_WRITE(V8)
{
  MyTron.SFX.playTrack((uint8_t)0);
}

BLYNK_WRITE(V9)
{
  MyTron.SFX.playTrack((uint8_t)1);
}

BLYNK_WRITE(V10)
{
  MyTron.SFX.playTrack((uint8_t)2);
}

BLYNK_WRITE(V11)
{
  MyTron.SFX.playTrack((uint8_t)3);
}

BLYNK_WRITE(V12)
{
  MyTron.SFX.playTrack((uint8_t)4);
}

BLYNK_WRITE(V13)
{
  //MyTron.Speed = GPS.getSpeed();
}

//======================================================


void setup()
{
    Serial.begin(9600);           // Serial monitor
    
    MyTron.init((int)LED_COUNT, (int)LED_PIN, (int)SFX_TX, (int)SFX_RX, (int)SFX_RST);    
}

void loop()
{
  // Run MyTron
  MyTron.run();
}
