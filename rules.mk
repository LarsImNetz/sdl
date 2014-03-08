# -----------------------------------------------------------------------------

DEFINE= 
CC= cc
CPP= g++
CFLAGS= -g -Wall
# CPPFLAGS    = -mpentium -O3 -Wall -funroll-loops -fexpensive-optimizations -fomit-frame-pointer -malign-loops=2 -malign-jumps=2 -malign-functions=2# -g -ggdb -Wall
# CPPFLAGS    = -mpentium -O6 -Wall -malign-loops=0 -malign-jumps=0 -malign-functions=0 # -g -ggdb -Wall

# Benoetigte Zeit: 0.103159 sec.
# CPPFLAGS    = -mpentium -O4 -Wall  -fexpensive-optimizations -fomit-frame-pointer -malign-loops=2 -malign-jumps=2 -malign-functions=2# -g -ggdb -Wall

# Benoetigte Zeit: 0.137243 sec.
COVERAGE= -ftest-coverage -fprofile-arcs
PERFOMANCE=-pg

# to use the debug start Makefile with:
# make debug=yes
# make coverage=yes
# make performance=yes
ifdef debug
	OPTIMIZE=-O0 
    DEBUG= -g3 -ggdb
else
	ifdef coverage
		DEBUG=$(COVERAGE)
	else
		ifdef performance
			DEBUG=$(PERFOMANCE)
		else
			OPTIMIZE=-O1 -Wuninitialized
		endif
	endif
endif

# WARNINGS=-Wall -Woverloaded-virtual -Wold-style-cast
WARNINGS=
# nice warning feature -Weffc++

CPPFLAGS= $(DEBUG) $(OPTIMIZE) $(WARNINGS) `sdl-config --cflags`

GPMLIB		= -lgpm
# ODBCLIB          = -lodbc
# CURSLIB          = -lncurses
AR		= ar rc
RANLIB		= ranlib

# %.o:	%.asm
# 	nasm -f elf $<

.c.o:
	$(CC) -c  $(CFLAGS) $(ADDOPTS) $*.c
	$(CC) -MM $(CFLAGS) $(ADDOPTS) $*.c >$*.d

.cc.o:
	$(CPP) -c  $(CPPFLAGS) $(ADDOPTS) $*.cc
	$(CPP) -MM $(CPPFLAGS) $(ADDOPTS) $*.cc >$*.d

.cxx.o:
	$(CPP) -c  $(CPPFLAGS) $(ADDOPTS) $*.cxx
	$(CPP) -MM $(CPPFLAGS) $(ADDOPTS) $*.cxx >$*.d


LIBS=`sdl-config --libs` -lm -lSDL_image -lSDL_ttf -lSDL_gfx
# removed due to problems with libmikmod linked to SDL_mixer AND SDL_sound
# -lSDL_sound 

