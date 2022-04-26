#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "string"
#include "ctime"

#define API_KEY "7BGKHY6ER3G9WR3M"
#define KAREL "M20000163"
#define TOM "M20000165"
// 480x272 velikost displaye
#define btnX 150
#define btnY 150
#define btnWidth 180
#define btnHeight 50

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
    
    BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)"Aplikace pro odesilani dat", CENTER_MODE);
}

void drawUI()
{ 
 BSP_LCD_DrawRect(btnX, btnY, btnWidth, btnHeight);
 uint16_t x = 0;
 uint16_t y = btnY + 17;
 BSP_LCD_DisplayStringAt(x, y, (uint8_t *) "Odeslat!", CENTER_MODE);
 
}

void sendPost(string apiKey, string studentNumber) {
    time_t t = time(0); 
    printf("POST /update HTTP/1.1 Host:api.thingspeak.com Content-Type:application/x-www-form-urlencoded\r\n");                   
    
    const char * key = apiKey.c_str();
    const char * student = studentNumber.c_str();
    
    printf("%s%s%s%s%s%d\n", "api_key=", key, "&field1=",student , "&field2=", t);
}

void test_thread(void const *name)
{
    uint16_t x, y;   
    uint8_t idx;
    
    while(true) {
        one_slot.acquire();
        BSP_TS_GetState(&TS_State);
        if (TS_State.touchDetected) {
            
            for(idx = 0; idx < TS_State.touchDetected; idx++) {
             
               x = TS_State.touchX[idx];
               y = TS_State.touchY[idx];
                           
                if (isTouch == 0 &&  x >= btnX && x <= btnX + btnWidth && y >= btnY && y <= btnY + btnHeight ) {
                    //BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)name, CENTER_MODE);
                    char val = *(char*)name;
                    if (val == '2') {
                        BSP_LCD_Clear(LCD_COLOR_WHITE);
                        BSP_LCD_SetFont(&Font24);
                        BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
                        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
                        BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)"Posilam data...", CENTER_MODE);
                                      
                        sendPost(API_KEY, KAREL);
                        sendPost(API_KEY, TOM);
                        
                        HAL_Delay(500);
                        BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)"Data byla poslana...", CENTER_MODE);
                        
                    }
                }
                isTouch = 1;
            }
        } else {
            isTouch = 0;
        }
        drawUI();
        one_slot.release();
    }
}

int main()
{
    // display init
    displayInit();
    drawUI();
    
    HAL_Delay(1000);
    // threads start
    t2.start(callback(test_thread, (void *)"2"));
    test_thread((void *)"1");
}