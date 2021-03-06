/************************************************************
 * Atari ST CONIO implementation                            *
 ************************************************************
 * @file conio.c
 * @author Matthias Arndt <marndt@asmsoftware.de>
 * @copyright The MIT License applies. See LICENSE
 ************************************************************
 * @brief implementation of CONIO functionality for Atari ST
 */

/**
 * @addtogroup CONIO
 * @{
 */

#include "conio.h"

#include "osbind.h"

/* -----------------------------------------------------------------------
 * Local definitions
 * -----------------------------------------------------------------------
 */

/**
 * @brief   helper macro to output ESC sequence to console with 1 control character
 * @details The output is done with Crawio() to avoid interpretation of control codes.
 */
#define ESC(x) (void)Crawio(0x001B);(void)Crawio(x)


#define BIT(x)    (1 << (x))  /**< helper macro */

#define INIT_FLAG    BIT(0)  /**< is subsystem initalized? */
#define CURSOR_SHOWN BIT(1)  /**< is the cursor active? */
#define REVERSE_ON   BIT(2)  /**< reverse video is active? */

/**
 * @brief structure for internal variables
 */
typedef struct
{
    uint16_t flags;  /**< internal flags */
    uint8_t x;       /**< cursor x position */
    uint8_t y;       /**< cursor y position */
    uint8_t fg;      /**< foreground text color */
    uint8_t bg;      /**< background text color */
	char hexstr[5];  /**< temporary buffer for decoding hexdigits */
    uint16_t res;     /**< XBIOS 4 resolution code */
} conio_vars_t;


/* -----------------------------------------------------------------------
 * Local variables
 * -----------------------------------------------------------------------
 */

static conio_vars_t Conio; /**< allocation of internal variables */

/* -----------------------------------------------------------------------
 * Internal function implementation
 * -----------------------------------------------------------------------
 */

/**
 * @brief   asserts the CONIO system is inited
 * @details This must be called by all functions handling the internal state.
 *          The cursor is homed and default colors are set. The screen is not cleared.
 */
static void assert_init ( void )
{
	if ( ( Conio.flags & INIT_FLAG ) == 0 )
	{
		ESC ( 'H' ); /* home cursor */
		ESC ( 'v' ); /* wrap at end of line */
		ESC ( 'f' ); /* cursor off */
		Conio.x = 0;
		Conio.y = 0;
		Conio.flags = INIT_FLAG;
		textcolor ( 15 );
		bgcolor ( 0 );
		revers ( 0 ); /* revers off */
		cursor ( 0 ); /* cursor off */

        Conio.res = Getrez();
	}

	return;
}

/**
 * @brief returns length of given string in characters
 */
static int16_t StrLen ( const char * string )
{
	int16_t n = 0;

	while ( *string++ != '\0' )
	{
		n++;
	}

	return ( n );
}

/**
 * @brief decode number into hexadecimal representation
 * @details The number is stored internall in Conio.hexstr
 */
static void decode_hex ( uint16_t n )
{
	static const char hex_to_ascii[] = "0123456789ABCDEF";
	uint8_t digits = 4;
	Conio.hexstr[4] = '\0';

	while ( digits > 0 )
	{
		/* decode nybble and advance for next run */
		Conio.hexstr[digits - 1] = hex_to_ascii[ ( n & 0x000F )];
		n = n >> 4;
		/* keep track of position in string */
		digits--;
	}

	return;
}

/* -----------------------------------------------------------------------
 * Public function implementation
 * -----------------------------------------------------------------------
 */

/**
 * @brief clear the whole screen and put the cursor into the top left corner
 */
void clrscr ( void )
{
	assert_init();
	ESC ( 'E' ); /* clear screen */
	Conio.x = 0;
	Conio.y = 0;
	return;
}

/**
 * @brief   check if the keyboard has been hit or key been pressed
 * @returns true if there's a key waiting
 * @returns false if not
 */
uint8_t kbhit ( void )
{
	/* assert_init();    here not needed, no output */
	return Cconis();
}

