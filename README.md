# Atari ST CONIO implementation

(c) 2017 by Matthias Arndt <marndt@asmsoftware.de>

The MIT License applies. Please see LICENSE for details.


## Abstract
conio.h is a C header file used mostly by MS-DOS compilers to provide 
console input/output. It is not part of the C standard library or ISO C, 
nor is it defined by POSIX.

This project implements the basic conio.h API for Atari ST.
Only clean GEMDOS and XBIOS calls are used.
Screen operations use Atari standard VT52 screen control codes.

## System requirements

- C89 compliant C compiler for the ST
- stdint.h optional, define NO_STDINT for conio.h to provide it

## Limitations

- Text coloring is not supported for ST-HIGH

## Use with your own Atari ST programs

- Use the provided conio.h
- Compile and link conio.c with your project

## Crosscompiling

A simple crosscompiling environment is provided to test the library and
functionality.
