#include "ap.h"




void apInit(void)
{    
  moduleInit(); 

  for (int i = 0; i < 32; i += 1)
  {
    lcdClearBuffer(black);
    lcdPrintfResize(0, 40 - i, green, 16, "  -- BARAM --");
    lcdDrawRect(0, 0, LCD_WIDTH, LCD_HEIGHT, white);
    lcdUpdateDraw();
    delay(10);
  }
  delay(500);
  lcdClear(black);    
}

void apMain(void)
{
  
  while(1)
  {
    ledToggle(_DEF_CH1);
    delay(500);
  }
}

