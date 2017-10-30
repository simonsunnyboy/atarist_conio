/************************************************************
 * Atari ST CONIO implementation                            *
 ************************************************************
 * @file conio.c
 * @author Matthias Arndt <marndt@asmsoftware.de>
 * @copyright The MIT License applies. See LICENSE
 ************************************************************
 * @brief test program for CONIO functionality
 */

#include "osbind.h"
#include "conio.h"

int main(int argc, char **argv)
{
    uint8_t col, x,y;

    (void)argc;
    (void)argv;

    clrscr();
revers(0);


    for(y = 0; y < 3; y++)
    {
        for(x = 0; x < 40; x++)
        {
            textcolor((col++ % 15)+1);
            bgcolor(0);
            cputcxy(x,y,'*');
        }
    }

    for(col = 0; col < 16; col++)
    {
        textcolor(col);
        gotoxy(1,5+col);
        cputs("Color 0x");
        cputhex8(col);
    }

    textcolor(9);
    revers(1);
    cputsxy(20,13,"TEST");
    revers(0);

    gotoxy(0,4);
    textcolor(14);
    chline(40);

    gotoxy(0,24);

    revers(1);
    textcolor(15);
    bgcolor(0);
    cputs(">> press any key <<");
    revers(0);

    Setcolor(9, 0x640);

    while(kbhit() == 0)
    {
        ;
    }
    (void)cgetc();

    return 0;
}
