/************************************************************
 * Atari ST CONIO implementation                            *
 ************************************************************
 * @file conio.h
 * @author Matthias Arndt <marndt@asmsoftware.de>
 * @copyright The MIT License applies. See LICENSE
 ************************************************************
 * @brief public API for CONIO
 */

/**
 * @addtogroup CONIO
 * @{
 */

#ifndef CONIO_H
    #define CONIO_H

    /* check if stdint.h is available: */
    #ifdef NO_STDINT
        /* in this case provide basic types: */
        typedef unsigned char uint8_t;
        typedef unsigned short uint16_t;
        typedef signed short int16_t;
    #else
        #include <stdint.h>
    #endif

    /**
     * @brief clear the whole screen and put the cursor into the top left corner
     */
    void clrscr (void);

    /**
     * @brief   check if the keyboard has been hit or key been pressed
     * @returns true if there's a key waiting
     * @returns false if not
     */
    uint8_t kbhit (void);

    /**
     * @brief set the cursor to the specified X position, leave the Y position untouched
     */
    void  gotox (uint8_t x);

    /**
     * @brief set the cursor to the specified Y position, leave the X position untouched
     */
    void gotoy (uint8_t y);

    /**
     * @brief set the cursor to the specified position
     */
    void gotoxy (uint8_t x, uint8_t y);

    /**
     * @brief Return the X position of the cursor
     */
    uint8_t wherex (void);

    /**
     * @brief Return the Y position of the cursor
     */
    uint8_t wherey (void);

    /**
     * @brief Output one character at the current cursor position
     */
    void cputc (char c);

    /**
     * @brief Same as gotoxy (x, y); cputc (c);
     * @see   gotoxy
     * @see   cputc
     */
    void cputcxy (uint8_t x, uint8_t y, char c);

    /**
     * @brief Output a NUL-terminated string at the current cursor position
     */
    void cputs (const char* s);

    /**
     * @brief Same as gotoxy (x, y); cputs (s);
     * @see   gotoxy
     * @see   cputs
     */
    void cputsxy (uint8_t x, uint8_t y, const char* s);

    /**
     * @brief   get a character from keyboard
     * @details If there is no character available, the function waits until the user does press a key.
     *          If cursor is set to 1 (see below), a blinking cursor is displayed while waiting.
     */
    char cgetc (void);

    /**
     * @brief get a NUL-terminated string from keyboard
     */
    char * cgets(void);

    /**
     * @brief   set cursor display state
     * @details If onoff is 1, a cursor is displayed when waiting for keyboard input. If
     *          onoff is 0, the cursor is hidden when waiting for keyboard input. The
     *          function returns the old cursor setting.
     */
    uint8_t cursor (uint8_t onoff);

    /**
     * @brief   Enable/disable reverse character display.
     * @details Return the old setting.
     */
    uint8_t revers (uint8_t onoff);

    /**
     * @brief   Set the color for text output.
     * @details The old color setting is returned.
     */
    uint8_t textcolor (uint8_t color);

    /**
     * @brief   Set the color for text background output.
     * @details The old color setting is returned.
     */
    uint8_t bgcolor (uint8_t color);

    /**
     * @brief     Set the color for the border.
     * @details   The old color setting is returned.
     * @attention This is not implemented for Atari ST. The call is available for compatibility reasons. 0 is returned
     */
    uint8_t bordercolor (uint8_t color);

    /**
     * @brief Output a horizontal line with the given length starting at the current cursor position.
     */
    void chline (uint8_t length);

    /**
     * @brief Same as "gotoxy (x, y); chline (length);"
     * @see gotoxy
     * @see chline
     */
    void chlinexy (uint8_t x, uint8_t y, uint8_t length);


    /**
     * @brief Output a vertical line with the given length at the current cursor position.
     */
    void cvline (uint8_t length);

    /**
     * @brief Same as "gotoxy (x, y); cvline (length);"
     * @see gotoxy
     * @see cvline
     */
    void cvlinexy (uint8_t x, uint8_t y, uint8_t length);

    /**
     * @brief Clear part of a line (write length spaces).
     */
    void cclear (uint8_t length);

    /**
     * @brief Same as "gotoxy (x, y); cclear (length);"
     * @see gotoxy
     * @see cclear
     */
    void cclearxy (uint8_t x, uint8_t y, uint8_t length);

    /**
     * @brief Return the current screen size.
     */
    void screensize (uint8_t* x, uint8_t* y);

    /**
     * @brief    prints the hexadecimal value of the given value
     * @details  This is used for byte sized values.
     */
    void cputhex8 (uint8_t val);

    /**
     * @brief    prints the hexadecimal value of the given value
     * @details  This is used for word sized values.
     */
    void cputhex16 (uint16_t val);

#endif

/**
 * @}
 */
