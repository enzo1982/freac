# Makefile for fre:ac

include Makefile-options

INCLUDEDIR1 = ./include
INCLUDEDIR2 = ../smooth/include
OBJECTDIR = ./objects
BINDIR = ./bin
SRCDIR = ./src
LIBDIR1 = ../smooth/lib
RESOURCEDIR = ./resources
BINRESDIR = $(RESOURCEDIR)/binary

DLLOBJECTS = $(OBJECTDIR)/cddb.o $(OBJECTDIR)/cddbcache.o $(OBJECTDIR)/cddbinfo.o $(OBJECTDIR)/cddblocal.o $(OBJECTDIR)/cddbremote.o $(OBJECTDIR)/cdinfo.o $(OBJECTDIR)/cdplayerini.o $(OBJECTDIR)/cdtext.o $(OBJECTDIR)/filter-in-aiff.o $(OBJECTDIR)/filter-in-au.o $(OBJECTDIR)/filter-in-bonk.o $(OBJECTDIR)/filter-in-faad2.o $(OBJECTDIR)/filter-in-flac.o $(OBJECTDIR)/filter-in-mad.o $(OBJECTDIR)/filter-in-mp4.o $(OBJECTDIR)/filter-in-voc.o $(OBJECTDIR)/filter-in-vorbis.o $(OBJECTDIR)/filter-in-wave.o $(OBJECTDIR)/inputfilter.o $(OBJECTDIR)/filter-out-bonk.o $(OBJECTDIR)/filter-out-faac.o $(OBJECTDIR)/filter-out-flac.o $(OBJECTDIR)/filter-out-lame.o $(OBJECTDIR)/filter-out-mp4.o $(OBJECTDIR)/filter-out-vorbis.o $(OBJECTDIR)/filter-out-wave.o $(OBJECTDIR)/outputfilter.o $(OBJECTDIR)/bonkenc.o $(OBJECTDIR)/config.o $(OBJECTDIR)/cuesheet.o $(OBJECTDIR)/debug.o $(OBJECTDIR)/dllinterfaces.o $(OBJECTDIR)/encoder.o $(OBJECTDIR)/joblist.o $(OBJECTDIR)/picture.o $(OBJECTDIR)/playlist.o $(OBJECTDIR)/track.o $(OBJECTDIR)/utilities.o

ifeq ($(BUILD_WIN32),True)
	DLLOBJECTS += $(OBJECTDIR)/filter-in-cdrip.o $(OBJECTDIR)/filter-in-winamp.o $(OBJECTDIR)/filter-in-wma.o $(OBJECTDIR)/filter-out-blade.o $(OBJECTDIR)/filter-out-tvq.o $(OBJECTDIR)/filter-out-wma.o
	RESOURCES = $(OBJECTDIR)/resources.o
endif

EXEOBJECTS = $(OBJECTDIR)/cddbbatch.o $(OBJECTDIR)/cddb_extsettings.o $(OBJECTDIR)/cddb_manage.o $(OBJECTDIR)/cddb_managequeries.o $(OBJECTDIR)/cddb_managesubmits.o $(OBJECTDIR)/cddb_multimatch.o $(OBJECTDIR)/cddb_query.o $(OBJECTDIR)/cddb_submit.o $(OBJECTDIR)/genconfig.o $(OBJECTDIR)/genconfig_encoders.o $(OBJECTDIR)/genconfig_language.o $(OBJECTDIR)/genconfig_playlists.o $(OBJECTDIR)/genconfig_tags.o $(OBJECTDIR)/adddirectory.o $(OBJECTDIR)/addpattern.o $(OBJECTDIR)/bonkconfig.o $(OBJECTDIR)/donate.o $(OBJECTDIR)/faacconfig.o $(OBJECTDIR)/flacconfig.o $(OBJECTDIR)/lameconfig.o $(OBJECTDIR)/vorbisconfig.o $(OBJECTDIR)/main.o

ifeq ($(BUILD_WIN32),True)
	EXEOBJECTS += $(OBJECTDIR)/genconfig_cddb.o $(OBJECTDIR)/genconfig_cdrip.o $(OBJECTDIR)/genconfig_plugins.o $(OBJECTDIR)/bladeconfig.o $(OBJECTDIR)/tvqconfig.o $(OBJECTDIR)/wmaconfig.o $(OBJECTDIR)/playback.o
