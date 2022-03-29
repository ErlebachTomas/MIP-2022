#include "mbed.h"
#include "stm32746g_discovery_lcd.h"

// 480x272 velikost displaye

Semaphore one_slot(1);
Thread t2;
Thread t3;
Thread t4;
InterruptIn button(BUTTON1);

void displayInit()
{
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
}

void triggerButton() {
    //BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)ThisThread::get_id(), CENTER_MODE);
}

void test_thread(void const *name)
{
    while (true) {
        one_slot.acquire();
        BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)name, CENTER_MODE);
        one_slot.release();

        //ThisThread::sleep_for(1000);
        //ThisThread::sleep_for(1000);
    }
}

int main()
{
    displayInit();
    BSP_LCD_DisplayStringAt(0, LINE(8), (uint8_t *)"MIP", CENTER_MODE);
    button.rise(&triggerButton);
    
    
    t2.start(callback(test_thread, (void *)"Th 2"));
    t3.start(callback(test_thread, (void *)"Th 3"));
    t4.start(callback(test_thread, (void *)"Th 4"));
    
    test_thread((void *)"Th 1");
}
