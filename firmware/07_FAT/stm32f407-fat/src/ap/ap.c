#include "ap.h"

#include "enet/enet.h"


void updateLED(void);
void updateLCD(void);
void updateSD(void);


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
    updateLED();
    updateLCD();
    updateSD();

    delay(10);
  }
}

void updateSD(void)
{
  sd_state_t sd_state;


  sd_state = sdUpdate();
  if (sd_state == SDCARD_CONNECTED)
  {
    logPrintf("\n[  ] SDCARD_CONNECTED\n");
  }
  if (sd_state == SDCARD_DISCONNECTED)
  {
    logPrintf("\n[  ] SDCARD_DISCONNECTED\n");
  }
}

void updateLED(void)
{
  static uint32_t pre_time = 0;
  
  
  if (millis() - pre_time >= 500)
  {
    pre_time = millis();
    ledToggle(_DEF_LED1);
  }
}

void updateLCD(void)
{
  int16_t         x_offset = 10;
  static uint8_t  menu     = 0;
  uint8_t         menu_max = 5;
  uint8_t         menu_cur = 0;


  if (!lcdIsInit())
  {
    return;
  }

  menu_cur = menu;

  if (lcdDrawAvailable())
  {
    lcdClearBuffer(black);

    lcdDrawRect(0, 0, 4, 32, white);
    for (int i=0; i<menu_max; i++)
    {
      if (i == menu)
        lcdDrawFillRect(0, i*(32/menu_max), 4, (32/menu_max), white);
    }


    if (menu_cur == 0)
    {
      if (enetIsLink() == true)
      {
        if (enetIsGetIP() == true)
        {
          enet_info_t net_info;

          enetGetInfo(&net_info);

          lcdPrintf(x_offset,  0, white,
                    "IP %d.%d.%d.%d", 
                    net_info.ip[0], 
                    net_info.ip[1],
                    net_info.ip[2],
                    net_info.ip[3]);
          lcdPrintf(x_offset, 16, white,
                    "DHCP : %s\n", enetIsGetIP() ? "True":"False");          
        }
        else
        {
          lcdPrintf(x_offset, 8, white, "Getting_IP..");        
        }      
      }
      else
      {
        lcdPrintf(x_offset, 8, white, "Not Connected");        
      }
    }

    lcdRequestDraw();
  }
}


