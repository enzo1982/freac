# Makefile for BonkEnc

INCLUDEDIR1 = ./include
INCLUDEDIR2 = ../smooth/include
OBJECTDIR = ./objects
BINDIR = ./bin
SRCDIR = ./src
LIBDIR1 = ../smooth/lib
RESOURCEDIR = ./resources
BINRESDIR = $(RESOURCEDIR)/binary

OBJECTS = $(OBJECTDIR)/bonkenc.o $(OBJECTDIR)/cddb.o $(OBJECTDIR)/cddb_multimatch.o $(OBJECTDIR)/cddb_query.o $(OBJECTDIR)/cdtext.o $(OBJECTDIR)/config.o $(OBJECTDIR)/dllinterfaces.o $(OBJECTDIR)/encoder.o $(OBJECTDIR)/joblist.o $(OBJECTDIR)/language.o $(OBJECTDIR)/playback.o $(OBJECTDIR)/filter-in-aiff.o $(OBJECTDIR)/filter-in-au.o $(OBJECTDIR)/filter-in-bonk.o $(OBJECTDIR)/filter-in-cdrip.o $(OBJECTDIR)/filter-in-lame.o $(OBJECTDIR)/filter-in-voc.o $(OBJECTDIR)/filter-in-vorbis.o $(OBJECTDIR)/filter-in-wave.o $(OBJECTDIR)/filter-in-winamp.o $(OBJECTDIR)/inputfilter.o $(OBJECTDIR)/filter-out-blade.o $(OBJECTDIR)/filter-out-bonk.o $(OBJECTDIR)/filter-out-faac.o $(OBJECTDIR)/filter-out-lame.o $(OBJECTDIR)/filter-out-tvq.o $(OBJECTDIR)/filter-out-vorbis.o $(OBJECTDIR)/filter-out-wave.o $(OBJECTDIR)/outputfilter.o
RESOURCES = $(OBJECTDIR)/resources.o
EXEOBJECTS = $(OBJECTDIR)/bladeconfig.o $(OBJECTDIR)/bonkconfig.o $(OBJECTDIR)/cddb_extsettings.o $(OBJECTDIR)/cddb_submit.o $(OBJECTDIR)/faacconfig.o $(OBJECTDIR)/genconfig.o $(OBJECTDIR)/lameconfig.o $(OBJECTDIR)/main.o $(OBJECTDIR)/tvqconfig.o $(OBJECTDIR)/vorbisconfig.o
CMDOBJECTS = $(OBJECTDIR)/cmdmain.o $(OBJECTDIR)/console.o

EXENAME = $(BINDIR)/BonkEnc.exe
CMDNAME = $(BINDIR)/BEcmd.exe

COMPILER = gcc
RESCOMP = windres
LINKER = gcc
REMOVER = rm
ECHO = echo
COMPILER_OPTS = -I$(INCLUDEDIR1) -I$(INCLUDEDIR2) -march=i586 -O6 -g0 -Wall -Wno-pmf-conversions -fno-exceptions -DUNICODE -D_UNICODE -c
LINKER_OPTS = -L$(LIBDIR1) -lsmooth -lunicows -lshell32 -lwsock32 -mwindows -o$(EXENAME)
CMDLINKER_OPTS = -L$(LIBDIR1) -lsmooth -lunicows -lshell32 -lwsock32 -o$(CMDNAME)
REMOVER_OPTS = -f
STRIP = strip
STRIP_OPTS = --strip-all
PACKER = upx
PACKER_OPTS = -9 -q
RESCOMP_OPTS = -O coff

.PHONY: all install clean
.SILENT:

all: $(OBJECTS) $(EXEOBJECTS) $(CMDOBJECTS) $(RESOURCES) $(EXENAME) $(CMDNAME)

install:

clean:
	$(ECHO) -n Cleaning directories...
	$(REMOVER) $(REMOVER_OPTS) $(OBJECTS) $(EXEOBJECTS) $(CMDOBJECTS) $(RESOURCES) $(EXENAME) $(CMDNAME)
	$(ECHO) done.

