#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "stlogo.h"

using namespace std::chrono;


// 480x272 velikost displaye

Timeout flipper;

void display() {
    // idk
    //BSP_LCD_SelectLayer(0);

    BSP_LCD_SetBackColor(LCD_COLOR_RED);
    
    BSP_LCD_DisplayStringAt(0, LINE(3), (uint8_t *)"BOOOOOOOOOOOOOM!!!", CENTER_MODE);
    HAL_Delay(1000);
   // BSP_LCD_Clear(LCD_COLOR_WHITE);
    flipper.attach(&display, 5s);
}


int main()
{
    TS_StateTypeDef TS_State;
    uint16_t x, y;
    uint8_t text[30];
    uint8_t status;
    uint8_t idx;
    uint8_t cleared = 0;
    uint8_t prev_nb_touches = 0;

    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);

    BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)"BOMBA DEMO", CENTER_MODE);
    HAL_Delay(1000);

    status = BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
    if (status != TS_OK) {
        BSP_LCD_Clear(LCD_COLOR_RED);
        BSP_LCD_SetBackColor(LCD_COLOR_RED);
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)"TOUCHSCREEN INIT FAIL", CENTER_MODE);
    } else {
        BSP_LCD_Clear(LCD_COLOR_GREEN);
        BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)"TOUCHSCREEN INIT OK", CENTER_MODE);
    }

    HAL_Delay(1000);
    
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    
    BSP_LCD_SetFont(&Font24);  
    // BSP_LCD_SetBackColor(LCD_COLOR_RED);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

    // set     
    BSP_LCD_DrawRect(150, 120, 170, 50);
    // set text
    BSP_LCD_DisplayStringAt(0, 135, (uint8_t *)"Set", CENTER_MODE);
    
    // dec
    BSP_LCD_DrawRect(150, 220, 50, 50);
    // dec text
    BSP_LCD_DisplayStringAt(-65, 235, (uint8_t *)"-", CENTER_MODE);
    
    // inc
    BSP_LCD_DrawRect(210, 220, 50, 50);
    // inc text
    BSP_LCD_DisplayStringAt(-5, 235, (uint8_t *)"+", CENTER_MODE);
    
    // submit
    BSP_LCD_DrawRect(270, 220, 50, 50);
    // submit text
    BSP_LCD_DisplayStringAt(55, 235, (uint8_t *)"S", CENTER_MODE);
    
    
    flipper.attach(&display, 5s);
    
   // sprintf((char*)text, "Scheduled Time: %llu ms", flipper.scheduled_time());
   // BSP_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)&text, CENTER_MODE);
    
    while(1) {
       // print zbyvajiciho casu
        sprintf((char*)text, "%llu seconds remaining!!!", duration_cast<seconds>(flipper.remaining_time()).count() + 1);
        BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)&text, CENTER_MODE);

        BSP_TS_GetState(&TS_State);
        if (TS_State.touchDetected) {
            for(idx = 0; idx < TS_State.touchDetected; idx++) {
                x = TS_State.touchX[idx];
                y = TS_State.touchY[idx];
                
                if(x >= 150 && x <= 320 && y >= 120 && y <= 170) {
                    BSP_LCD_DisplayStringAt(0, LINE(4), (uint8_t *)"Bylo Selectnuto", CENTER_MODE);
                } else {
                    BSP_LCD_DisplayStringAt(0, LINE(4), (uint8_t *)"Neni Selectnuto", CENTER_MODE);
                }
            }         
        }
    }
}
