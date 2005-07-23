# Makefile for BonkEnc

INCLUDEDIR1 = ./include
INCLUDEDIR2 = ../smooth/include
OBJECTDIR = ./objects
BINDIR = ./bin
SRCDIR = ./src
LIBDIR1 = ../smooth/lib
RESOURCEDIR = ./resources
BINRESDIR = $(RESOURCEDIR)/binary

OBJECTS = $(OBJECTDIR)/bonkenc.o $(OBJECTDIR)/cddb.o $(OBJECTDIR)/config.o $(OBJECTDIR)/cuesheet.o $(OBJECTDIR)/debug.o $(OBJECTDIR)/dllinterfaces.o $(OBJECTDIR)/encoder.o $(OBJECTDIR)/joblist.o $(OBJECTDIR)/playback.o $(OBJECTDIR)/playlist.o $(OBJECTDIR)/track.o $(OBJECTDIR)/utilities.o $(OBJECTDIR)/cddb_multimatch.o $(OBJECTDIR)/cddb_query.o $(OBJECTDIR)/language.o $(OBJECTDIR)/filter-in-aiff.o $(OBJECTDIR)/filter-in-au.o $(OBJECTDIR)/filter-in-bonk.o $(OBJECTDIR)/filter-in-cdrip.o $(OBJECTDIR)/filter-in-faad2.o $(OBJECTDIR)/filter-in-flac.o $(OBJECTDIR)/filter-in-lame.o $(OBJECTDIR)/filter-in-mp4.o $(OBJECTDIR)/filter-in-voc.o $(OBJECTDIR)/filter-in-vorbis.o $(OBJECTDIR)/filter-in-wave.o $(OBJECTDIR)/filter-in-winamp.o $(OBJECTDIR)/inputfilter.o $(OBJECTDIR)/filter-out-blade.o $(OBJECTDIR)/filter-out-bonk.o $(OBJECTDIR)/filter-out-faac.o $(OBJECTDIR)/filter-out-lame.o $(OBJECTDIR)/filter-out-mp4.o $(OBJECTDIR)/filter-out-tvq.o $(OBJECTDIR)/filter-out-vorbis.o $(OBJECTDIR)/filter-out-wave.o $(OBJECTDIR)/outputfilter.o
RESOURCES = $(OBJECTDIR)/resources.o
EXEOBJECTS = $(OBJECTDIR)/cddb_extsettings.o $(OBJECTDIR)/cddb_submit.o $(OBJECTDIR)/genconfig.o $(OBJECTDIR)/genconfig_cddb.o $(OBJECTDIR)/genconfig_cdrip.o $(OBJECTDIR)/genconfig_encoders.o $(OBJECTDIR)/genconfig_language.o $(OBJECTDIR)/genconfig_plugins.o $(OBJECTDIR)/genconfig_tags.o $(OBJECTDIR)/bladeconfig.o $(OBJECTDIR)/bonkconfig.o $(OBJECTDIR)/faacconfig.o $(OBJECTDIR)/lameconfig.o $(OBJECTDIR)/tvqconfig.o $(OBJECTDIR)/vorbisconfig.o $(OBJECTDIR)/main.o
CMDOBJECTS = $(OBJECTDIR)/cmdmain.o

EXENAME = $(BINDIR)/BonkEnc.exe
CMDNAME = $(BINDIR)/BEcmd.exe

COMPILER = gcc
RESCOMP = windres
LINKER = gcc
REMOVER = rm
ECHO = echo
COMPILER_OPTS = -I$(INCLUDEDIR1) -I$(INCLUDEDIR2) -march=i586 -Os -g0 -Wall -Wno-pmf-conversions -fno-exceptions -DUNICODE -D_UNICODE -DID3LIB_LINKOPTION=LINKOPTION_USE_DYNAMIC -c
LINKER_OPTS = -L$(LIBDIR1) -lsmooth -lunicows -lshell32 -lws2_32 -lole32 -lstdc++ -mwindows -o$(EXENAME)
CMDLINKER_OPTS = -L$(LIBDIR1) -lsmooth -lunicows -lshell32 -lws2_32 -lole32 -lstdc++ -o$(CMDNAME)
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
	$(LINKER) $(OBJECTS) $(EXEOBJECTS) $(RESOURCES) $(LINKER_OPTS)
	$(STRIP) $(STRIP_OPTS) $(EXENAME)
	$(PACKER) $(PACKER_OPTS) $(EXENAME)
	$(ECHO) done.

$(CMDNAME): $(OBJECTS) $(CMDOBJECTS)
	$(ECHO) -n Linking $(CMDNAME)...
	$(LINKER) $(OBJECTS) $(CMDOBJECTS) $(RESOURCES) $(CMDLINKER_OPTS)
	$(STRIP) $(STRIP_OPTS) $(CMDNAME)
	$(PACKER) $(PACKER_OPTS) $(CMDNAME)
	$(ECHO) done.