endif

CMDOBJECTS = $(OBJECTDIR)/cmdmain.o

EXENAME = $(BINDIR)/freac$(EXECUTABLE)
CMDNAME = $(BINDIR)/freaccmd$(EXECUTABLE)
DLLNAME = $(BINDIR)/freac$(SHARED)
LIBNAME = $(OBJECTDIR)/libfreac.a

COMPILER = gcc
RESCOMP = windres
LINKER = gcc
REMOVER = rm
ECHO = echo
COMPILER_OPTS = -I$(INCLUDEDIR1) -I$(INCLUDEDIR2) -m32 -march=i586 -Os -g0 -Wall -fno-exceptions -DUNICODE -D_UNICODE -DID3LIB_LINKOPTION=LINKOPTION_USE_DYNAMIC -c
LINKER_OPTS = -L$(LIBDIR1) $(LIBNAME) -m32 -lsmooth -lstdc++ -o$(EXENAME)
CMDLINKER_OPTS = -L$(LIBDIR1) $(LIBNAME) -m32 -lsmooth -lstdc++ -o$(CMDNAME)
DLLLINKER_OPTS = -L$(LIBDIR1) -m32 -lsmooth -lstdc++ --shared -o$(DLLNAME)
REMOVER_OPTS = -f
STRIP = strip
STRIP_OPTS = --strip-all
RESCOMP_OPTS = -O coff --target=pe-i386

ifneq ($(BUILD_WIN32),True)
ifneq ($(BUILD_SOLARIS),True)
ifneq ($(BUILD_QNX),True)
	COMPILER_OPTS			+= -pthread
endif
endif
endif

ifeq ($(BUILD_OPENBSD),True)
	LINKER_OPTS			+= -L/usr/local/lib -logg -lvorbis -lvorbisfile
	CMDLINKER_OPTS			+= -L/usr/local/lib -logg -lvorbis -lvorbisfile
endif

ifeq ($(BUILD_SOLARIS),True)
	COMPILER_OPTS			+= -fpic
endif

ifeq ($(BUILD_WIN32),True)
	DLLLINKER_OPTS			+= -Wl,--dynamicbase,--nxcompat -lunicows -lole32 -lws2_32 -luuid -Wl,--out-implib,$(LIBNAME)
	LINKER_OPTS			+= -Wl,--dynamicbase,--nxcompat -lunicows -lole32 -lwinmm -mwindows
	CMDLINKER_OPTS			+= -Wl,--dynamicbase,--nxcompat -lunicows
else
	ifeq ($(BUILD_OSX),True)
		DLLLINKER_OPTS		+= -Wl,-dylib_install_name,freac$(SHARED)
	endif

	DLLLINKER_OPTS			+= -Wl,-rpath,.
	LINKER_OPTS			+= -Wl,-rpath,.
	CMDLINKER_OPTS			+= -Wl,-rpath,.
endif

.PHONY: all headers install clean clean_headers
.SILENT:

all: $(HEADERS) $(DLLOBJECTS) $(EXEOBJECTS) $(CMDOBJECTS) $(RESOURCES) $(DLLNAME) $(EXENAME) $(CMDNAME)

install:

clean:
	$(ECHO) -n Cleaning directories...
	$(REMOVER) $(REMOVER_OPTS) $(OBJECTS) $(DLLOBJECTS) $(EXEOBJECTS) $(CMDOBJECTS) $(RESOURCES) $(DLLNAME) $(EXENAME) $(CMDNAME) $(LIBNAME)
	$(ECHO) done.

$(DLLNAME): $(DLLOBJECTS)
	$(ECHO) Linking $(DLLNAME)...
	$(LINKER) $(DLLOBJECTS) $(DLLLINKER_OPTS)
ifneq ($(BUILD_OSX),True)
	$(STRIP) $(STRIP_OPTS) $(DLLNAME)
endif
ifeq ($(BUILD_WIN32),True)
	countbuild BuildNumber
endif
	$(ECHO) done.

$(EXENAME): $(EXEOBJECTS) $(RESOURCES)
	$(ECHO) -n Linking $(EXENAME)...
	$(LINKER) $(EXEOBJECTS) $(RESOURCES) $(LINKER_OPTS)
