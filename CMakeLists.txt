cmake_minimum_required(VERSION 2.8)
set(TARGETNAME conio.tos)
project(AtariST_CONIO)
# disable strange gcc assumptions
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

# set some directories
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)

# select Atari ST cross compiler and enable assembly language source files
enable_language("ASM")

set(CMAKE_C_COMPILER m68k-atari-mint-gcc)
set(CMAKE_ASM_COMPILER m68k-atari-mint-as)
set(CMAKE_AR m68k-atari-mint-ar)
set(CMAKE_RANLIB m68k-atari-mint-ranlib)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m68000 -fomit-frame-pointer -Wextra -Wall -Wl,--traditional-format -mshort -nostdlib -std=c99")
set(CMAKE_EXE_LINKER_FLAGS  "${SRC_DIR}/testapp/startup.S" )

# GNU as configuration
set(CMAKE_ASM_FLAGS_RELEASE "")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -m68000 -I${SRC_DIR}")

# include directories
include_directories(${SRC_DIR})

# main target for testing purposes
add_executable(${TARGETNAME}
	${SRC_DIR}/testapp/main.c
	${SRC_DIR}/conio.c
	${SRC_DIR}/conio.h
)

# just a library (never used) to list some files in a generated IDE project
add_library(not_used
	${SRC_DIR}/testapp/startup.S
	README.md
	.gitignore
)