$(EXENAME): $(OBJECTS) $(EXEOBJECTS)
	$(ECHO) -n Linking $(EXENAME)...
	$(LINKER) $(OBJECTS) $(EXEOBJECTS) $(OBJECTDIR)/libid3dll.a $(RESOURCES) $(LINKER_OPTS)
	$(STRIP) $(STRIP_OPTS) $(EXENAME)
	$(PACKER) $(PACKER_OPTS) $(EXENAME)
	$(ECHO) done.

$(CMDNAME): $(OBJECTS) $(CMDOBJECTS)
	$(ECHO) -n Linking $(CMDNAME)...
	$(LINKER) $(OBJECTS) $(CMDOBJECTS) $(OBJECTDIR)/libid3dll.a $(RESOURCES) $(CMDLINKER_OPTS)
	$(STRIP) $(STRIP_OPTS) $(CMDNAME)
	$(PACKER) $(PACKER_OPTS) $(CMDNAME)
	$(ECHO) done.

$(OBJECTDIR)/bladeconfig.o: $(SRCDIR)/bladeconfig.cpp
	$(ECHO) -n Compiling $(SRCDIR)/bladeconfig.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/bladeconfig.cpp -o $(OBJECTDIR)/bladeconfig.o
	$(ECHO) done.

$(OBJECTDIR)/bonkconfig.o: $(SRCDIR)/bonkconfig.cpp
	$(ECHO) -n Compiling $(SRCDIR)/bonkconfig.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/bonkconfig.cpp -o $(OBJECTDIR)/bonkconfig.o
	$(ECHO) done.

$(OBJECTDIR)/bonkenc.o: $(SRCDIR)/bonkenc.cpp
	$(ECHO) -n Compiling $(SRCDIR)/bonkenc.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/bonkenc.cpp -o $(OBJECTDIR)/bonkenc.o
	$(ECHO) done.

$(OBJECTDIR)/cddb.o: $(SRCDIR)/cddb.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cddb.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/cddb.cpp -o $(OBJECTDIR)/cddb.o
	$(ECHO) done.

$(OBJECTDIR)/cddb_extsettings.o: $(SRCDIR)/cddb_extsettings.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cddb_extsettings.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/cddb_extsettings.cpp -o $(OBJECTDIR)/cddb_extsettings.o
	$(ECHO) done.

$(OBJECTDIR)/cddb_multimatch.o: $(SRCDIR)/cddb_multimatch.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cddb_multimatch.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/cddb_multimatch.cpp -o $(OBJECTDIR)/cddb_multimatch.o
	$(ECHO) done.

$(OBJECTDIR)/cddb_query.o: $(SRCDIR)/cddb_query.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cddb_query.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/cddb_query.cpp -o $(OBJECTDIR)/cddb_query.o
	$(ECHO) done.

$(OBJECTDIR)/cddb_submit.o: $(SRCDIR)/cddb_submit.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cddb_submit.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/cddb_submit.cpp -o $(OBJECTDIR)/cddb_submit.o
	$(ECHO) done.

$(OBJECTDIR)/cdtext.o: $(SRCDIR)/cdtext.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cdtext.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/cdtext.cpp -o $(OBJECTDIR)/cdtext.o
	$(ECHO) done.

$(OBJECTDIR)/cmdmain.o: $(SRCDIR)/cmdmain.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cmdmain.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/cmdmain.cpp -o $(OBJECTDIR)/cmdmain.o
	$(ECHO) done.

$(OBJECTDIR)/config.o: $(SRCDIR)/config.cpp
	$(ECHO) -n Compiling $(SRCDIR)/config.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/config.cpp -o $(OBJECTDIR)/config.o
	$(ECHO) done.

$(OBJECTDIR)/console.o: $(SRCDIR)/console.cpp
	$(ECHO) -n Compiling $(SRCDIR)/console.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/console.cpp -o $(OBJECTDIR)/console.o
	$(ECHO) done.

$(OBJECTDIR)/dllinterfaces.o: $(SRCDIR)/dllinterfaces.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dllinterfaces.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dllinterfaces.cpp -o $(OBJECTDIR)/dllinterfaces.o
	$(ECHO) done.

