#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "string"
#include "ctime"


// 480x272 velikost displaye

Semaphore one_slot(1);
Thread t2;

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
                if (val == '2') {
                    BSP_LCD_Clear(LCD_COLOR_WHITE);
                    BSP_LCD_SetFont(&Font24);
                    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
                    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
                    BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)"Posilam data...", CENTER_MODE);
                    time_t t = time(0);
                    printf("%s %s %s %s %s %d\n", "api_key=", "\"7BGKHY6ER3G9WR3M\"", "&field1=", "\"CISLO\"", "&field2=", t);
                    HAL_Delay(500);
                    BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)"Data byla poslana...", CENTER_MODE);
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
    test_thread((void *)"1");
}