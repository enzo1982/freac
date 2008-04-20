# Makefile for BonkEnc

INCLUDEDIR1 = ./include
INCLUDEDIR2 = ../smooth/include
INCLUDEDIR3 = cdk/include
OBJECTDIR = ./objects
BINDIR = ./bin
SRCDIR = ./src
LIBDIR1 = ../smooth/lib
LIBDIR2 = cdk/lib
RESOURCEDIR = ./resources
BINRESDIR = $(RESOURCEDIR)/binary

DLLOBJECTS = $(OBJECTDIR)/cddb.o $(OBJECTDIR)/cddbbatch.o $(OBJECTDIR)/cddbcache.o $(OBJECTDIR)/cddbinfo.o $(OBJECTDIR)/cddblocal.o $(OBJECTDIR)/cddbremote.o $(OBJECTDIR)/cdplayerini.o $(OBJECTDIR)/cdtext.o $(OBJECTDIR)/cddb_extsettings.o $(OBJECTDIR)/cddb_manage.o $(OBJECTDIR)/cddb_managequeries.o $(OBJECTDIR)/cddb_managesubmits.o $(OBJECTDIR)/cddb_multimatch.o $(OBJECTDIR)/cddb_query.o $(OBJECTDIR)/cddb_submit.o $(OBJECTDIR)/dialog_config.o $(OBJECTDIR)/config_cddb.o $(OBJECTDIR)/config_cdrip.o $(OBJECTDIR)/config_encoders.o $(OBJECTDIR)/config_language.o $(OBJECTDIR)/config_playlists.o $(OBJECTDIR)/config_tags.o $(OBJECTDIR)/configcomponent.o $(OBJECTDIR)/configentry.o $(OBJECTDIR)/adddirectory.o $(OBJECTDIR)/addpattern.o $(OBJECTDIR)/language.o $(OBJECTDIR)/main_joblist.o $(OBJECTDIR)/main_protocol.o $(OBJECTDIR)/main_threads.o $(OBJECTDIR)/job.o $(OBJECTDIR)/bonkenc.o $(OBJECTDIR)/config.o $(OBJECTDIR)/cuesheet.o $(OBJECTDIR)/dllinterfaces.o $(OBJECTDIR)/encoder.o $(OBJECTDIR)/joblist.o $(OBJECTDIR)/playback.o $(OBJECTDIR)/playlist.o $(OBJECTDIR)/progress.o $(OBJECTDIR)/startconsole.o $(OBJECTDIR)/startgui.o $(OBJECTDIR)/utilities.o
RESOURCES = $(OBJECTDIR)/resources.o
EXEOBJECTS = $(OBJECTDIR)/gui.o
CMDOBJECTS = $(OBJECTDIR)/console.o

EXENAME = $(BINDIR)/BonkEnc.exe
CMDNAME = $(BINDIR)/BEcmd.exe
DLLNAME = $(BINDIR)/BonkEnc.dll
LIBNAME = $(OBJECTDIR)/libbonkenc.a

COMPILER = gcc
RESCOMP = windres
LINKER = gcc
REMOVER = rm
ECHO = echo
COMPILER_OPTS = -I$(INCLUDEDIR1) -I$(INCLUDEDIR2) -I$(INCLUDEDIR3) -march=i586 -Os -g0 -Wall -fno-exceptions -DUNICODE -D_UNICODE -c
LINKER_OPTS = -L$(LIBDIR1) -L$(LIBDIR2) -lboca -lsmooth -llisa -lunicows -lshell32 -lws2_32 -lole32 -lstdc++ -mwindows --shared -Xlinker --out-implib -Xlinker $(LIBNAME) -o$(DLLNAME)
LOADER_COMPILER_OPTS = -I$(INCLUDEDIR2) -march=i586 -Os -g0 -Wall -fno-exceptions -c
LOADER_GUI_LINKER_OPTS = -L$(LIBDIR1) -lsmooth -lstdc++ -mwindows -o$(EXENAME)
LOADER_CONSOLE_LINKER_OPTS = -L$(LIBDIR1) -lsmooth -lstdc++ -o$(CMDNAME)
REMOVER_OPTS = -f
STRIP = strip
STRIP_OPTS = --strip-all
PACKER = upx
PACKER_OPTS = -9 -q
RESCOMP_OPTS = -O coff

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
	$(LINKER) $(DLLOBJECTS) $(LINKER_OPTS)
	$(STRIP) $(STRIP_OPTS) $(DLLNAME)
	$(PACKER) $(PACKER_OPTS) $(DLLNAME)
	countbuild BuildNumber
	$(ECHO) done.

