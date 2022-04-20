#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "string"


#define MAXIMUM_BUFFER_SIZE  32
static BufferedSerial  serial_port(USBTX, USBRX);


// 480x272 velikost displaye
InterruptIn button(BUTTON1);

Semaphore one_slot(1);
Thread t2;


uint8_t status;
uint8_t text[30];
uint8_t idx;


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
        one_slot.acquire();
        
        char buff[MAXIMUM_BUFFER_SIZE];
        sprintf(buff,"Test!");
              
        serial_port.write( buff , sizeof(buff));
       
        one_slot.release();
    
}

void triggerButton() { 
  
  string msg = "Tlacitko!";
  //serial_port.write( msg , sizeof(msg));
  
}

int main()
{
    
    serial_port.set_baud(9600);
    serial_port.set_format(
        /* bits */ 8,
        /* parity */ BufferedSerial::None,
        /* stop bit */ 1
    );
    
    
    // display init
    displayInit();
    HAL_Delay(1000);
    
    button.rise(&triggerButton);
    
    // threads start
    t2.start(callback(test_thread, (void *)"2"));  
    test_thread((void *)"1");
}