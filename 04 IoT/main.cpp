#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "string"
#include "ctime"

#define MAXIMUM_BUFFER_SIZE  32
#define API_KEY "7BGKHY6ER3G9WR3M"
#define KAREL "M20000163"
#define TOM "M20000165"


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
        //TODO 
        char buff[MAXIMUM_BUFFER_SIZE];
        sprintf(buff,"Test!");              
        serial_port.write( buff , sizeof(buff));
       
        one_slot.release();
    
}

void sendPOST(char* key, char* time, char* student) {
    
   char buff[MAXIMUM_BUFFER_SIZE];
           
   sprintf(buff, "POST /update HTTP/1.1\r\n Host: api.thingspeak.com\r\n Content-Type: application/x-www-form-urlencoded\r\n");
   sprintf(buff, "api_key=%s&field1=%s&field2=%s",key, student, time);
   serial_port.write( buff , sizeof(buff));  
}


void triggerButton() { 
  
  sendPOST(API_KEY, "ted", KAREL);
 
  
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