$(EXENAME): $(EXEOBJECTS) $(RESOURCES)
	$(ECHO) -n Linking $(EXENAME)...
	$(LINKER) $(EXEOBJECTS) $(RESOURCES) $(LOADER_GUI_LINKER_OPTS)
	$(STRIP) $(STRIP_OPTS) $(EXENAME)
	$(PACKER) $(PACKER_OPTS) $(EXENAME)
	$(ECHO) done.

$(CMDNAME): $(CMDOBJECTS) $(RESOURCES)
	$(ECHO) -n Linking $(CMDNAME)...
	$(LINKER) $(CMDOBJECTS) $(RESOURCES) $(LOADER_CONSOLE_LINKER_OPTS)
	$(STRIP) $(STRIP_OPTS) $(CMDNAME)
	$(PACKER) $(PACKER_OPTS) $(CMDNAME)
	$(ECHO) done.

$(OBJECTDIR)/cddb.o: $(SRCDIR)/cddb/cddb.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cddb/cddb.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/cddb/cddb.cpp -o $(OBJECTDIR)/cddb.o
	$(ECHO) done.

$(OBJECTDIR)/cddbbatch.o: $(SRCDIR)/cddb/cddbbatch.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cddb/cddbbatch.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/cddb/cddbbatch.cpp -o $(OBJECTDIR)/cddbbatch.o
	$(ECHO) done.

$(OBJECTDIR)/cddbcache.o: $(SRCDIR)/cddb/cddbcache.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cddb/cddbcache.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/cddb/cddbcache.cpp -o $(OBJECTDIR)/cddbcache.o
	$(ECHO) done.

$(OBJECTDIR)/cddbinfo.o: $(SRCDIR)/cddb/cddbinfo.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cddb/cddbinfo.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/cddb/cddbinfo.cpp -o $(OBJECTDIR)/cddbinfo.o
	$(ECHO) done.

$(OBJECTDIR)/cddblocal.o: $(SRCDIR)/cddb/cddblocal.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cddb/cddblocal.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/cddb/cddblocal.cpp -o $(OBJECTDIR)/cddblocal.o
	$(ECHO) done.

$(OBJECTDIR)/cddbremote.o: $(SRCDIR)/cddb/cddbremote.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cddb/cddbremote.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/cddb/cddbremote.cpp -o $(OBJECTDIR)/cddbremote.o
	$(ECHO) done.

$(OBJECTDIR)/cdplayerini.o: $(SRCDIR)/cdinfo/cdplayerini.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cdinfo/cdplayerini.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/cdinfo/cdplayerini.cpp -o $(OBJECTDIR)/cdplayerini.o
	$(ECHO) done.

$(OBJECTDIR)/cdtext.o: $(SRCDIR)/cdinfo/cdtext.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cdinfo/cdtext.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/cdinfo/cdtext.cpp -o $(OBJECTDIR)/cdtext.o
	$(ECHO) done.

$(OBJECTDIR)/cddb_extsettings.o: $(SRCDIR)/dialogs/cddb/extsettings.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/cddb/extsettings.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/cddb/extsettings.cpp -o $(OBJECTDIR)/cddb_extsettings.o
	$(ECHO) done.

$(OBJECTDIR)/cddb_manage.o: $(SRCDIR)/dialogs/cddb/manage.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/cddb/manage.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/cddb/manage.cpp -o $(OBJECTDIR)/cddb_manage.o
	$(ECHO) done.

$(OBJECTDIR)/cddb_managequeries.o: $(SRCDIR)/dialogs/cddb/managequeries.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/cddb/managequeries.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/cddb/managequeries.cpp -o $(OBJECTDIR)/cddb_managequeries.o
	$(ECHO) done.