ifneq ($(BUILD_OSX),True)
	$(STRIP) $(STRIP_OPTS) $(EXENAME)
endif
	$(ECHO) done.

$(CMDNAME): $(CMDOBJECTS) $(RESOURCES)
	$(ECHO) -n Linking $(CMDNAME)...
	$(LINKER) $(CMDOBJECTS) $(RESOURCES) $(CMDLINKER_OPTS)
ifneq ($(BUILD_OSX),True)
	$(STRIP) $(STRIP_OPTS) $(CMDNAME)
endif
	$(ECHO) done.

$(OBJECTDIR)/bladeconfig.o: $(SRCDIR)/dialogs/bladeconfig.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/bladeconfig.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/bladeconfig.cpp -o $(OBJECTDIR)/bladeconfig.o
	$(ECHO) done.

$(OBJECTDIR)/bonkconfig.o: $(SRCDIR)/dialogs/bonkconfig.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/bonkconfig.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/bonkconfig.cpp -o $(OBJECTDIR)/bonkconfig.o
	$(ECHO) done.

$(OBJECTDIR)/faacconfig.o: $(SRCDIR)/dialogs/faacconfig.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/faacconfig.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/faacconfig.cpp -o $(OBJECTDIR)/faacconfig.o
	$(ECHO) done.

$(OBJECTDIR)/flacconfig.o: $(SRCDIR)/dialogs/flacconfig.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/flacconfig.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/flacconfig.cpp -o $(OBJECTDIR)/flacconfig.o
	$(ECHO) done.

$(OBJECTDIR)/lameconfig.o: $(SRCDIR)/dialogs/lameconfig.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/lameconfig.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/lameconfig.cpp -o $(OBJECTDIR)/lameconfig.o
	$(ECHO) done.

$(OBJECTDIR)/tvqconfig.o: $(SRCDIR)/dialogs/tvqconfig.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/tvqconfig.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/tvqconfig.cpp -o $(OBJECTDIR)/tvqconfig.o
	$(ECHO) done.

$(OBJECTDIR)/vorbisconfig.o: $(SRCDIR)/dialogs/vorbisconfig.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/vorbisconfig.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/vorbisconfig.cpp -o $(OBJECTDIR)/vorbisconfig.o
	$(ECHO) done.

$(OBJECTDIR)/wmaconfig.o: $(SRCDIR)/dialogs/wmaconfig.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/wmaconfig.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/wmaconfig.cpp -o $(OBJECTDIR)/wmaconfig.o
	$(ECHO) done.

$(OBJECTDIR)/cddb.o: $(SRCDIR)/cddb/cddb.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cddb/cddb.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/cddb/cddb.cpp -o $(OBJECTDIR)/cddb.o
	$(ECHO) done.

$(OBJECTDIR)/cddbbatch.o: $(SRCDIR)/cddb/cddbbatch.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cddb/cddbbatch.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/cddb/cddbbatch.cpp -o $(OBJECTDIR)/cddbbatch.o
	$(ECHO) done.

$(OBJECTDIR)/cddbcache.o: $(SRCDIR)/cddb/cddbcache.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cddb/cddbcache.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/cddb/cddbcache.cpp -o $(OBJECTDIR)/cddbcache.o
	$(ECHO) done.

$(OBJECTDIR)/cddbinfo.o: $(SRCDIR)/cddb/cddbinfo.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cddb/cddbinfo.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/cddb/cddbinfo.cpp -o $(OBJECTDIR)/cddbinfo.o
	$(ECHO) done.

$(OBJECTDIR)/cddblocal.o: $(SRCDIR)/cddb/cddblocal.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cddb/cddblocal.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/cddb/cddblocal.cpp -o $(OBJECTDIR)/cddblocal.o
	$(ECHO) done.

$(OBJECTDIR)/cddbremote.o: $(SRCDIR)/cddb/cddbremote.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cddb/cddbremote.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/cddb/cddbremote.cpp -o $(OBJECTDIR)/cddbremote.o
	$(ECHO) done.

$(OBJECTDIR)/cdinfo.o: $(SRCDIR)/cdinfo/cdinfo.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cdinfo/cdinfo.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/cdinfo/cdinfo.cpp -o $(OBJECTDIR)/cdinfo.o
	$(ECHO) done.

