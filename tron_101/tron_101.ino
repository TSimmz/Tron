//======================================================
//
//
//
//
//
//
//
//======================================================
#include <BlynkSimpleCurieBLE.h>
#include <CurieBLE.h>
#include "tron.h"

//======================================================
// Adafruit Audio SFX board set up

//#define SFX_RX 4
//#define SFX_TX 5
//#define SFX_RST 6
//======================================================

//======================================================
// Blynk/BLE set up

char auth[] = "RhyEuIrXcuzWXikp8Pq2lJlQox1JTkoZ";

//======================================================

//======================================================
// Adafruit Neopixel set up
//
//#define LED_PIN             7
//#define LED_COUNT           50

//======================================================
BLEPeripheral blePeripheral;
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
    init_blynk();
    MyTron.init_LoopVars();
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
  //MyTron.Status[SpeedGPS]= GPS.getSpeed();
}

//======================================================


void setup()
{
    Serial.begin(9600);           // Serial monitor

    // Set up BLE peripheral for Blynk
    blePeripheral.setLocalName("TRON_101");
    blePeripheral.setDeviceName("TRON_101");
    blePeripheral.setAppearance(384);
    
    Blynk.begin(blePeripheral, auth);
    blePeripheral.begin();
    
    MyTron.init();    
}

void loop()
{
  // Poll BLE and run Blynk service
  blePeripheral.poll();
  Blynk.run();
  
  // Run MyTron
  MyTron.run();
}


void init_blynk()
{
  Serial.println("Initializing Blynk..");
  for(int i = 0; i < STATUS_COUNT; i++)
  {
    if (i == Brightness) 
      MyTron.Status[i] = DEFAULT_BRIGHT;
    else if (i == Speed)
      MyTron.Status[i] = DEFAULT_FPS;
    else
      MyTron.Status[i] = DEFAULT_VALUE;
  }
  
  Blynk.virtualWrite(V0, MyTron.Status[Power]);
  Blynk.virtualWrite(V1, MyTron.Status[zRed], MyTron.Status[zGrn], MyTron.Status[zBlu]);
  Blynk.virtualWrite(V2, MyTron.Status[Brightness]);
  Blynk.virtualWrite(V3, MyTron.Status[Speed]);
  Blynk.virtualWrite(V5, MyTron.Status[Init]);
  Blynk.virtualWrite(V6, MyTron.Status[Animation]);
  Blynk.virtualWrite(V7, MyTron.Status[ColorType]);

  Serial.println("Complete!");

}
