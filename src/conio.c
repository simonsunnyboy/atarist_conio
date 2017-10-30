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

#define Print(x)  ((void)Cconws(x))  /**< cast GEMDOS error away */

#define ESC "\0x1B"  /**< helper */

#define BIT(x)    (1 << (x))  /**< helper macro */

#define INIT_FLAG    BIT(0)  /**< is subsystem initalized? */
#define CURSOR_SHOWN BIT(1)  /**< is the cursor active? */

/**
 * @brief structure for internal variables
 */
typedef struct
{
    uint16_t flags; /**< internal flags */
    uint8_t x;  /**< cursor x position */
    uint8_t y;  /**< cursor y position */
    uint8_t fg; /**< foreground text color */
    uint8_t bg; /**< background text color */
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
static void assert_init(void)
{
    if((Conio.flags & INIT_FLAG) == 0)
    {
        Print(ESC "H");  /* home cursor */
        Print(ESC "v");  /* wrap at end of line */
        Print(ESC "f" ESC "q");  /* cursor off normal video */
        Print(ESC "b\0x01" ESC "c\0x00"); /* color 1 foreground, color 0 background */

        Conio.x = 0;
        Conio.y = 0;
        Conio.fg = 1;
        Conio.bg = 0;

        Conio.flags = INIT_FLAG;
    }
    return;
}

/**
 * @brief returns length of given string in characters
 */
int16_t StrLen(const char * string)
{
    int16_t n = 0;

    while(*string++ != '\0')
    {
        n++;
    }

    return(n);
}


/* -----------------------------------------------------------------------
 * Public function implementation
 * -----------------------------------------------------------------------
 */

/**
 * @brief clear the whole screen and put the cursor into the top left corner
 */
void clrscr (void)
{
    assert_init();
    Print(ESC "E");  /* clear screen */
    Conio.x = 0;
    Conio.y = 0;
    return;
}

/**
 * @brief   check if the keyboard has been hit or key been pressed
 * @returns true if there's a key waiting
 * @returns false if not
 */
uint8_t kbhit (void)
{
    /* assert_init();    here not needed, no output */
    return Cconis();
}

/**
 * @brief set the cursor to the specified X position, leave the Y position untouched
 */
void  gotox (uint8_t x)
{
    gotoxy(x, Conio.y);
    return;
}

/**
 * @brief set the cursor to the specified Y position, leave the X position untouched
 */
void gotoy (uint8_t y)
{
    gotoxy(Conio.x, y);
    return;
}

/**
 * @brief set the cursor to the specified position
 */
void gotoxy (uint8_t x, uint8_t y)
{
    assert_init();

    Conio.x = x;
    Conio.y = y;
    Print( ESC "Y");
    Cconout(32+Conio.x);
    Cconout(32+Conio.y);

    return;
}


/**
 * @brief Return the X position of the cursor
 */
uint8_t wherex (void)
{
    return Conio.x;
}

/**
 * @brief Return the Y position of the cursor
 */
uint8_t wherey (void)
{
    return Conio.y;
}

/**
 * @brief Output one character at the current cursor position
 */
void cputc (char c)
{
    Cconout(c);
    Conio.x++;
    return;
}

/**
 * @brief Same as gotoxy (x, y); cputc (c);
 * @see   gotoxy
 * @see   cputc
 */
void cputcxy (uint8_t x, uint8_t y, char c)
{
    gotoxy(x,y);
    cputc(c);
    return;
}

/**
 * @brief Output a NUL-terminated string at the current cursor position
 */
void cputs (const char* s)
{
    uint16_t len_s = StrLen(s);
    uint16_t written_chars = Cconws(s);

    if(written_chars < len_s)
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
void cputsxy (uint8_t x, uint8_t y, const char* s)
{
    gotoxy(x,y);
    cputs(s);
    return;
}

/**
 * @brief   get a character from keyboard
 * @details If there is no character available, the function waits until the user does press a key.
 *          If cursor is set to 1 (see below), a blinking cursor is displayed while waiting.
 */
char cgetc (void)
{
    char c;

    /* check if cursor is to be shown: */
    if((Conio.flags & CURSOR_SHOWN) != 0)
    {
        Print(ESC "e");
    }

    c = (char)Cconin();
    Print(ESC "f");  /* cursor off */
    return c;
}

/**
 * @brief get a NUL-terminated string from keyboard
 */
char * cgets(void)
{
    static char buffer[81];

    /* check if cursor is to be shown: */
    if((Conio.flags & CURSOR_SHOWN) != 0)
    {
        Print(ESC "e");
    }

    /* read string from keyboard */
    buffer[0] = sizeof(buffer) - 1;
    Cconrs(buffer);

    Print(ESC "f");  /* cursor off */
    return &buffer[0];
}



/**
 * @}
 */

