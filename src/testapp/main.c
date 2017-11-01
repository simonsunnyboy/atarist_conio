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

static void wait_for_key ( void )
{
    /* prompt in revers video */
	gotoxy ( 0,24 );
	revers ( 1 );
	textcolor ( 15 );
	bgcolor ( 0 );
	cputs ( ">> press any key <<" );
	revers ( 0 );

	while ( kbhit() == 0 )
	{
		;
	}

	( void ) cgetc();
	return;
}

int main ( int argc, char **argv )
{
	uint8_t col, x,y;
	char *your_name;
	( void ) argc;
	( void ) argv;

    /* some stars with colors */
	clrscr();
	for ( y = 0; y < 3; y++ )
	{
		for ( x = 0; x < 40; x++ )
		{
			textcolor ( ( col++ % 15 )+1 );
			bgcolor ( 0 );
			cputcxy ( x,y,'*' );
		}
	}

    /*  write color codes */
	for ( col = 0; col < 16; col++ )
	{
		textcolor ( col );
		gotoxy ( 1,5+col );
		cputs ( "Color 0x" );
		cputhex8 ( col );
	}

    /* test line drawing: */
    textcolor(3);
    chlinexy(30,15,10);
    chlinexy(30,20,10);
    cvlinexy(30,16,4);
    cvlinexy(39,16,4);
	gotoxy ( 0,4 );
	textcolor ( 14 );
	chline ( 40 );
	wait_for_key();

    /* test string input with cursor shown */
	clrscr();
	textcolor ( 15 );
	cputsxy ( 0,0, "Enter your name:" );
	cursor ( 1 );
	gotoxy ( 0,1 );
	your_name = cgets();
	cursor ( 0 );
	cputsxy ( 0,2, "Your name is " );
	cputs ( your_name );
	wait_for_key();

	return 0;
}
