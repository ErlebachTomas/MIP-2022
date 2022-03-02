/* MIP 2022 Drnec Erlebach */
#include "mbed.h"

// Blinking rate in milliseconds
#define BLINKING_SHORT    500ms
#define BLINKING_LONG    1500ms // mazera jako 3 tečky
#define SYMBOL_SPACE     500ms // pauza za znakem
#define LETTER_SPACE    1500ms // mezera mezi slovy jako tri tecky

int main()
{
    // Initialise the digital pin LED1 as an output
#ifdef LED1
    DigitalOut led(LED1);
#else
    bool led;
#endif

    while (true) {
        led = !led;
        ThisThread::sleep_for(BLINKING_LONG);
    }
}