$(OBJECTDIR)/cdplayerini.o: $(SRCDIR)/cdinfo/cdplayerini.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cdinfo/cdplayerini.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/cdinfo/cdplayerini.cpp -o $(OBJECTDIR)/cdplayerini.o
	$(ECHO) done.

$(OBJECTDIR)/cdtext.o: $(SRCDIR)/cdinfo/cdtext.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cdinfo/cdtext.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/cdinfo/cdtext.cpp -o $(OBJECTDIR)/cdtext.o
	$(ECHO) done.

$(OBJECTDIR)/cddb_extsettings.o: $(SRCDIR)/dialogs/cddb/extsettings.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/cddb/extsettings.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/cddb/extsettings.cpp -o $(OBJECTDIR)/cddb_extsettings.o
	$(ECHO) done.

$(OBJECTDIR)/cddb_manage.o: $(SRCDIR)/dialogs/cddb/manage.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/cddb/manage.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/cddb/manage.cpp -o $(OBJECTDIR)/cddb_manage.o
	$(ECHO) done.

$(OBJECTDIR)/cddb_managequeries.o: $(SRCDIR)/dialogs/cddb/managequeries.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/cddb/managequeries.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/cddb/managequeries.cpp -o $(OBJECTDIR)/cddb_managequeries.o
	$(ECHO) done.

$(OBJECTDIR)/cddb_managesubmits.o: $(SRCDIR)/dialogs/cddb/managesubmits.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/cddb/managesubmits.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/cddb/managesubmits.cpp -o $(OBJECTDIR)/cddb_managesubmits.o
	$(ECHO) done.

$(OBJECTDIR)/cddb_multimatch.o: $(SRCDIR)/dialogs/cddb/multimatch.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/cddb/multimatch.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/cddb/multimatch.cpp -o $(OBJECTDIR)/cddb_multimatch.o
	$(ECHO) done.

$(OBJECTDIR)/cddb_query.o: $(SRCDIR)/dialogs/cddb/query.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/cddb/query.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/cddb/query.cpp -o $(OBJECTDIR)/cddb_query.o
	$(ECHO) done.

$(OBJECTDIR)/cddb_submit.o: $(SRCDIR)/dialogs/cddb/submit.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/cddb/submit.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/cddb/submit.cpp -o $(OBJECTDIR)/cddb_submit.o
	$(ECHO) done.

$(OBJECTDIR)/bonkenc.o: $(SRCDIR)/bonkenc.cpp
	$(ECHO) -n Compiling $(SRCDIR)/bonkenc.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/bonkenc.cpp -o $(OBJECTDIR)/bonkenc.o
	$(ECHO) done.

$(OBJECTDIR)/cmdmain.o: $(SRCDIR)/cmdmain.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cmdmain.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/cmdmain.cpp -o $(OBJECTDIR)/cmdmain.o
	$(ECHO) done.

$(OBJECTDIR)/config.o: $(SRCDIR)/config.cpp
	$(ECHO) -n Compiling $(SRCDIR)/config.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/config.cpp -o $(OBJECTDIR)/config.o
	$(ECHO) done.

$(OBJECTDIR)/cuesheet.o: $(SRCDIR)/cuesheet.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cuesheet.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/cuesheet.cpp -o $(OBJECTDIR)/cuesheet.o
	$(ECHO) done.

$(OBJECTDIR)/debug.o: $(SRCDIR)/debug.cpp
	$(ECHO) -n Compiling $(SRCDIR)/debug.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/debug.cpp -o $(OBJECTDIR)/debug.o
	$(ECHO) done.

$(OBJECTDIR)/directories.o: $(SRCDIR)/directories.cpp
	$(ECHO) -n Compiling $(SRCDIR)/directories.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/directories.cpp -o $(OBJECTDIR)/directories.o
	$(ECHO) done.

$(OBJECTDIR)/dllinterfaces.o: $(SRCDIR)/dllinterfaces.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dllinterfaces.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/dllinterfaces.cpp -o $(OBJECTDIR)/dllinterfaces.o
	$(ECHO) done.

$(OBJECTDIR)/encoder.o: $(SRCDIR)/encoder.cpp
	$(ECHO) -n Compiling $(SRCDIR)/encoder.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/encoder.cpp -o $(OBJECTDIR)/encoder.o
	$(ECHO) done.

