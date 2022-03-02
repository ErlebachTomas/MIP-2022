/* MIP 2022 Drnec Erlebach */
#include "mbed.h"

// Blinking rate in milliseconds
#define BLINKING_SHORT    500ms
#define BLINKING_LONG    1500ms // mazera jako 3 tečky
#define SYMBOL_SPACE     500ms // pauza za znakem
#define LETTER_SPACE    1500ms // mezera mezi slovy jako tri tecky

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
    int lenght = s.length();
    string str;

    for (int i = 0; i < lenght; i++) {

        str.append(morseEncode(s[i]));           
    }
    
    return str;
}

int main()
{
    string s = "Karel Drnec a Tomas Erlebach";
        
    string str = morseCode(s);
    //cout << str << endl;
}