$(OBJECTDIR)/encoder.o: $(SRCDIR)/encoder.cpp
	$(ECHO) -n Compiling $(SRCDIR)/encoder.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/encoder.cpp -o $(OBJECTDIR)/encoder.o
	$(ECHO) done.

$(OBJECTDIR)/faacconfig.o: $(SRCDIR)/faacconfig.cpp
	$(ECHO) -n Compiling $(SRCDIR)/faacconfig.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/faacconfig.cpp -o $(OBJECTDIR)/faacconfig.o
	$(ECHO) done.

$(OBJECTDIR)/genconfig.o: $(SRCDIR)/genconfig.cpp
	$(ECHO) -n Compiling $(SRCDIR)/genconfig.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/genconfig.cpp -o $(OBJECTDIR)/genconfig.o
	$(ECHO) done.

$(OBJECTDIR)/joblist.o: $(SRCDIR)/joblist.cpp
	$(ECHO) -n Compiling $(SRCDIR)/joblist.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/joblist.cpp -o $(OBJECTDIR)/joblist.o
	$(ECHO) done.

$(OBJECTDIR)/lameconfig.o: $(SRCDIR)/lameconfig.cpp
	$(ECHO) -n Compiling $(SRCDIR)/lameconfig.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/lameconfig.cpp -o $(OBJECTDIR)/lameconfig.o
	$(ECHO) done.

$(OBJECTDIR)/language.o: $(SRCDIR)/language.cpp
	$(ECHO) -n Compiling $(SRCDIR)/language.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/language.cpp -o $(OBJECTDIR)/language.o
	$(ECHO) done.

$(OBJECTDIR)/main.o: $(SRCDIR)/main.cpp
	$(ECHO) -n Compiling $(SRCDIR)/main.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/main.cpp -o $(OBJECTDIR)/main.o
	$(ECHO) done.

$(OBJECTDIR)/playback.o: $(SRCDIR)/playback.cpp
	$(ECHO) -n Compiling $(SRCDIR)/playback.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/playback.cpp -o $(OBJECTDIR)/playback.o
	$(ECHO) done.

$(OBJECTDIR)/tvqconfig.o: $(SRCDIR)/tvqconfig.cpp
	$(ECHO) -n Compiling $(SRCDIR)/tvqconfig.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/tvqconfig.cpp -o $(OBJECTDIR)/tvqconfig.o
	$(ECHO) done.

$(OBJECTDIR)/vorbisconfig.o: $(SRCDIR)/vorbisconfig.cpp
	$(ECHO) -n Compiling $(SRCDIR)/vorbisconfig.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/vorbisconfig.cpp -o $(OBJECTDIR)/vorbisconfig.o
	$(ECHO) done.

$(OBJECTDIR)/filter-in-aiff.o: $(SRCDIR)/input/filter-in-aiff.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-aiff.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/input/filter-in-aiff.cpp -o $(OBJECTDIR)/filter-in-aiff.o
	$(ECHO) done.

$(OBJECTDIR)/filter-in-au.o: $(SRCDIR)/input/filter-in-au.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-au.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/input/filter-in-au.cpp -o $(OBJECTDIR)/filter-in-au.o
	$(ECHO) done.

$(OBJECTDIR)/filter-in-bonk.o: $(SRCDIR)/input/filter-in-bonk.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-bonk.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/input/filter-in-bonk.cpp -o $(OBJECTDIR)/filter-in-bonk.o
	$(ECHO) done.

$(OBJECTDIR)/filter-in-cdrip.o: $(SRCDIR)/input/filter-in-cdrip.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-cdrip.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/input/filter-in-cdrip.cpp -o $(OBJECTDIR)/filter-in-cdrip.o
	$(ECHO) done.

$(OBJECTDIR)/filter-in-lame.o: $(SRCDIR)/input/filter-in-lame.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-lame.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/input/filter-in-lame.cpp -o $(OBJECTDIR)/filter-in-lame.o
	$(ECHO) done.

$(OBJECTDIR)/filter-in-voc.o: $(SRCDIR)/input/filter-in-voc.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-voc.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/input/filter-in-voc.cpp -o $(OBJECTDIR)/filter-in-voc.o
	$(ECHO) done.

