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
uint8_t status;
int timeValue;
uint8_t text[30];

enum class State {RESET, COUNT, SET, BOOM};
State state = State::RESET;

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


void displayBoomBoom()
{
    boomAnimation(4);
    HAL_Delay(1000);
    state = State::RESET;
}

void trigger() {
    state = State::BOOM;
}

void triggerButton() {
    if(timeValue + timeToAdd <= 0) {
       timeToAdd = -timeValue + 2;  
    }
    state = State::SET;   
}

void displayInit()
{
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
    
    BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)"Remaining Time", CENTER_MODE);
    
    flipper.attach(&trigger, 120s);
    
    button.rise(&triggerButton);

}

void reset() {
    HAL_Delay(1000);
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

    timeToAdd = 0;
    // draw buttons
    drawButtons();

    // draw bomba
    drawBomb();
    
    BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)"Remaining Time", CENTER_MODE);
    
    sprintf((char*)text, "  %d  ", timeToAdd);
    BSP_LCD_DisplayStringAt(0, 135, (uint8_t *)&text, CENTER_MODE);
    
    flipper.attach(&trigger, 120s);
    
    button.rise(&triggerButton);
    
    state = State::COUNT;    
}

void set()
{
    HAL_Delay(1000);
    
    flipper.attach(&trigger, timeToAdd + duration_cast<seconds>(flipper.remaining_time()).count() + 1);
    
    timeToAdd = 0;
    sprintf((char*)text, "  %d  ", timeToAdd);
    BSP_LCD_DisplayStringAt(0, 135, (uint8_t *)&text, CENTER_MODE);
    
    state = State::COUNT;
}

void count()
{
    timeValue = duration_cast<seconds>(flipper.remaining_time()).count() + 1;

    sprintf((char*)text, "  %llu  ", duration_cast<seconds>(flipper.remaining_time()).count() + 1);
    BSP_LCD_DisplayStringAt(0, LINE(2), (uint8_t *)&text, CENTER_MODE);

    sprintf((char*)text, "  %d  ", timeToAdd);
    BSP_LCD_DisplayStringAt(0, 135, (uint8_t *)&text, CENTER_MODE);
}

int main()
{
    int isTouch = 0;
    timeToAdd = 0;
    uint16_t x, y;
    uint8_t idx;

    displayInit();

    while(1) {
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
        
        //switch states
        switch(state) {
            case State::RESET:
                reset();
                break;
            case State::COUNT:
                count();
                break;
            case State::SET:
                set();
                break;
            case State::BOOM:
                displayBoomBoom();
                break; 
        }    
    }
}
