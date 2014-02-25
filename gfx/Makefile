# Makefile only for create a compression version of all files
# (cdw) Lars Langhans 2006
# Distributed under the terms of the GNU General Public License v2

# CVSNAME=sdl-musicplay
PRJ=..
NAME=gfx

ADDOPTS=-I$(PRJ)

include $(PRJ)/rules.mk

all: libtest_sdl.a

include $(PRJ)/target.mk
# -----------------------------------------------------------------------------

OBJS_TEST_SDL = test_sdl.o fixfont.o

OBJS=$(OBJS_TEST_SDL)

libtest_sdl.a: $(OBJS_TEST_SDL)
#	$(CPP) -o $@ $(OBJS_TEST_SDL) $(CPPFLAGS) $(DEFINE) $(LIBS)
	$(AR) $@ $(OBJS_TEST_SDL)
#-include $(OBJS_TEST_SDL:.o=.d)
