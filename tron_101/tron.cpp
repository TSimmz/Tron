#include "tron.h"

Tron::Tron()
{

}

Tron::~Tron()
{
  
}

void Tron::init()
{
  Audio_Serial.begin(9600);
  
  mCurrentAnimation = 0;
  mPreviousAnimation = -1;
  
  mBreathStatus = 0;
  mWipeStatus = 0;
  mChaseStatus = 0;

  // Start Neopixel strip
  pixels.begin();
  pixels.show();
}

void Tron::run()
{  
  if (Status[Power] == ON)
  {
	  mCurrentAnimation = Status[Animation];
	  if (mPreviousAnimation != mCurrentAnimation)
  	{
  		init_LoopVars();
  		mPreviousAnimation = mCurrentAnimation;
  	}
	
    switch(Status[Animation])
    {
    case SOLID:
      Solid(getColor(Status[ColorType]));
      break;
    case BREATH:
      Breath(getColor(Status[ColorType]));
      break;
	  case STREAK:
	    Streak(getColor(Status[ColorType]), 10, 64, true);
		  break;
	  case STREAKGPS:
	    StreakGPS(getColor(Status[ColorType]));
	    break;
	  case COLORWIPE:
	    ColorWipe(getColor(Status[ColorType]));
	    break;
	  case THEATERCHASE:
	    TheaterChase(getColor(Status[ColorType]));
		  break;
    case RAINBOW:
      Rainbow();
      break;
    case RAINBOWSTRIP:
	    RainbowStrip();
		  break;
	  case RAINBOWBREATH:
	    RainbowBreath();
	    break;
	  case RAINBOWCOLORWIPE:
	    RainbowColorWipe();
	    break;
    case THEATER_RAINBOW:
      TheaterChaseRainbow();
      break;
    }
  }
  else
    Solid(Color_List[BLACK_PXL]);

  fps();
}


void Tron::init_LoopVars()
{
  int func = 0;

  SetStrip(Color_List[BLACK_PXL]);
  pixels.show();
  
  if (func == 0)
  {
    LoopVar[iGlobal] = 0;
    LoopVar[jGlobal] = 0;
    LoopVar[kGlobal] = 0;

    mBreathStatus = 0;
    mWipeStatus = 0;
    mChaseStatus = 0;
  }
  else
  {
	  LoopVar[iGlobal] = 0;
    LoopVar[jGlobal] = 0;
    LoopVar[kGlobal] = 0;
  }

}

void Tron::fps()
{
  delay(1000 / Status[Speed]);
}