$(OBJECTDIR)/joblist.o: $(SRCDIR)/joblist.cpp
	$(ECHO) -n Compiling $(SRCDIR)/joblist.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/joblist.cpp -o $(OBJECTDIR)/joblist.o
	$(ECHO) done.

$(OBJECTDIR)/adddirectory.o: $(SRCDIR)/dialogs/adddirectory.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/adddirectory.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/adddirectory.cpp -o $(OBJECTDIR)/adddirectory.o
	$(ECHO) done.

$(OBJECTDIR)/addpattern.o: $(SRCDIR)/dialogs/addpattern.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/addpattern.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/addpattern.cpp -o $(OBJECTDIR)/addpattern.o
	$(ECHO) done.

$(OBJECTDIR)/donate.o: $(SRCDIR)/dialogs/donate.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/donate.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/donate.cpp -o $(OBJECTDIR)/donate.o
	$(ECHO) done.

$(OBJECTDIR)/main.o: $(SRCDIR)/main.cpp
	$(ECHO) -n Compiling $(SRCDIR)/main.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/main.cpp -o $(OBJECTDIR)/main.o
	$(ECHO) done.

$(OBJECTDIR)/picture.o: $(SRCDIR)/picture.cpp
	$(ECHO) -n Compiling $(SRCDIR)/picture.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/picture.cpp -o $(OBJECTDIR)/picture.o
	$(ECHO) done.

$(OBJECTDIR)/playback.o: $(SRCDIR)/playback.cpp
	$(ECHO) -n Compiling $(SRCDIR)/playback.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/playback.cpp -o $(OBJECTDIR)/playback.o
	$(ECHO) done.

$(OBJECTDIR)/playlist.o: $(SRCDIR)/playlist.cpp
	$(ECHO) -n Compiling $(SRCDIR)/playlist.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/playlist.cpp -o $(OBJECTDIR)/playlist.o
	$(ECHO) done.

$(OBJECTDIR)/track.o: $(SRCDIR)/track.cpp
	$(ECHO) -n Compiling $(SRCDIR)/track.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/track.cpp -o $(OBJECTDIR)/track.o
	$(ECHO) done.

$(OBJECTDIR)/utilities.o: $(SRCDIR)/utilities.cpp
	$(ECHO) -n Compiling $(SRCDIR)/utilities.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/utilities.cpp -o $(OBJECTDIR)/utilities.o
	$(ECHO) done.

$(OBJECTDIR)/genconfig.o: $(SRCDIR)/dialogs/genconfig/genconfig.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/genconfig/genconfig.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/genconfig/genconfig.cpp -o $(OBJECTDIR)/genconfig.o
	$(ECHO) done.

$(OBJECTDIR)/genconfig_cddb.o: $(SRCDIR)/dialogs/genconfig/genconfig_cddb.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/genconfig/genconfig_cddb.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/genconfig/genconfig_cddb.cpp -o $(OBJECTDIR)/genconfig_cddb.o
	$(ECHO) done.

$(OBJECTDIR)/genconfig_cdrip.o: $(SRCDIR)/dialogs/genconfig/genconfig_cdrip.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/genconfig/genconfig_cdrip.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/genconfig/genconfig_cdrip.cpp -o $(OBJECTDIR)/genconfig_cdrip.o
	$(ECHO) done.

$(OBJECTDIR)/genconfig_encoders.o: $(SRCDIR)/dialogs/genconfig/genconfig_encoders.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/genconfig/genconfig_encoders.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/genconfig/genconfig_encoders.cpp -o $(OBJECTDIR)/genconfig_encoders.o
	$(ECHO) done.

$(OBJECTDIR)/genconfig_language.o: $(SRCDIR)/dialogs/genconfig/genconfig_language.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/genconfig/genconfig_language.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/genconfig/genconfig_language.cpp -o $(OBJECTDIR)/genconfig_language.o
	$(ECHO) done.

$(OBJECTDIR)/genconfig_playlists.o: $(SRCDIR)/dialogs/genconfig/genconfig_playlists.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/genconfig/genconfig_playlists.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/genconfig/genconfig_playlists.cpp -o $(OBJECTDIR)/genconfig_playlists.o
	$(ECHO) done.

