head	1.2;
access;
symbols;
locks; strict;
comment	@# @;


1.2
date	2000.04.10.06.49.13;	author Toby;	state Exp;
branches;
next	1.1;

1.1
date	99.06.26.19.11.18;	author tjones;	state Rel;
branches;
next	;


desc
@Makefile for Skipjack encryption library.
@


1.2
log
@convert skipjack to cvs
@
text
@# $Id: makefile,v 1.1 2000/04/10 06:06:50 Toby Exp $
# Makefile for skipjack

all: test.exe encrypt.exe decrypt.exe

test.exe: test.obj skipjack.obj
    $(LINKER) $(CONSOLEFLAGS) -OUT:bin\test.exe bin\test.obj bin\skipjack.obj $(GUILIBS)

test.obj: test.c skipjack.h
    $(CC) $(FLAGS) $(STDLIB) /D "_CONSOLE" test.c

encrypt.exe: encrypt.obj skipjack.obj
    $(LINKER) $(CONSOLEFLAGS) -OUT:bin\encrypt.exe bin\encrypt.obj bin\skipjack.obj $(GUILIBS)

encrypt.obj: encrypt.c skipjack.h
    $(CC) $(FLAGS) $(STDLIB) /D "_CONSOLE" encrypt.c

decrypt.exe: decrypt.obj skipjack.obj
    $(LINKER) $(CONSOLEFLAGS) -OUT:bin\decrypt.exe bin\decrypt.obj bin\skipjack.obj $(GUILIBS)

decrypt.obj: decrypt.c skipjack.h
    $(CC) $(FLAGS) $(STDLIB) /D "_CONSOLE" decrypt.c

skipjack.obj: skipjack.c
    $(CC) $(FLAGS) $(STDLIB) /D "_CONSOLE" skipjack.c

skipjack.c: skipjack.h endian.h

skipjack.h: platform.h

clean:
    del bin\test.exe
    del bin\test.obj
    del bin\encrypt.exe
    del bin\encrypt.obj
    del bin\decrypt.exe
    del bin\decrypt.obj
    del bin\skipjack.obj
    del bin\*.sbr
    del bin\vc60.*
@


1.1
log
@Initial revision
@
text
@d1 2
a2 1
# $Id: MAKEFILE 1.1 1999/06/27 02:55:50 tjones Rel $
d4 1
a4 46
# Makefile for Skipjack library.
# Copyright (c) 1998-1999 Thobias Jones.
# All rights reserved.
# If you have a new compiler to add to the list,
# email tjones@@hot-shot.com to have it included.

# default flags for GNU DOS utilities (i.e. DJGPP)
CC = gcc
CFLAGS = -c -Wall
LINK = gcc
LFLAGS = -o
DELETE = del
OBJ = o
EXE = .exe

# default flags for GNU Unix utilities (i.e. Linux)
#CC = gcc
#CFLAGS = -c -Wall
#LINK = gcc
#LFLAGS = -o
#DELETE = rm
#OBJ = o
#EXE =

# default flags for generic Unix
#CC = cc
#CFLAGS = -c
#LINK = cc
#LFLAGS = -o
#DELETE = rm
#OBJ = o
#EXE =

all: test$(EXE) encrypt$(EXE) decrypt$(EXE)

test$(EXE): test.$(OBJ) skipjack.$(OBJ)
	$(LINK) $(LFLAGS) test$(EXE) test.$(OBJ) skipjack.$(OBJ)

test.$(OBJ): test.c skipjack.h
	$(CC) $(CFLAGS) test.c

encrypt$(EXE): encrypt.$(OBJ) skipjack.$(OBJ)
	$(LINK) $(LFLAGS) encrypt$(EXE) encrypt.$(OBJ) skipjack.$(OBJ)

encrypt.$(OBJ): encrypt.c skipjack.h
	$(CC) $(CFLAGS) encrypt.c
d6 2
a7 2
decrypt$(EXE): decrypt.$(OBJ) skipjack.$(OBJ)
	$(LINK) $(LFLAGS) decrypt$(EXE) decrypt.$(OBJ) skipjack.$(OBJ)
d9 2
a10 2
decrypt.$(OBJ): decrypt.c skipjack.h
	$(CC) $(CFLAGS) decrypt.c
d12 14
a25 2
skipjack.$(OBJ): skipjack.c
	$(CC) $(CFLAGS) skipjack.c
d32 9
a40 7
	$(DELETE) test$(EXE)
	$(DELETE) test.$(OBJ)
	$(DELETE) encrypt$(EXE)
	$(DELETE) encrypt.$(OBJ)
	$(DELETE) decrypt$(EXE)
	$(DELETE) decrypt.$(OBJ)
	$(DELETE) skipjack.$(OBJ)
@

