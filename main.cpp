#include "TXLib.h"
#include "gameLib.h"

int main ()
    {
    igraBegin();
    bool exit= true;
    standart();
    zaz();
    while (exit)
    {
    switch (gameMenu())
        {
        case 1:
            igra();
            break;
        case 2:
            zaztavka ();
            break;
        case 3:
            exit=false;
            break;
        default :;
        }
    }
    igraEnd();
    return 0;
    }