$(OBJECTDIR)/bladeconfig.o: $(SRCDIR)/dialogs/bladeconfig.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/bladeconfig.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/bladeconfig.cpp -o $(OBJECTDIR)/bladeconfig.o
	$(ECHO) done.

$(OBJECTDIR)/bonkconfig.o: $(SRCDIR)/dialogs/bonkconfig.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/bonkconfig.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/bonkconfig.cpp -o $(OBJECTDIR)/bonkconfig.o
	$(ECHO) done.

$(OBJECTDIR)/bonkenc.o: $(SRCDIR)/bonkenc.cpp
	$(ECHO) -n Compiling $(SRCDIR)/bonkenc.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/bonkenc.cpp -o $(OBJECTDIR)/bonkenc.o
	$(ECHO) done.

$(OBJECTDIR)/cddb.o: $(SRCDIR)/cddb.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cddb.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/cddb.cpp -o $(OBJECTDIR)/cddb.o
	$(ECHO) done.

$(OBJECTDIR)/cddb_extsettings.o: $(SRCDIR)/dialogs/cddb/extsettings.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/cddb/extsettings.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/cddb/extsettings.cpp -o $(OBJECTDIR)/cddb_extsettings.o
	$(ECHO) done.

$(OBJECTDIR)/cddb_multimatch.o: $(SRCDIR)/dialogs/cddb/multimatch.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/cddb/multimatch.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/cddb/multimatch.cpp -o $(OBJECTDIR)/cddb_multimatch.o
	$(ECHO) done.

$(OBJECTDIR)/cddb_query.o: $(SRCDIR)/dialogs/cddb/query.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/cddb/query.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/cddb/query.cpp -o $(OBJECTDIR)/cddb_query.o
	$(ECHO) done.

$(OBJECTDIR)/cddb_submit.o: $(SRCDIR)/dialogs/cddb/submit.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/cddb/submit.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/cddb/submit.cpp -o $(OBJECTDIR)/cddb_submit.o
	$(ECHO) done.

$(OBJECTDIR)/cmdmain.o: $(SRCDIR)/cmdmain.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cmdmain.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/cmdmain.cpp -o $(OBJECTDIR)/cmdmain.o
	$(ECHO) done.

$(OBJECTDIR)/config.o: $(SRCDIR)/config.cpp
	$(ECHO) -n Compiling $(SRCDIR)/config.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/config.cpp -o $(OBJECTDIR)/config.o
	$(ECHO) done.

$(OBJECTDIR)/cuesheet.o: $(SRCDIR)/cuesheet.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cuesheet.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/cuesheet.cpp -o $(OBJECTDIR)/cuesheet.o
	$(ECHO) done.

$(OBJECTDIR)/debug.o: $(SRCDIR)/debug.cpp
	$(ECHO) -n Compiling $(SRCDIR)/debug.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/debug.cpp -o $(OBJECTDIR)/debug.o
	$(ECHO) done.

$(OBJECTDIR)/dllinterfaces.o: $(SRCDIR)/dllinterfaces.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dllinterfaces.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dllinterfaces.cpp -o $(OBJECTDIR)/dllinterfaces.o
	$(ECHO) done.

$(OBJECTDIR)/encoder.o: $(SRCDIR)/encoder.cpp
	$(ECHO) -n Compiling $(SRCDIR)/encoder.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/encoder.cpp -o $(OBJECTDIR)/encoder.o
	$(ECHO) done.

$(OBJECTDIR)/faacconfig.o: $(SRCDIR)/dialogs/faacconfig.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/faacconfig.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/faacconfig.cpp -o $(OBJECTDIR)/faacconfig.o
	$(ECHO) done.

$(OBJECTDIR)/joblist.o: $(SRCDIR)/joblist.cpp
	$(ECHO) -n Compiling $(SRCDIR)/joblist.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/joblist.cpp -o $(OBJECTDIR)/joblist.o
	$(ECHO) done.

$(OBJECTDIR)/lameconfig.o: $(SRCDIR)/dialogs/lameconfig.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/lameconfig.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/lameconfig.cpp -o $(OBJECTDIR)/lameconfig.o
	$(ECHO) done.

$(OBJECTDIR)/language.o: $(SRCDIR)/dialogs/language.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/language.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/language.cpp -o $(OBJECTDIR)/language.o
	$(ECHO) done.

$(OBJECTDIR)/main.o: $(SRCDIR)/main.cpp
	$(ECHO) -n Compiling $(SRCDIR)/main.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/main.cpp -o $(OBJECTDIR)/main.o
	$(ECHO) done.