void Tron::fps_alt()
{
  delay(map(Status[Speed]/10, 1, 10, 10, 1));
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

uint32_t Tron::getColor(int index)
{
  switch(index)
  {
    case CUSTOM_PXL : return Color_List[CUSTOM_PXL];
    case BLACK_PXL  : return Color_List[BLACK_PXL];
    case RED_PXL    : return Color_List[RED_PXL];
    case ORANGE_PXL : return Color_List[ORANGE_PXL];
    case YELLOW_PXL : return Color_List[YELLOW_PXL];
    case GREEN_PXL  : return Color_List[GREEN_PXL];
    case CYAN_PXL   : return Color_List[CYAN_PXL];
    case BLUE_PXL   : return Color_List[BLUE_PXL];
    case MAGENTA_PXL: return Color_List[MAGENTA_PXL];
    case PURPLE_PXL : return Color_List[PURPLE_PXL];
    case WHITE_PXL  : return Color_List[WHITE_PXL];
    default: return Color_List[BLACK_PXL];
  }
}

void Tron::FadeToBlack(int ledNo, byte fadeValue) 
{
    uint32_t oldColor;
    uint8_t r, g, b;
    
    oldColor = pixels.getPixelColor(ledNo);
    
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r = (r <= 10) ? 0 : (int) r - (r * fadeValue/256);
    g = (g <= 10) ? 0 : (int) g - (g * fadeValue/256);
    b = (b <= 10) ? 0 : (int) b - (b * fadeValue/256);
    
    pixels.setPixelColor(ledNo, pixels.Color(r, g, b));
}

void Tron::SetStrip(uint32_t c)
{
	for(uint16_t i = 0; i < pixels.numPixels(); i++) 
		pixels.setPixelColor(i, c);
}

void Tron::SetStripSection(uint32_t c, int s, int e)
{
  for(uint16_t i = s; i < e; i++) 
    pixels.setPixelColor(i, c);
}

/// Sets the NeoPixel strip one color
void Tron::Solid(uint32_t c)
{
  SetStrip(c);
  
  pixels.show();
}

/// Breath animation with one color. 
void Tron::Breath(uint32_t c)
{
  if (mBreathStatus == 0)
  {
  	if(LoopVar[iGlobal] < 256)
  	{
  		SetStrip(c);
      
  		pixels.show();
  		pixels.setBrightness(LoopVar[iGlobal]);
  	
  		LoopVar[iGlobal]++;
      }
  	else
  	{
  		mBreathStatus = 1;
  		LoopVar[iGlobal]--;
  	}
  }
  else if (mBreathStatus == 1)
  {
  	if(LoopVar[iGlobal] >= 0)
  	{
  		SetStrip(c);
      
  		pixels.show();
  		pixels.setBrightness(LoopVar[iGlobal]);

      if (LoopVar[iGlobal] == 0)
        mBreathStatus = 0;
      else
  		  LoopVar[iGlobal]--;
  	}
  }  
}

void Tron::Streak(uint32_t c, uint8_t mSize, uint8_t mTrailDecay, bool mRandDecay)
{
  SetStrip(Color_List[BLACK_PXL]);
  
  for(int i = 0; i < pixels.numPixels() * 2; i++) {
    
    // fade brightness all LEDs one step
    for(int j = 0; j < pixels.numPixels(); j++)
    {
      if( (!mRandDecay) || (random(10) > 5) ) 
      {
        FadeToBlack(j, mTrailDecay );        
      }
    }
    
    // draw meteor
    for(int j = 0; j < mSize; j++) 
    {
      if((i - j < pixels.numPixels()) && (i - j >= 0) ) 
      {
        pixels.setPixelColor(i-j, c);
      } 
    }
   
    pixels.show();
    delay(10);
  }
}

void Tron::StreakGPS(uint32_t c)
{
  for (int i = 0; i < pixels.numPixels(); i++)
  {
    
  }
}

void Tron::ColorWipe(uint32_t c)
{
  int index = LoopVar[iGlobal]++;
  
  if (mWipeStatus == 0)
  {
    SetStripSection(c, 0, index);
    pixels.show();

    if (index == pixels.numPixels())
    {
      mWipeStatus = 1;
      LoopVar[iGlobal] = 0;
    }
  }

  else if (mWipeStatus == 1)
  {
    SetStripSection(Color_List[BLACK_PXL], 0, index);
    pixels.show();

    if (index == pixels.numPixels())
    {
      mWipeStatus = 0;
      LoopVar[iGlobal] = 0;
    }
  }
}

void Tron::TheaterChase(uint32_t c)
{
  if (LoopVar[iGlobal] < 3)
  {
    if (mChaseStatus == 0)
    {
      mChaseStatus = 1;
      
      for (int j = 0; j < pixels.numPixels(); j+=3)
      {
        pixels.setPixelColor(LoopVar[iGlobal] + j, c);
      }
      pixels.show();
    }
    else
    {
      mChaseStatus = 0;
      for (int j = 0; j < pixels.numPixels(); j+=3)
      {
        pixels.setPixelColor(LoopVar[iGlobal] + j, Color_List[BLACK_PXL]);
      } 
      
      LoopVar[iGlobal]++;
    }
  }
  else
  {
    LoopVar[iGlobal] = 0;
  }
}

void Tron::Rainbow()
{
  if (LoopVar[iGlobal] < 256)
  {
    for (int j = 0; j < pixels.numPixels(); j++)
      pixels.setPixelColor(j, wheel((LoopVar[iGlobal]+j) & 255));
    
    pixels.show();

    LoopVar[iGlobal]++;
  }
  else
  {
    LoopVar[iGlobal] = 0;
  }
}

void Tron::RainbowBreath()
{
  for (int c = RED_PXL; c <= PURPLE_PXL; c++)
  {
    for(int j = 0; j < 256; j++)
    {
      for(uint16_t i = 0; i < pixels.numPixels(); i++) 
        pixels.setPixelColor(i, Color_List[c]);
      
      pixels.show();
      pixels.setBrightness(j);
      fps_alt();
    }

    for(int j = 255; j > 0; j--)
    {
      for(uint16_t i = 0; i < pixels.numPixels(); i++) 
        pixels.setPixelColor(i, Color_List[c]);
      
      pixels.show();
      pixels.setBrightness(j);
      fps_alt();
    } 
  }

}

void Tron::RainbowStrip()
{
  for (int i = 0; i < 256; i++)
  {
    for (int j = 0; j < pixels.numPixels(); j++)
    {
      for (int k = 0; k < pixels.numPixels(); k++)
        {
          pixels.setPixelColor(k, wheel((i+j) & 255));
        }
    }
    pixels.show();
    fps_alt();
  }

}

void Tron::RainbowColorWipe()
{
  for (int i = RED_PXL; i <= PURPLE_PXL; i++)
  {
    for (int j = 0; j < pixels.numPixels(); j++)
    {
      pixels.setPixelColor(j, i);
      pixels.show();
      fps_alt();
    }
  }
}

void Tron::TheaterChaseRainbow()
{
  if (LoopVar[iGlobal] < 256)
  {
    if (LoopVar[jGlobal] < 3)
    {
      if (mChaseStatus == 0)
      {
        mChaseStatus = 1;
        
        for (int k = 0; k < pixels.numPixels(); k+=3)
        {
          pixels.setPixelColor(LoopVar[jGlobal] + k, wheel((LoopVar[iGlobal] + k) % 255));
        }
        pixels.show();
      }
      else
      {
        mChaseStatus = 0;
        for (int k = 0; k < pixels.numPixels(); k+=3)
        {
          pixels.setPixelColor(LoopVar[jGlobal] + k, Color_List[BLACK_PXL]);
        } 
        
        LoopVar[jGlobal]++;
      }
    }
    else
    {
      LoopVar[iGlobal]++;
      LoopVar[jGlobal] = 0;
    }
  }
  else
  {
    LoopVar[iGlobal] = 0;
    LoopVar[jGlobal] = 0;
  }
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