$(OBJECTDIR)/filter-in-vorbis.o: $(SRCDIR)/input/filter-in-vorbis.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-vorbis.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/input/filter-in-vorbis.cpp -o $(OBJECTDIR)/filter-in-vorbis.o
	$(ECHO) done.

$(OBJECTDIR)/filter-in-wave.o: $(SRCDIR)/input/filter-in-wave.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-wave.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/input/filter-in-wave.cpp -o $(OBJECTDIR)/filter-in-wave.o
	$(ECHO) done.

$(OBJECTDIR)/filter-in-winamp.o: $(SRCDIR)/input/filter-in-winamp.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-winamp.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/input/filter-in-winamp.cpp -o $(OBJECTDIR)/filter-in-winamp.o
	$(ECHO) done.

$(OBJECTDIR)/inputfilter.o: $(SRCDIR)/input/inputfilter.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/inputfilter.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/input/inputfilter.cpp -o $(OBJECTDIR)/inputfilter.o
	$(ECHO) done.

$(OBJECTDIR)/filter-out-blade.o: $(SRCDIR)/output/filter-out-blade.cpp
	$(ECHO) -n Compiling $(SRCDIR)/output/filter-out-blade.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/output/filter-out-blade.cpp -o $(OBJECTDIR)/filter-out-blade.o
	$(ECHO) done.

$(OBJECTDIR)/filter-out-bonk.o: $(SRCDIR)/output/filter-out-bonk.cpp
	$(ECHO) -n Compiling $(SRCDIR)/output/filter-out-bonk.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/output/filter-out-bonk.cpp -o $(OBJECTDIR)/filter-out-bonk.o
	$(ECHO) done.

$(OBJECTDIR)/filter-out-faac.o: $(SRCDIR)/output/filter-out-faac.cpp
	$(ECHO) -n Compiling $(SRCDIR)/output/filter-out-faac.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/output/filter-out-faac.cpp -o $(OBJECTDIR)/filter-out-faac.o
	$(ECHO) done.

$(OBJECTDIR)/filter-out-lame.o: $(SRCDIR)/output/filter-out-lame.cpp
	$(ECHO) -n Compiling $(SRCDIR)/output/filter-out-lame.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/output/filter-out-lame.cpp -o $(OBJECTDIR)/filter-out-lame.o
	$(ECHO) done.

$(OBJECTDIR)/filter-out-tvq.o: $(SRCDIR)/output/filter-out-tvq.cpp
	$(ECHO) -n Compiling $(SRCDIR)/output/filter-out-tvq.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/output/filter-out-tvq.cpp -o $(OBJECTDIR)/filter-out-tvq.o
	$(ECHO) done.

$(OBJECTDIR)/filter-out-vorbis.o: $(SRCDIR)/output/filter-out-vorbis.cpp
	$(ECHO) -n Compiling $(SRCDIR)/output/filter-out-vorbis.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/output/filter-out-vorbis.cpp -o $(OBJECTDIR)/filter-out-vorbis.o
	$(ECHO) done.

$(OBJECTDIR)/filter-out-wave.o: $(SRCDIR)/output/filter-out-wave.cpp
	$(ECHO) -n Compiling $(SRCDIR)/output/filter-out-wave.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/output/filter-out-wave.cpp -o $(OBJECTDIR)/filter-out-wave.o
	$(ECHO) done.

$(OBJECTDIR)/outputfilter.o: $(SRCDIR)/output/outputfilter.cpp
	$(ECHO) -n Compiling $(SRCDIR)/output/outputfilter.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/output/outputfilter.cpp -o $(OBJECTDIR)/outputfilter.o
	$(ECHO) done.

$(OBJECTDIR)/resources.o: $(INCLUDEDIR1)/resources.h $(BINRESDIR)/bonkenc.ico
	$(ECHO) -n Compiling $(RESOURCEDIR)/resources.rc...
	$(RESCOMP) $(RESCOMP_OPTS) $(RESOURCEDIR)/resources.rc -o $(OBJECTDIR)/resources.o
	$(ECHO) done.
