#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"

// 480x272 velikost displaye

Semaphore one_slot(1);
Thread t2;
Thread t3;
Thread t4;
Thread t5;
Thread t6;

int isTouch = 0;

TS_StateTypeDef TS_State;
uint8_t status;
uint8_t text[30];
uint8_t idx;
char previousThread = '0';

void displayInit()
{
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
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
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
}

void test_thread(void const *name)
{
    while(true) {
        one_slot.acquire();
        BSP_TS_GetState(&TS_State);
        if (TS_State.touchDetected) {
            if (isTouch == 0) {
                //BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)name, CENTER_MODE);
                char val = *(char*)name;
                if (previousThread != val) {
                    previousThread = val;
                    switch(val) {
                        case '1':
                            BSP_LCD_Clear(LCD_COLOR_GREEN);
                            BSP_LCD_SetFont(&Font24);
                            BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
                            BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
                            BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)"Thread 1", CENTER_MODE);
                            break;
                        case '2':
                            BSP_LCD_Clear(LCD_COLOR_BLUE);
                            BSP_LCD_SetFont(&Font24);
                            BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
                            BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
                            BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)"Thread 2", CENTER_MODE);
                            break;
                        case '3':
                            BSP_LCD_Clear(LCD_COLOR_YELLOW);
                            BSP_LCD_SetFont(&Font24);
                            BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
                            BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
                            BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)"Thread 3", CENTER_MODE);
                            break;
                        case '4':
                            BSP_LCD_Clear(LCD_COLOR_RED);
                            BSP_LCD_SetFont(&Font24);
                            BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
                            BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
                            BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)"Thread 4", CENTER_MODE);
                            break;
                        case '5':
                            BSP_LCD_Clear(LCD_COLOR_BLACK);
                            BSP_LCD_SetFont(&Font24);
                            BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
                            BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
                            BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)"Thread 5", CENTER_MODE);
                            break;
                        case '6':
                            BSP_LCD_Clear(LCD_COLOR_WHITE);
                            BSP_LCD_SetFont(&Font24);
                            BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
                            BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
                            BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)"Thread 6", CENTER_MODE);
                            break;
                    }
                }
            }
            isTouch = 1;
        } else {
            isTouch = 0;
        }
        one_slot.release();
    }
}

int main()
{
    // display init
    displayInit();
    HAL_Delay(1000);
    // threads start
    t2.start(callback(test_thread, (void *)"2"));
    t3.start(callback(test_thread, (void *)"3"));
    t4.start(callback(test_thread, (void *)"4"));
    t5.start(callback(test_thread, (void *)"5"));
    t6.start(callback(test_thread, (void *)"6"));
    test_thread((void *)"1");
}
