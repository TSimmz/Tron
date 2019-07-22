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

char auth[] = "RhyEuIrXcuzWXikp8Pq2lJlQox1JTkoZ";

//======================================================

//======================================================
// Adafruit Neopixel set up

#define LED_PIN             7
#define LED_COUNT           50

//======================================================

//======================================================
// Blynk virtual pin protocols

// Power button
BLYNK_WRITE(V0)
{
  Tron.Status[Power] = param.asInt();
}

// zeRGBa Color picker
BLYNK_WRITE(V1)
{
  Tron.Status[zRed] = param[0].asInt();
  Tron.Status[zGrn] = param[1].asInt();
  Tron.Status[zBlu] = param[2].asInt();
  Tron.Color_List[CUSTOM_PXL] = pixels.Color(Tron.Status[zRed], Tron.Status[zGrn], Tron.Status[zBlu]);
}

// Brightness slider
BLYNK_WRITE(V2)
{
  Tron.pixels.setBrightness(param.asInt());
}

// Speed slider
BLYNK_WRITE(V3)
{
  Tron.Status[Speed] = param.asInt();
}

// Initialize button
BLYNK_WRITE(V5)
{
  Tron.Status[Init] = param.asInt();
  if (Tron.Status[Init])
  {
    Tron.init_blynk();
    Blynk.virtualWrite(V5, 0);
  }
}

// Animation drop-down menu
BLYNK_WRITE(V6)
{
  Tron.Status[Animation] = param.asInt() - 1;
}

// Color drop-down menu
BLYNK_WRITE(V7)
{
  Tron.Status[mColor] = param.asInt() - 1;
}

//================= Sound FX buttons =================
BLYNK_WRITE(V8)
{
  SFX.playTrack((uint8_t)0);
}

BLYNK_WRITE(V9)
{
  Tron.SFX.playTrack((uint8_t)1);
}

BLYNK_WRITE(V10)
{
  Tron.SFX.playTrack((uint8_t)2);
}

BLYNK_WRITE(V11)
{
  Tron.SFX.playTrack((uint8_t)3);
}

BLYNK_WRITE(V12)
{
  Tron.SFX.playTrack((uint8_t)4);
}

BLYNK_WRITE(V13)
{
  Tron.Speed = GPS.getSpeed();
}

//======================================================


void setup()
{
    Serial.begin(9600);           // Serial monitor
    
    Tron Tron = new Tron();
    Tron101.init();    
}

void loop()
{
  // Run tron
  Tron.run();
}
