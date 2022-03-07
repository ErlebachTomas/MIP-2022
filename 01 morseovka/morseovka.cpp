/* MIP 2022 Drnec Erlebach */
#include "mbed.h"
#include <string>;
using namespace std;


//=====================================

// Blinking rate in milliseconds
#define BLINKING_RATE    500ms

#define BLINKING_SHORT 1
#define BLINKING_LONG    3 // mazera jako 3 tečky
#define SYMBOL_SPACE     1 // pauza za znakem
#define LETTER_SPACE    2 // 2x SYMBOL_SPACE (např. 500 + 1000, mezera mezi slovy jako tri tecky)

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

// <summary> Funkce realizující bliknutí </summary>
void blink(DigitalOut led, int length) {

    led = 1;
    for (int i = 0; i < length; i++) {
        ThisThread::sleep_for(BLINKING_RATE);
    }
}
void turnOffLed(DigitalOut led, int length) {
    led = 0;
    for (int i = 0; i < length; i++) {
        ThisThread::sleep_for(BLINKING_RATE);
    }
}

// <summary> Nastaví interval bliknutí podle znaku </summary>
void blinkChar(DigitalOut led, char ch) {

    switch (ch) {
    case '.': blink(led, BLINKING_SHORT);
        turnOffLed(led, SYMBOL_SPACE);
    case '-': blink(led, BLINKING_LONG);
        turnOffLed(led, SYMBOL_SPACE);
    default:
        turnOffLed(led, LETTER_SPACE);

    }

}

// <summary> Realizuje blikání dle morseova kódu </summary>
void blinkSwitch(DigitalOut led, string code) {

    int length = code.length();

    for (int i = 0; i < length; i++) {

        blinkChar(led, code[i]);
    }

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
    blinkSwitch(led, str);


    //cout << str << endl;
}