$(OBJECTDIR)/genconfig_plugins.o: $(SRCDIR)/dialogs/genconfig/genconfig_plugins.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/genconfig/genconfig_plugins.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/genconfig/genconfig_plugins.cpp -o $(OBJECTDIR)/genconfig_plugins.o
	$(ECHO) done.

$(OBJECTDIR)/genconfig_tags.o: $(SRCDIR)/dialogs/genconfig/genconfig_tags.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/genconfig/genconfig_tags.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllimport)" $(SRCDIR)/dialogs/genconfig/genconfig_tags.cpp -o $(OBJECTDIR)/genconfig_tags.o
	$(ECHO) done.

$(OBJECTDIR)/filter-in-aiff.o: $(SRCDIR)/input/filter-in-aiff.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-aiff.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/input/filter-in-aiff.cpp -o $(OBJECTDIR)/filter-in-aiff.o
	$(ECHO) done.

$(OBJECTDIR)/filter-in-au.o: $(SRCDIR)/input/filter-in-au.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-au.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/input/filter-in-au.cpp -o $(OBJECTDIR)/filter-in-au.o
	$(ECHO) done.

$(OBJECTDIR)/filter-in-bonk.o: $(SRCDIR)/input/filter-in-bonk.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-bonk.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/input/filter-in-bonk.cpp -o $(OBJECTDIR)/filter-in-bonk.o
	$(ECHO) done.

$(OBJECTDIR)/filter-in-cdrip.o: $(SRCDIR)/input/filter-in-cdrip.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-cdrip.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/input/filter-in-cdrip.cpp -o $(OBJECTDIR)/filter-in-cdrip.o
	$(ECHO) done.

$(OBJECTDIR)/filter-in-faad2.o: $(SRCDIR)/input/filter-in-faad2.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-faad2.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/input/filter-in-faad2.cpp -o $(OBJECTDIR)/filter-in-faad2.o
	$(ECHO) done.

$(OBJECTDIR)/filter-in-flac.o: $(SRCDIR)/input/filter-in-flac.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-flac.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/input/filter-in-flac.cpp -o $(OBJECTDIR)/filter-in-flac.o
	$(ECHO) done.

$(OBJECTDIR)/filter-in-mad.o: $(SRCDIR)/input/filter-in-mad.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-mad.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/input/filter-in-mad.cpp -o $(OBJECTDIR)/filter-in-mad.o
	$(ECHO) done.

$(OBJECTDIR)/filter-in-mp4.o: $(SRCDIR)/input/filter-in-mp4.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-mp4.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/input/filter-in-mp4.cpp -o $(OBJECTDIR)/filter-in-mp4.o
	$(ECHO) done.

$(OBJECTDIR)/filter-in-voc.o: $(SRCDIR)/input/filter-in-voc.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-voc.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/input/filter-in-voc.cpp -o $(OBJECTDIR)/filter-in-voc.o
	$(ECHO) done.

$(OBJECTDIR)/filter-in-vorbis.o: $(SRCDIR)/input/filter-in-vorbis.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-vorbis.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/input/filter-in-vorbis.cpp -o $(OBJECTDIR)/filter-in-vorbis.o
	$(ECHO) done.

$(OBJECTDIR)/filter-in-wave.o: $(SRCDIR)/input/filter-in-wave.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-wave.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/input/filter-in-wave.cpp -o $(OBJECTDIR)/filter-in-wave.o
	$(ECHO) done.

$(OBJECTDIR)/filter-in-winamp.o: $(SRCDIR)/input/filter-in-winamp.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-winamp.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/input/filter-in-winamp.cpp -o $(OBJECTDIR)/filter-in-winamp.o
	$(ECHO) done.

$(OBJECTDIR)/filter-in-wma.o: $(SRCDIR)/input/filter-in-wma.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/filter-in-wma.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/input/filter-in-wma.cpp -o $(OBJECTDIR)/filter-in-wma.o
	$(ECHO) done.

$(OBJECTDIR)/inputfilter.o: $(SRCDIR)/input/inputfilter.cpp
	$(ECHO) -n Compiling $(SRCDIR)/input/inputfilter.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/input/inputfilter.cpp -o $(OBJECTDIR)/inputfilter.o
	$(ECHO) done.

