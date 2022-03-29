#include "mbed.h"
#include "stm32746g_discovery_lcd.h"

// 480x272 velikost displaye

Semaphore one_slot(1);
Thread t2;
Thread t3;
Thread t4;
InterruptIn button(BUTTON1);



void drawButtons() {
    BSP_LCD_DrawRect(350, 120, 50, 50);
    BSP_LCD_DrawRect(250, 120, 50, 50);
    BSP_LCD_DrawRect(150, 120, 50, 50);
    BSP_LCD_DrawRect(50, 120, 50, 50);
    
}


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
    one_slot.release();
}

void test_thread(void const *name)
{
    while (true) {
       // one_slot.acquire();
        if (one_slot.try_acquire()) {
            //BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)name, CENTER_MODE);
            
            char val = *(char*)name;
            switch(val){
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
            }
            
            // one_slot.release();
            //ThisThread::sleep_for(500ms);
             
        }

        //ThisThread::sleep_for(1000);

    }
}

int main()
{
    displayInit();
    drawButtons();
    BSP_LCD_DisplayStringAt(0, LINE(8), (uint8_t *)"MIP", CENTER_MODE);
    button.rise(&triggerButton);
    
    
    t2.start(callback(test_thread, (void *)"2"));
    t3.start(callback(test_thread, (void *)"3"));
    t4.start(callback(test_thread, (void *)"4"));
    
    test_thread((void *)"1");
}