/**
 * @brief set the cursor to the specified X position, leave the Y position untouched
 */
void  gotox ( uint8_t x )
{
	gotoxy ( x, Conio.y );
	return;
}

/**
 * @brief set the cursor to the specified Y position, leave the X position untouched
 */
void gotoy ( uint8_t y )
{
	gotoxy ( Conio.x, y );
	return;
}

/**
 * @brief set the cursor to the specified position
 */
void gotoxy ( uint8_t x, uint8_t y )
{
	assert_init();
	Conio.x = x;
	Conio.y = y;
	ESC ( 'Y' );
	( void ) Crawio ( 32+Conio.y );
	( void ) Crawio ( 32+Conio.x );
	return;
}


/**
 * @brief Return the X position of the cursor
 */
uint8_t wherex ( void )
{
	return Conio.x;
}

/**
 * @brief Return the Y position of the cursor
 */
uint8_t wherey ( void )
{
	return Conio.y;
}

/**
 * @brief Output one character at the current cursor position
 */
void cputc ( char c )
{
	Cconout ( c );
	Conio.x++;
	return;
}

/**
 * @brief Same as gotoxy (x, y); cputc (c);
 * @see   gotoxy
 * @see   cputc
 */
void cputcxy ( uint8_t x, uint8_t y, char c )
{
	gotoxy ( x,y );
	cputc ( c );
	return;
}

/**
 * @brief Output a NUL-terminated string at the current cursor position
 */
void cputs ( const char* s )
{
	uint16_t len_s = StrLen ( s );
	uint16_t written_chars = Cconws ( s );

	if ( written_chars < len_s )
	{
		Conio.x += written_chars;
	}
	else
	{
		Conio.x += len_s;
	}

	return;
}

/**
 * @brief Same as gotoxy (x, y); cputs (s);
 * @see   gotoxy
 * @see   cputs
 */
void cputsxy ( uint8_t x, uint8_t y, const char* s )
{
	gotoxy ( x,y );
	cputs ( s );
	return;
}

/**
 * @brief   get a character from keyboard
 * @details If there is no character available, the function waits until the user does press a key.
 *          If cursor is set to 1 (see below), a blinking cursor is displayed while waiting.
 */
char cgetc ( void )
{
	char c;

	/* check if cursor is to be shown: */
	if ( ( Conio.flags & CURSOR_SHOWN ) != 0 )
	{
		ESC ( 'e' ); /* show cursor */
	}

	c = ( char ) Cconin();
	ESC ( 'f' ); /* cursor off again */
	return c;
}

/**
 * @brief get a NUL-terminated string from keyboard
 */
char * cgets ( void )
{
	static char buffer[81];

	/* check if cursor is to be shown: */
	if ( ( Conio.flags & CURSOR_SHOWN ) != 0 )
	{
		ESC ( 'e' ); /* show cursor */
	}

	/* read string from keyboard */
	buffer[0] = sizeof ( buffer ) - 1;
	Cconrs ( buffer );
	ESC ( 'f' ); /* cursor off again */
	return &buffer[1];   /* first digit in string buffer contains maximum length */
}

/**
 * @brief   set cursor display state
 * @details If onoff is 1, a cursor is displayed when waiting for keyboard input. If
 *          onoff is 0, the cursor is hidden when waiting for keyboard input. The
 *          function returns the old cursor setting.
 */
uint8_t cursor ( uint8_t onoff )
{
	uint8_t old_st = 0;

	/* cursor is on? */
	if ( ( Conio.flags & CURSOR_SHOWN ) != 0 )
	{
		old_st = 1;
	}

	if ( onoff == 0 )
	{
		Conio.flags &= ~CURSOR_SHOWN;
	}
	else
	{
		Conio.flags |= CURSOR_SHOWN;
	}

	return old_st;
}

/**
 * @brief   Enable/disable reverse character display.
 * @details Return the old setting.
 */
