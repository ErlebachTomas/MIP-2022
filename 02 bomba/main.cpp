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

void drawButtons()
{
    // set
    BSP_LCD_DrawRect(setX, setY, setWidth, setHeight);
    BSP_LCD_DisplayStringAt(0, 135, (uint8_t *)"Set", CENTER_MODE);
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

void redrawWick(int remaining_time)
{
    // TODO
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


