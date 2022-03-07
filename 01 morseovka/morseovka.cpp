/* MIP 2022 Drnec Erlebach */
#include "mbed.h"

// Blinking rate in milliseconds
#define BLINKING_SHORT    500ms
#define BLINKING_LONG    1500ms // mazera jako 3 tečky
#define SYMBOL_SPACE     500ms // pauza za znakem
#define LETTER_SPACE    1000ms // 2x SYMBOL_SPACE (např. 500 + 1000, mezera mezi slovy jako tri tecky)

/// <summary>Prevede znak na odpovidajici morseuv kod (zachova mezery)</summary>
string morseEncode(char x)
{
    if (isupper(x)) {
        tolower(x); // prevede velke na male pismeno
    }

    switch (x) {
    case 'a':
        return ".-";
    case 'b':
        return "-...";
    case 'c':
        return "-.-.";
    case 'd':
        return "-..";
    case 'e':
        return ".";
    case 'f':
        return "..-.";
    case 'g':
        return "--.";
    case 'h':
        return "....";
    case 'i':
        return "..";
    case 'j':
        return ".---";
    case 'k':
        return "-.-";
    case 'l':
        return ".-..";
    case 'm':
        return "--";
    case 'n':
        return "-.";
    case 'o':
        return "---";
    case 'p':
        return ".--.";
    case 'q':
        return "--.-";
    case 'r':
        return ".-.";
    case 's':
        return "...";
    case 't':
        return "-";
    case 'u':
        return "..-";
    case 'v':
        return "...-";
    case 'w':
        return ".--";
    case 'x':
        return "-..-";
    case 'y':
        return "-.--";
    case 'z':
        return "--..";
    case '1':
        return ".----";
    case '2':
        return "..---";
    case '3':
        return "...--";
    case '4':
        return "....-";
    case '5':
        return ".....";
    case '6':
        return "-....";
    case '7':
        return "--...";
    case '8':
        return "---..";
    case '9':
        return "----.";
    case '0':
        return "-----";
    default:
        return " ";
    }
}

// <summary>zakoduje retezec do morseova kodu </summary>
string morseCode(string s) {
    int length = s.length();
    string str;

    for (int i = 0; i < length; i++) {

        str.append(morseEncode(s[i]));           
    }
    
    return str;
}

void blinkSwitch(DigitalOut led, string code) {

    int length = code.length();  

    for (int i = 0; i < length; i++) {

        blinkChar(led, code[i]);
    }

}


void blinkChar(DigitalOut led,char ch) {

    switch (ch) {    
    case '.': blink(led, BLINKING_SHORT);
              turnOffLed(led, SYMBOL_SPACE);
    case '-': blink(led, BLINKING_LONG);
              turnOffLed(led, SYMBOL_SPACE);
    default:
        turnOffLed(led, LETTER_SPACE);
       
    }


}

void blink(DigitalOut led, BLINKING_RATE) {

    led = 1;
    ThisThread::sleep_for(BLINKING_RATE);
    
}
void turnOffLed(DigitalOut led, RATE) {
    led = 0;
    ThisThread::sleep_for(RATE);
}

int main()
{
    #ifdef LED1
        DigitalOut led(LED1);
    #else
        bool led;
    #endif


    string s = "Karel Drnec a Tomas Erlebach";
        
    string str = morseCode(s);
    blinkSwitch(led,str);


    //cout << str << endl;
}