uint8_t revers ( uint8_t onoff )
{
	uint8_t old_st = 0;

	/* reverse is on? */
	if ( ( Conio.flags & REVERSE_ON ) != 0 )
	{
		old_st = 1;
	}

	if ( onoff == 0 )
	{
		Conio.flags &= ~REVERSE_ON;
		ESC ( 'q' ); /* reverse off */
	}
	else
	{
		Conio.flags |= REVERSE_ON;
		ESC ( 'p' ); /* reverse on */
	}

	return old_st;
}

/**
 * @brief   Set the color for text output.
 * @details The old color setting is returned.
 */
uint8_t textcolor ( uint8_t color )
{
    uint8_t old_col;

    assert_init();

    old_col = Conio.fg;
	Conio.fg = ( color & 0x0F );

    if(Conio.res != 2)
    {
        ESC ( 'b' );
        ( void ) Crawio ( Conio.fg );
    }

	return old_col;
}

/**
 * @brief   Set the color for text background output.
 * @details The old color setting is returned.
 */
uint8_t bgcolor ( uint8_t color )
{
    uint8_t old_col;

    assert_init();

    old_col = Conio.bg;
	Conio.bg = ( color & 0x0F );

    if(Conio.res != 2)
    {
        ESC ( 'c' );
        ( void ) Crawio ( Conio.bg );
    }

	return old_col;
}

/**
 * @brief     Set the color for the border.
 * @details   The old color setting is returned.
 * @attention This is not implemented for Atari ST. The call is available for compatibility reasons. 0 is returned
 */
uint8_t bordercolor ( uint8_t color )
{
	( void ) color;
	return 0;
}

/**
 * @brief Output a horizontal line with the given length starting at the current cursor position.
 */
void chline ( uint8_t length )
{
	while ( length > 0 )
	{
		cputc ( '-' );
		length--;
	}

	return;
}

/**
 * @brief Same as "gotoxy (x, y); chline (length);"
 * @see gotoxy
 * @see chline
 */
void chlinexy ( uint8_t x, uint8_t y, uint8_t length )
{
	gotoxy ( x,y );
	chline ( length );
	return;
}

/**
 * @brief Output a vertical line with the given length at the current cursor position.
 */
void cvline ( uint8_t length )
{
	uint8_t x = wherex();
	uint8_t y = wherey();

	while ( length > 0 )
	{
		gotoxy ( x,y );
		cputc ( '|' );
		y++;
		length--;
	}

	return;
}

/**
 * @brief Same as "gotoxy (x, y); cvline (length);"
 * @see gotoxy
 * @see cvline
 */
void cvlinexy ( uint8_t x, uint8_t y, uint8_t length )
{
	gotoxy ( x,y );
	cvline ( length );
	return;
}

/**
 * @brief Clear part of a line (write length spaces).
 */
void cclear ( uint8_t length )
{
	while ( length > 0 )
	{
		cputc ( ' ' );
		length--;
	}

	return;
}

/**
 * @brief Same as "gotoxy (x, y); cclear (length);"
 * @see gotoxy
 * @see cclear
 */
void cclearxy ( uint8_t x, uint8_t y, uint8_t length )
{
	gotoxy ( x,y );
	cclear ( length );
	return;
}

/**
 * @brief Return the current screen size.
 */
void screensize ( uint8_t* x, uint8_t* y )
{
	/** @todo determine size from actual screen resolution */
	*x = 80;
	*y = 25;
	return;
}

/**
 * @brief    prints the hexadecimal value of the given value
 * @details  This is used for byte sized values.
 */
void cputhex8 ( uint8_t val )
{
	decode_hex ( val );
	cputs ( &Conio.hexstr[2] );
	return;
}

/**
 * @brief    prints the hexadecimal value of the given value
 * @details  This is used for word sized values.
 */
void cputhex16 ( uint16_t val )
{
	decode_hex ( val );
	cputs ( &Conio.hexstr[0] );
	return;
}

/**
 * @}
 */