$(OBJECTDIR)/cddb_managesubmits.o: $(SRCDIR)/dialogs/cddb/managesubmits.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/cddb/managesubmits.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/cddb/managesubmits.cpp -o $(OBJECTDIR)/cddb_managesubmits.o
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

$(OBJECTDIR)/job.o: $(SRCDIR)/jobs/job.cpp
	$(ECHO) -n Compiling $(SRCDIR)/jobs/job.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/jobs/job.cpp -o $(OBJECTDIR)/job.o
	$(ECHO) done.

$(OBJECTDIR)/bonkenc.o: $(SRCDIR)/bonkenc.cpp
	$(ECHO) -n Compiling $(SRCDIR)/bonkenc.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/bonkenc.cpp -o $(OBJECTDIR)/bonkenc.o
	$(ECHO) done.

$(OBJECTDIR)/config.o: $(SRCDIR)/config.cpp
	$(ECHO) -n Compiling $(SRCDIR)/config.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/config.cpp -o $(OBJECTDIR)/config.o
	$(ECHO) done.

$(OBJECTDIR)/cuesheet.o: $(SRCDIR)/cuesheet.cpp
	$(ECHO) -n Compiling $(SRCDIR)/cuesheet.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/cuesheet.cpp -o $(OBJECTDIR)/cuesheet.o
	$(ECHO) done.

$(OBJECTDIR)/dllinterfaces.o: $(SRCDIR)/dllinterfaces.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dllinterfaces.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dllinterfaces.cpp -o $(OBJECTDIR)/dllinterfaces.o
	$(ECHO) done.

$(OBJECTDIR)/encoder.o: $(SRCDIR)/encoder.cpp
	$(ECHO) -n Compiling $(SRCDIR)/encoder.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/encoder.cpp -o $(OBJECTDIR)/encoder.o
	$(ECHO) done.

$(OBJECTDIR)/joblist.o: $(SRCDIR)/joblist.cpp
	$(ECHO) -n Compiling $(SRCDIR)/joblist.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/joblist.cpp -o $(OBJECTDIR)/joblist.o
	$(ECHO) done.

$(OBJECTDIR)/adddirectory.o: $(SRCDIR)/dialogs/adddirectory.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/adddirectory.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/adddirectory.cpp -o $(OBJECTDIR)/adddirectory.o
	$(ECHO) done.

$(OBJECTDIR)/addpattern.o: $(SRCDIR)/dialogs/addpattern.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/addpattern.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/addpattern.cpp -o $(OBJECTDIR)/addpattern.o
	$(ECHO) done.

$(OBJECTDIR)/language.o: $(SRCDIR)/dialogs/language.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/language.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/language.cpp -o $(OBJECTDIR)/language.o
	$(ECHO) done.

$(OBJECTDIR)/playback.o: $(SRCDIR)/playback.cpp
	$(ECHO) -n Compiling $(SRCDIR)/playback.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/playback.cpp -o $(OBJECTDIR)/playback.o
	$(ECHO) done.

$(OBJECTDIR)/playlist.o: $(SRCDIR)/playlist.cpp
	$(ECHO) -n Compiling $(SRCDIR)/playlist.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/playlist.cpp -o $(OBJECTDIR)/playlist.o
	$(ECHO) done.

$(OBJECTDIR)/progress.o: $(SRCDIR)/progress.cpp
	$(ECHO) -n Compiling $(SRCDIR)/progress.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/progress.cpp -o $(OBJECTDIR)/progress.o
	$(ECHO) done.

$(OBJECTDIR)/startconsole.o: $(SRCDIR)/startconsole.cpp
	$(ECHO) -n Compiling $(SRCDIR)/startconsole.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/startconsole.cpp -o $(OBJECTDIR)/startconsole.o
	$(ECHO) done.

$(OBJECTDIR)/startgui.o: $(SRCDIR)/startgui.cpp
	$(ECHO) -n Compiling $(SRCDIR)/startgui.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/startgui.cpp -o $(OBJECTDIR)/startgui.o
	$(ECHO) done.

$(OBJECTDIR)/utilities.o: $(SRCDIR)/utilities.cpp
	$(ECHO) -n Compiling $(SRCDIR)/utilities.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/utilities.cpp -o $(OBJECTDIR)/utilities.o
	$(ECHO) done.

