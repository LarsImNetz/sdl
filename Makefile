# Makefile only for create a compression version of all files
# (cdw) Lars Langhans 2006
# Distributed under the terms of the GNU General Public License v2

# CVSNAME=sdl-musicplay
NAME=sdl-musicplay
VERSION=0.1
FULLNAME=$(NAME)-$(VERSION)

DOC=CHANGELOG README

include rules.mk

SUBDIRS=gfx

OBJS_MAIN_EXEC = simple.o playaudio.o ttffont.o scrollviewer.o somecolors.o directory.o storage.o t_print.o ioextend.o playlist.o ColorHelper.o rectangleWithRoundVertex.o viewdirectory.o information.o cliprect.o mvc_dispatcher.o selection.o controller.o stringhelper.o dispatcher.o listfilter.o global.o quality.o profile.o filehelper.o updaterect.o
OBJS_CHECK_MP3 = checkmp3.o t_print.o profile.o filehelper.o stringhelper.o

OBJS_GETOPT = demo_getopt.o t_print.o
OBJS_TEST_SDL = t_print.o somecolors.o rgb2hsv.o ioextend.o playlist.o ttffont.o rectangleWithRoundVertex.o ColorHelper.o stringhelper.o controller.o global.o filehelper.o profile.o updaterect.o stopwatch.o
OBJS_CHOOSER = chooser.o t_print.o somecolors.o rgb2hsv.o ioextend.o playlist.o ttffont.o rectangleWithRoundVertex.o ColorHelper.o mvc_dispatcher.o selection.o controller.o stringhelper.o global.o filehelper.o profile.o updaterect.o
OBJS_TEST_EXEC = test_exec.o
OBJS_TEST_EXEC_MPLAYER = test_exec_mplayer.o

OBJS=$(OBJS_MAIN_EXEC) $(OBJS_CHECK_MP3) $(OBJS_GETOPT) $(OBJS_TEST_SDL) $(OBJS_CHOOSER) $(OBJS_TEST_EXEC) $(OBJS_TEST_EXEC_MPLAYER) 

# info:	
# 	  @echo "Demoprogramme zur Unixprogrammierung"
# 	  @echo "Folgende Programme können erstellt werden:"
# 

EXECS=sdl-musicplay checkmp3 demo_getopt test_sdl chooser test_exec test_exec_mplayer

all:	subdirs $(EXECS)

subdirs:
	for dir in ${SUBDIRS} ; do ( cd $$dir ; ${MAKE} all ) ; done

include target.mk

# Abhängigkeiten

sdl-musicplay: $(OBJS_MAIN_EXEC)
	$(CPP) -o $@ $(OBJS_MAIN_EXEC) $(CPPFLAGS) $(DEFINE) -lSDL_mixer $(LIBS)
# pull in dependency info for existing .o files
-include $(OBJS_MAIN_EXEC:.o=.d)

checkmp3:	$(OBJS_CHECK_MP3)
	$(CPP) -o $@ $(OBJS_CHECK_MP3) $(CPPFLAGS) $(DEFINE) $(LIBS) -lSDL_sound 
-include $(OBJS_CHECK_MP3:.o=.d)

demo_getopt: $(OBJS_GETOPT)
	$(CPP) -o $@ $(OBJS_GETOPT) $(CPPFLAGS) $(DEFINE) $(LIBS)
-include $(OBJS_GETOPT:.o=.d)

test_sdl: $(OBJS_TEST_SDL)
	$(CPP) -o $@ $(OBJS_TEST_SDL) $(CPPFLAGS) $(DEFINE) $(LIBS) -Lgfx -ltest_sdl
-include $(OBJS_TEST_SDL:.o=.d)

chooser: $(OBJS_CHOOSER)
	$(CPP) -o $@ $(OBJS_CHOOSER) $(CPPFLAGS) $(DEFINE) $(LIBS)
-include $(OBJS_CHOOSER:.o=.d)

test_exec: $(OBJS_TEST_EXEC)
	$(CPP) -o $@ $(OBJS_TEST_EXEC) $(CPPFLAGS) $(DEFINE) $(LIBS)
-include $(OBJS_TEST_EXEC:.o=.d)

test_exec_mplayer: $(OBJS_TEST_EXEC_MPLAYER)
	$(CPP) -o $@ $(OBJS_TEST_EXEC_MPLAYER) $(CPPFLAGS) $(DEFINE) $(LIBS)
-include $(OBJS_TEST_EXEC_MPLAYER:.o=.d)