$(OBJECTDIR)/filter-out-blade.o: $(SRCDIR)/output/filter-out-blade.cpp
	$(ECHO) -n Compiling $(SRCDIR)/output/filter-out-blade.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/output/filter-out-blade.cpp -o $(OBJECTDIR)/filter-out-blade.o
	$(ECHO) done.

$(OBJECTDIR)/filter-out-bonk.o: $(SRCDIR)/output/filter-out-bonk.cpp
	$(ECHO) -n Compiling $(SRCDIR)/output/filter-out-bonk.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/output/filter-out-bonk.cpp -o $(OBJECTDIR)/filter-out-bonk.o
	$(ECHO) done.

$(OBJECTDIR)/filter-out-faac.o: $(SRCDIR)/output/filter-out-faac.cpp
	$(ECHO) -n Compiling $(SRCDIR)/output/filter-out-faac.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/output/filter-out-faac.cpp -o $(OBJECTDIR)/filter-out-faac.o
	$(ECHO) done.

$(OBJECTDIR)/filter-out-flac.o: $(SRCDIR)/output/filter-out-flac.cpp
	$(ECHO) -n Compiling $(SRCDIR)/output/filter-out-flac.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/output/filter-out-flac.cpp -o $(OBJECTDIR)/filter-out-flac.o
	$(ECHO) done.

$(OBJECTDIR)/filter-out-lame.o: $(SRCDIR)/output/filter-out-lame.cpp
	$(ECHO) -n Compiling $(SRCDIR)/output/filter-out-lame.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/output/filter-out-lame.cpp -o $(OBJECTDIR)/filter-out-lame.o
	$(ECHO) done.

$(OBJECTDIR)/filter-out-mp4.o: $(SRCDIR)/output/filter-out-mp4.cpp
	$(ECHO) -n Compiling $(SRCDIR)/output/filter-out-mp4.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/output/filter-out-mp4.cpp -o $(OBJECTDIR)/filter-out-mp4.o
	$(ECHO) done.

$(OBJECTDIR)/filter-out-tvq.o: $(SRCDIR)/output/filter-out-tvq.cpp
	$(ECHO) -n Compiling $(SRCDIR)/output/filter-out-tvq.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/output/filter-out-tvq.cpp -o $(OBJECTDIR)/filter-out-tvq.o
	$(ECHO) done.

$(OBJECTDIR)/filter-out-vorbis.o: $(SRCDIR)/output/filter-out-vorbis.cpp
	$(ECHO) -n Compiling $(SRCDIR)/output/filter-out-vorbis.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/output/filter-out-vorbis.cpp -o $(OBJECTDIR)/filter-out-vorbis.o
	$(ECHO) done.

$(OBJECTDIR)/filter-out-wave.o: $(SRCDIR)/output/filter-out-wave.cpp
	$(ECHO) -n Compiling $(SRCDIR)/output/filter-out-wave.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/output/filter-out-wave.cpp -o $(OBJECTDIR)/filter-out-wave.o
	$(ECHO) done.

$(OBJECTDIR)/filter-out-wma.o: $(SRCDIR)/output/filter-out-wma.cpp
	$(ECHO) -n Compiling $(SRCDIR)/output/filter-out-wma.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/output/filter-out-wma.cpp -o $(OBJECTDIR)/filter-out-wma.o
	$(ECHO) done.

$(OBJECTDIR)/outputfilter.o: $(SRCDIR)/output/outputfilter.cpp
	$(ECHO) -n Compiling $(SRCDIR)/output/outputfilter.cpp...
	$(COMPILER) $(COMPILER_OPTS) -DBEEXPORT="__declspec (dllexport)" $(SRCDIR)/output/outputfilter.cpp -o $(OBJECTDIR)/outputfilter.o
	$(ECHO) done.

$(OBJECTDIR)/resources.o: $(RESOURCEDIR)/resources.rc $(INCLUDEDIR1)/resources.h $(BINRESDIR)/freac.ico
	$(ECHO) -n Compiling $(RESOURCEDIR)/resources.rc...
	$(RESCOMP) $(RESCOMP_OPTS) $(RESOURCEDIR)/resources.rc -o $(OBJECTDIR)/resources.o
	$(ECHO) done.