$(OBJECTDIR)/dialog_config.o: $(SRCDIR)/dialogs/config/config.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/config/config.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/config/config.cpp -o $(OBJECTDIR)/dialog_config.o
	$(ECHO) done.

$(OBJECTDIR)/config_cddb.o: $(SRCDIR)/dialogs/config/config_cddb.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/config/config_cddb.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/config/config_cddb.cpp -o $(OBJECTDIR)/config_cddb.o
	$(ECHO) done.

$(OBJECTDIR)/config_cdrip.o: $(SRCDIR)/dialogs/config/config_cdrip.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/config/config_cdrip.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/config/config_cdrip.cpp -o $(OBJECTDIR)/config_cdrip.o
	$(ECHO) done.

$(OBJECTDIR)/config_encoders.o: $(SRCDIR)/dialogs/config/config_encoders.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/config/config_encoders.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/config/config_encoders.cpp -o $(OBJECTDIR)/config_encoders.o
	$(ECHO) done.

$(OBJECTDIR)/config_language.o: $(SRCDIR)/dialogs/config/config_language.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/config/config_language.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/config/config_language.cpp -o $(OBJECTDIR)/config_language.o
	$(ECHO) done.

$(OBJECTDIR)/config_playlists.o: $(SRCDIR)/dialogs/config/config_playlists.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/config/config_playlists.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/config/config_playlists.cpp -o $(OBJECTDIR)/config_playlists.o
	$(ECHO) done.

$(OBJECTDIR)/config_tags.o: $(SRCDIR)/dialogs/config/config_tags.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/config/config_tags.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/config/config_tags.cpp -o $(OBJECTDIR)/config_tags.o
	$(ECHO) done.

$(OBJECTDIR)/configcomponent.o: $(SRCDIR)/dialogs/config/configcomponent.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/config/configcomponent.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/config/configcomponent.cpp -o $(OBJECTDIR)/configcomponent.o
	$(ECHO) done.

$(OBJECTDIR)/configentry.o: $(SRCDIR)/dialogs/config/configentry.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/config/configentry.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/config/configentry.cpp -o $(OBJECTDIR)/configentry.o
	$(ECHO) done.

$(OBJECTDIR)/main_joblist.o: $(SRCDIR)/gui/main_joblist.cpp
	$(ECHO) -n Compiling $(SRCDIR)/gui/main_joblist.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/gui/main_joblist.cpp -o $(OBJECTDIR)/main_joblist.o
	$(ECHO) done.

$(OBJECTDIR)/main_protocol.o: $(SRCDIR)/gui/main_protocol.cpp
	$(ECHO) -n Compiling $(SRCDIR)/gui/main_protocol.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/gui/main_protocol.cpp -o $(OBJECTDIR)/main_protocol.o
	$(ECHO) done.

$(OBJECTDIR)/main_threads.o: $(SRCDIR)/gui/main_threads.cpp
	$(ECHO) -n Compiling $(SRCDIR)/gui/main_threads.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/gui/main_threads.cpp -o $(OBJECTDIR)/main_threads.o
	$(ECHO) done.

$(OBJECTDIR)/console.o: $(SRCDIR)/loader/console.cpp
	$(ECHO) -n Compiling $(SRCDIR)/loader/console.cpp...
	$(COMPILER) $(LOADER_COMPILER_OPTS) $(SRCDIR)/loader/console.cpp -o $(OBJECTDIR)/console.o
	$(ECHO) done.

$(OBJECTDIR)/gui.o: $(SRCDIR)/loader/gui.cpp
	$(ECHO) -n Compiling $(SRCDIR)/loader/gui.cpp...
	$(COMPILER) $(LOADER_COMPILER_OPTS) $(SRCDIR)/loader/gui.cpp -o $(OBJECTDIR)/gui.o
	$(ECHO) done.

$(OBJECTDIR)/resources.o: $(RESOURCEDIR)/resources.rc $(INCLUDEDIR1)/resources.h $(BINRESDIR)/bonkenc.ico
	$(ECHO) -n Compiling $(RESOURCEDIR)/resources.rc...
	$(RESCOMP) $(RESCOMP_OPTS) $(RESOURCEDIR)/resources.rc -o $(OBJECTDIR)/resources.o
	$(ECHO) done.
