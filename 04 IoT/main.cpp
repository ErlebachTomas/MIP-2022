#include "mbed.h"

DigitalOut myled(LED1);
Serial pc(USBTX, USBRX);
 
int main() {  
    while(1) {
       pc.printf("Waiting for serial line.\r\n");
        myled = 1; // LED is ON
        wait(0.2); // 200 ms
        myled = 0; // LED is OFF
        wait(1.0); // 1 sec
    }
}