$(OBJECTDIR)/playback.o: $(SRCDIR)/playback.cpp
	$(ECHO) -n Compiling $(SRCDIR)/playback.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/playback.cpp -o $(OBJECTDIR)/playback.o
	$(ECHO) done.

$(OBJECTDIR)/playlist.o: $(SRCDIR)/playlist.cpp
	$(ECHO) -n Compiling $(SRCDIR)/playlist.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/playlist.cpp -o $(OBJECTDIR)/playlist.o
	$(ECHO) done.

$(OBJECTDIR)/track.o: $(SRCDIR)/track.cpp
	$(ECHO) -n Compiling $(SRCDIR)/track.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/track.cpp -o $(OBJECTDIR)/track.o
	$(ECHO) done.

$(OBJECTDIR)/utilities.o: $(SRCDIR)/utilities.cpp
	$(ECHO) -n Compiling $(SRCDIR)/utilities.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/utilities.cpp -o $(OBJECTDIR)/utilities.o
	$(ECHO) done.

$(OBJECTDIR)/tvqconfig.o: $(SRCDIR)/dialogs/tvqconfig.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/tvqconfig.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/tvqconfig.cpp -o $(OBJECTDIR)/tvqconfig.o
	$(ECHO) done.

$(OBJECTDIR)/vorbisconfig.o: $(SRCDIR)/dialogs/vorbisconfig.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/vorbisconfig.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/vorbisconfig.cpp -o $(OBJECTDIR)/vorbisconfig.o
	$(ECHO) done.

$(OBJECTDIR)/genconfig.o: $(SRCDIR)/dialogs/genconfig/genconfig.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/genconfig/genconfig.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/genconfig/genconfig.cpp -o $(OBJECTDIR)/genconfig.o
	$(ECHO) done.

$(OBJECTDIR)/genconfig_cddb.o: $(SRCDIR)/dialogs/genconfig/genconfig_cddb.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/genconfig/genconfig_cddb.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/genconfig/genconfig_cddb.cpp -o $(OBJECTDIR)/genconfig_cddb.o
	$(ECHO) done.

$(OBJECTDIR)/genconfig_cdrip.o: $(SRCDIR)/dialogs/genconfig/genconfig_cdrip.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/genconfig/genconfig_cdrip.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/genconfig/genconfig_cdrip.cpp -o $(OBJECTDIR)/genconfig_cdrip.o
	$(ECHO) done.

$(OBJECTDIR)/genconfig_encoders.o: $(SRCDIR)/dialogs/genconfig/genconfig_encoders.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/genconfig/genconfig_encoders.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/genconfig/genconfig_encoders.cpp -o $(OBJECTDIR)/genconfig_encoders.o
	$(ECHO) done.

$(OBJECTDIR)/genconfig_language.o: $(SRCDIR)/dialogs/genconfig/genconfig_language.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/genconfig/genconfig_language.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/genconfig/genconfig_language.cpp -o $(OBJECTDIR)/genconfig_language.o
	$(ECHO) done.

$(OBJECTDIR)/genconfig_plugins.o: $(SRCDIR)/dialogs/genconfig/genconfig_plugins.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/genconfig/genconfig_plugins.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/genconfig/genconfig_plugins.cpp -o $(OBJECTDIR)/genconfig_plugins.o
	$(ECHO) done.

$(OBJECTDIR)/genconfig_tags.o: $(SRCDIR)/dialogs/genconfig/genconfig_tags.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/genconfig/genconfig_tags.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/genconfig/genconfig_tags.cpp -o $(OBJECTDIR)/genconfig_tags.o
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

$(OBJECTDIR)/filter-in-faad2.o: $(SRCDIR)/input/filter-in-faad2.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-faad2.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/input/filter-in-faad2.cpp -o $(OBJECTDIR)/filter-in-faad2.o
	$(ECHO) done.

$(OBJECTDIR)/filter-in-flac.o: $(SRCDIR)/input/filter-in-flac.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-flac.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/input/filter-in-flac.cpp -o $(OBJECTDIR)/filter-in-flac.o
	$(ECHO) done.

$(OBJECTDIR)/filter-in-lame.o: $(SRCDIR)/input/filter-in-lame.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-lame.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/input/filter-in-lame.cpp -o $(OBJECTDIR)/filter-in-lame.o
	$(ECHO) done.

$(OBJECTDIR)/filter-in-mp4.o: $(SRCDIR)/input/filter-in-mp4.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-mp4.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/input/filter-in-mp4.cpp -o $(OBJECTDIR)/filter-in-mp4.o
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

$(OBJECTDIR)/filter-out-mp4.o: $(SRCDIR)/output/filter-out-mp4.cpp
	$(ECHO) -n Compiling $(SRCDIR)/output/filter-out-mp4.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/output/filter-out-mp4.cpp -o $(OBJECTDIR)/filter-out-mp4.o
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
