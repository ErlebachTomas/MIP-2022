#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"

using namespace std::chrono;

// 480x272 velikost displaye

#define setX 150
#define setY 120
#define setWidth 180
#define setHeight 50
#define incDecX 400
#define incDecSize 50
#define incY 120
#define decY 180

InterruptIn button(BUTTON1);
Timeout flipper;
TS_StateTypeDef TS_State;
int timeToAdd;

void drawButtons()
{
    // set
    BSP_LCD_DrawRect(setX, setY, setWidth, setHeight);
    BSP_LCD_DisplayStringAt(0, 135, (uint8_t *)" ", CENTER_MODE);
    // inc
    BSP_LCD_DrawRect(incDecX, incY, incDecSize, incDecSize);
    BSP_LCD_DisplayChar(417, 135, '+');
    // dec
    BSP_LCD_DrawRect(incDecX, decY, incDecSize, incDecSize);
    BSP_LCD_DisplayChar(417, 195, '-');

}

void drawBomb()
{
    // draw a bomb
    BSP_LCD_FillCircle(55, 210, 50);
    BSP_LCD_DrawVLine(55, 130, 30);
    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
    BSP_LCD_FillCircle(55, 120, 10);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
}

void boomAnimation(int count)
{
    for(int i = 0; i < count; i++) {
        BSP_LCD_Clear(LCD_COLOR_RED);
        BSP_LCD_SetFont(&Font24);
        BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)"BOOM!", CENTER_MODE);
        HAL_Delay(500);
        BSP_LCD_ClearStringLine(5);
        BSP_LCD_Clear(LCD_COLOR_DARKGRAY);
        BSP_LCD_SetFont(&Font24);
        BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)"PRASK!", CENTER_MODE);
        HAL_Delay(500);
    }
    BSP_LCD_Clear(LCD_COLOR_WHITE);
}

void display()
{
    boomAnimation(4);
    HAL_Delay(1000);
    drawButtons();
    drawBomb();
    flipper.attach(&display, 120s);
}

void set()
{ 
    uint8_t text2[30];
    HAL_Delay(1000);
    flipper.detach();
    flipper.attach(&set, timeToAdd + duration_cast<seconds>(flipper.remaining_time()).count() + 1);
}

int main()
{
    int isTouch = 0;
    int timeValue;
    timeToAdd = 0;
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
        BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)"TOUCHSCREEN INIT OK", CENTER_MODE);
    }
    HAL_Delay(1000);
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

    // draw buttons
    drawButtons();

    // draw bomba
    drawBomb();

    flipper.attach(&display, 100s);

    button.rise(&set);

    while(1) {
        
        timeValue = duration_cast<seconds>(flipper.remaining_time()).count() + 1;
        BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)"Remaining Time", CENTER_MODE);

        sprintf((char*)text, "  %d  ", timeToAdd);
        
        BSP_LCD_DisplayStringAt(0, 135, (uint8_t *)&text, CENTER_MODE);

        sprintf((char*)text, "  %llu  ", duration_cast<seconds>(flipper.remaining_time()).count() + 1);

        BSP_LCD_DisplayStringAt(0, LINE(2), (uint8_t *)&text, CENTER_MODE);

        // touch detect
        BSP_TS_GetState(&TS_State);
        if (TS_State.touchDetected) {
            for(idx = 0; idx < TS_State.touchDetected; idx++) {
                x = TS_State.touchX[idx];
                y = TS_State.touchY[idx];
                if(x >= incDecX && x <= incDecX + incDecSize && y >= incY && y <= incY + incDecSize && isTouch == 0) {
                    timeToAdd += 10;
                } else if (x >= incDecX && x <= incDecX + incDecSize && y >= decY && y <= decY + incDecSize && isTouch == 0) {
                    timeToAdd -= 10;
                }
                isTouch = 1;
            }
        } else {
            isTouch = 0;    
        }
    }
}
