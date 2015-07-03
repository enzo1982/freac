# Makefile for fre:ac

include Makefile-options

CDK	    = ./cdk

INCLUDEDIR  = ./include
OBJECTDIR   = ./objects
SRCDIR	    = ./src

BINDIR	    = ./bin
LIBDIR	    = $(CDK)/lib

ifeq ($(BUILD_X86_64),True)
	BINDIR = ./bin64
	LIBDIR = $(CDK)/lib64
else ifeq ($(BUILD_PPC64),True)
	BINDIR = ./bin64
	LIBDIR = $(CDK)/lib64
endif

RESOURCEDIR = ./resources
BINRESDIR   = $(RESOURCEDIR)/binary

DLLOBJECTS  = $(OBJECTDIR)/cddb.o $(OBJECTDIR)/cddbbatch.o $(OBJECTDIR)/cddbcache.o $(OBJECTDIR)/cddbinfo.o $(OBJECTDIR)/cddblocal.o $(OBJECTDIR)/cddbremote.o $(OBJECTDIR)/cddb_extsettings.o $(OBJECTDIR)/cddb_manage.o $(OBJECTDIR)/cddb_managequeries.o $(OBJECTDIR)/cddb_managesubmits.o $(OBJECTDIR)/cddb_multimatch.o $(OBJECTDIR)/cddb_query.o $(OBJECTDIR)/cddb_submit.o $(OBJECTDIR)/dialog_config.o $(OBJECTDIR)/config_cddb.o $(OBJECTDIR)/config_encoders.o $(OBJECTDIR)/config_interface.o $(OBJECTDIR)/config_language.o $(OBJECTDIR)/config_playlists.o $(OBJECTDIR)/config_resources.o $(OBJECTDIR)/config_tags.o $(OBJECTDIR)/configcomponent.o $(OBJECTDIR)/configentry.o $(OBJECTDIR)/adddirectory.o $(OBJECTDIR)/addpattern.o $(OBJECTDIR)/charset.o $(OBJECTDIR)/error.o $(OBJECTDIR)/overwrite.o $(OBJECTDIR)/engine_converter.o $(OBJECTDIR)/engine_decoder.o $(OBJECTDIR)/engine_encoder.o $(OBJECTDIR)/engine_worker.o $(OBJECTDIR)/engine_worker_singlefile.o $(OBJECTDIR)/layer_tooltip.o $(OBJECTDIR)/main_joblist.o $(OBJECTDIR)/main_threads.o $(OBJECTDIR)/player.o $(OBJECTDIR)/job_convert.o $(OBJECTDIR)/job_adddirectory.o $(OBJECTDIR)/job_addfiles.o $(OBJECTDIR)/job_addtracks.o $(OBJECTDIR)/job_removeall.o $(OBJECTDIR)/job_removedisc.o $(OBJECTDIR)/job_checkforupdates.o $(OBJECTDIR)/job.o $(OBJECTDIR)/jobmanager.o $(OBJECTDIR)/tools_encoding.o $(OBJECTDIR)/bonkenc.o $(OBJECTDIR)/config.o $(OBJECTDIR)/dllinterfaces.o $(OBJECTDIR)/joblist.o $(OBJECTDIR)/playback.o $(OBJECTDIR)/progress.o $(OBJECTDIR)/startconsole.o $(OBJECTDIR)/startgui.o $(OBJECTDIR)/utilities.o

ifeq ($(BUILD_WIN32),True)
	DLLOBJECTS += $(OBJECTDIR)/autorelease_none.o
	DLLOBJECTS += $(OBJECTDIR)/notification_win32.o
else ifeq ($(BUILD_LINUX),True)
	DLLOBJECTS += $(OBJECTDIR)/autorelease_none.o
	DLLOBJECTS += $(OBJECTDIR)/notification_udev.o
else ifeq ($(BUILD_OSX),True)
	DLLOBJECTS += $(OBJECTDIR)/autorelease_osx.o
	DLLOBJECTS += $(OBJECTDIR)/notification_osx.o
else
	DLLOBJECTS += $(OBJECTDIR)/autorelease_none.o
	DLLOBJECTS += $(OBJECTDIR)/notification_none.o
endif

ifeq ($(BUILD_WIN32),True)
	ifeq ($(BUILD_VIDEO_DOWNLOADER),True)
		RESOURCES = $(OBJECTDIR)/resources_vd.o
	else
		RESOURCES = $(OBJECTDIR)/resources.o
	endif
endif

EXEOBJECTS  = $(OBJECTDIR)/gui.o
CMDOBJECTS  = $(OBJECTDIR)/console.o

EXENAME	    = $(BINDIR)/freac$(EXECUTABLE)
CMDNAME	    = $(BINDIR)/freaccmd$(EXECUTABLE)
DLLNAME	    = $(BINDIR)/freac$(SHARED)
LIBNAME	    = $(OBJECTDIR)/libfreac.a

COMPILER		   = gcc
RESCOMP			   = windres
LINKER			   = gcc
REMOVER			   = rm
ECHO			   = echo
COMPILER_OPTS		   = -pipe -I$(INCLUDEDIR) -g0 -Wall -Os -fvisibility=hidden -fno-exceptions -c
RESCOMP_OPTS		   = -O coff
LINKER_OPTS		   = -pipe -lstdc++ -o $(DLLNAME)
LOADER_GUI_LINKER_OPTS	   = -pipe -lstdc++ -o $(EXENAME)
LOADER_CONSOLE_LINKER_OPTS = -pipe -lstdc++ -o $(CMDNAME)
REMOVER_OPTS		   = -f

ifeq ($(BUILD_FREEBSD),True)
	COMPILER = clang
	LINKER	 = clang
endif

ifeq ($(BUILD_VIDEO_DOWNLOADER),True)
	COMPILER_OPTS			+= -D BUILD_VIDEO_DOWNLOADER
endif

ifneq ($(BUILD_WIN32),True)
ifneq ($(BUILD_SOLARIS),True)
ifneq ($(BUILD_QNX),True)
ifneq ($(BUILD_HAIKU),True)
	COMPILER_OPTS			+= -pthread
endif
endif
endif
endif

ifeq ($(BUILD_OSX),True)
	LINKER_OPTS			+= -Wl,-x -dynamiclib
	LOADER_GUI_LINKER_OPTS		+= -Wl,-x
	LOADER_CONSOLE_LINKER_OPTS	+= -Wl,-x

ifeq ($(BUILD_X86),True)
	COMPILER_OPTS			+= -arch i386 -Xarch_i386 -march=prescott

	LINKER_OPTS			+= -arch i386
	LOADER_GUI_LINKER_OPTS		+= -arch i386
	LOADER_CONSOLE_LINKER_OPTS	+= -arch i386
endif
ifeq ($(BUILD_X86_64),True)
	COMPILER_OPTS			+= -arch x86_64 -Xarch_x86_64 -march=core2

	LINKER_OPTS			+= -arch x86_64
	LOADER_GUI_LINKER_OPTS		+= -arch x86_64
	LOADER_CONSOLE_LINKER_OPTS	+= -arch x86_64
endif
ifeq ($(BUILD_PPC),True)
	COMPILER_OPTS			+= -arch ppc -Xarch_ppc -mcpu=G4

	LINKER_OPTS			+= -arch ppc
	LOADER_GUI_LINKER_OPTS		+= -arch ppc
	LOADER_CONSOLE_LINKER_OPTS	+= -arch ppc
endif
ifeq ($(BUILD_PPC64),True)
	COMPILER_OPTS			+= -arch ppc64 -Xarch_ppc64 -mcpu=G5

	LINKER_OPTS			+= -arch ppc64
	LOADER_GUI_LINKER_OPTS		+= -arch ppc64
	LOADER_CONSOLE_LINKER_OPTS	+= -arch ppc64
endif
else
	LINKER_OPTS			+= -s --shared
	LOADER_GUI_LINKER_OPTS		+= -s
	LOADER_CONSOLE_LINKER_OPTS	+= -s

ifeq ($(BUILD_X86),True)
	COMPILER_OPTS			+= -m32
	RESCOMP_OPTS			+= --target=pe-i386

	LINKER_OPTS			+= -m32
	LOADER_GUI_LINKER_OPTS		+= -m32
	LOADER_CONSOLE_LINKER_OPTS	+= -m32
else ifeq ($(BUILD_X86_64),True)
	COMPILER_OPTS			+= -m64
	RESCOMP_OPTS			+= --target=pe-x86-64

	LINKER_OPTS			+= -m64
	LOADER_GUI_LINKER_OPTS		+= -m64
	LOADER_CONSOLE_LINKER_OPTS	+= -m64
endif
endif

ifeq ($(BUILD_OPENBSD),True)
	LOADER_GUI_LINKER_OPTS		+= -L/usr/X11R6/lib -L/usr/local/lib -logg -lvorbis -lvorbisfile
	LOADER_CONSOLE_LINKER_OPTS	+= -L/usr/X11R6/lib -L/usr/local/lib -logg -lvorbis -lvorbisfile
endif

ifneq ($(BUILD_WIN32),True)
	COMPILER_OPTS			+= -fPIC
endif

ifeq ($(BUILD_WIN32),True)
	COMPILER_OPTS			+= -DUNICODE -D_UNICODE -I$(CDK)/include

	LINKER_OPTS			+= -L$(LIBDIR) -lsmooth -lboca -Wl,--dynamicbase,--nxcompat -lws2_32 -Wl,--out-implib,$(LIBNAME)
	LOADER_GUI_LINKER_OPTS		+= -L$(LIBDIR) -lsmooth -Wl,--dynamicbase,--nxcompat -mwindows
	LOADER_CONSOLE_LINKER_OPTS	+= -L$(LIBDIR) -lsmooth -Wl,--dynamicbase,--nxcompat
else
	COMPILER_OPTS			+= -I$(PREFIX)/include

	ifeq ($(BUILD_OSX),True)
		LINKER_OPTS		+= -framework Cocoa -framework IOKit -Wl,-dylib_install_name,freac$(SHARED) -Wl,-headerpad,80
	endif

	LINKER_OPTS			+= -L$(PREFIX)/lib -lsmooth-$(SMOOTHVER) -lboca-$(BOCAVER) -Wl,-rpath,.
	LOADER_GUI_LINKER_OPTS		+= -L$(PREFIX)/lib -lsmooth-$(SMOOTHVER) -Wl,-rpath,.
	LOADER_CONSOLE_LINKER_OPTS	+= -L$(PREFIX)/lib -lsmooth-$(SMOOTHVER) -Wl,-rpath,.
endif

.PHONY: all directories headers install uninstall clean clean_headers
.SILENT:

all: directories $(HEADERS) $(DLLOBJECTS) $(EXEOBJECTS) $(CMDOBJECTS) $(RESOURCES) $(DLLNAME) $(EXENAME) $(CMDNAME)

directories:
	mkdir -p $(OBJECTDIR)

install: uninstall
ifneq ($(BUILD_WIN32),True)
	mkdir -p $(PREFIX)/bin

	cp $(EXENAME) $(PREFIX)/bin
	cp $(CMDNAME) $(PREFIX)/bin

	chmod a=rX,u=rwX $(PREFIX)/bin/freac
	chmod a=rX,u=rwX $(PREFIX)/bin/freaccmd

	mkdir -p $(PREFIX)/$(LIB)/freac
	chmod -R a=rX,u=rwX $(PREFIX)/$(LIB)/freac

	cp $(DLLNAME) $(PREFIX)/$(LIB)/freac
	chmod a=r,u=rw $(PREFIX)/$(LIB)/freac/freac$(SHARED)

	mkdir -p $(PREFIX)/share/freac
	chmod -R a=rX,u=rwX $(PREFIX)/share/freac

	cp -r $(BINDIR)/icons $(PREFIX)/share/freac
	chmod -R a=rX,u=rwX $(PREFIX)/share/freac/icons

	cp -r $(BINDIR)/lang $(PREFIX)/share/freac
	chmod -R a=rX,u=rwX $(PREFIX)/share/freac/lang

	cp $(BINDIR)/freac.pci $(PREFIX)/share/freac
	chmod a=r,u=rw $(PREFIX)/share/freac/freac.pci

	mkdir -p $(PREFIX)/share/doc/freac
	chmod -R a=rX,u=rwX $(PREFIX)/share/doc/freac

	cp -r $(BINDIR)/manual $(PREFIX)/share/doc/freac
	chmod -R a=rX,u=rwX $(PREFIX)/share/doc/freac/manual
endif

uninstall:
ifneq ($(BUILD_WIN32),True)
	rm -f $(PREFIX)/bin/freac
	rm -f $(PREFIX)/bin/freaccmd

	rm -f $(PREFIX)/$(LIB)/freac/freac$(SHARED)
	rm -f -r $(PREFIX)/$(LIB)/freac

	rm -f -r $(PREFIX)/share/freac/icons
	rm -f -r $(PREFIX)/share/freac/lang

	rm -f $(PREFIX)/share/freac/freac.pci
	rm -f -r $(PREFIX)/share/freac

	rm -f -r $(PREFIX)/share/doc/freac/manual

	rm -f -r $(PREFIX)/share/doc/freac
endif

clean:
	$(ECHO) -n Cleaning directories...
	$(REMOVER) $(REMOVER_OPTS) $(OBJECTS) $(DLLOBJECTS) $(EXEOBJECTS) $(CMDOBJECTS) $(RESOURCES) $(DLLNAME) $(EXENAME) $(CMDNAME) $(LIBNAME)
	$(ECHO) done.

$(DLLNAME): $(DLLOBJECTS)
	$(ECHO) Linking $(DLLNAME)...
	$(LINKER) $(DLLOBJECTS) $(LINKER_OPTS)
ifeq ($(BUILD_WIN32),True)
ifeq ($(BUILD_X86),True)
	countbuild BuildNumber
endif
endif
	$(ECHO) done.

$(EXENAME): $(EXEOBJECTS) $(RESOURCES)
	$(ECHO) -n Linking $(EXENAME)...
	$(LINKER) $(EXEOBJECTS) $(RESOURCES) $(LOADER_GUI_LINKER_OPTS)
ifeq ($(BUILD_HAIKU),True)
	xres -o $(EXENAME) resources/binary/freac.rsrc
endif
	$(ECHO) done.

$(CMDNAME): $(CMDOBJECTS) $(RESOURCES)
	$(ECHO) -n Linking $(CMDNAME)...
	$(LINKER) $(CMDOBJECTS) $(RESOURCES) $(LOADER_CONSOLE_LINKER_OPTS)
ifeq ($(BUILD_HAIKU),True)
	xres -o $(CMDNAME) resources/binary/freac.rsrc
endif
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

$(OBJECTDIR)/engine_converter.o: $(SRCDIR)/engine/converter.cpp
	$(ECHO) -n Compiling $(SRCDIR)/engine/converter.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/engine/converter.cpp -o $(OBJECTDIR)/engine_converter.o
	$(ECHO) done.

$(OBJECTDIR)/engine_decoder.o: $(SRCDIR)/engine/decoder.cpp
	$(ECHO) -n Compiling $(SRCDIR)/engine/decoder.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/engine/decoder.cpp -o $(OBJECTDIR)/engine_decoder.o
	$(ECHO) done.

$(OBJECTDIR)/engine_encoder.o: $(SRCDIR)/engine/encoder.cpp
	$(ECHO) -n Compiling $(SRCDIR)/engine/encoder.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/engine/encoder.cpp -o $(OBJECTDIR)/engine_encoder.o
	$(ECHO) done.

$(OBJECTDIR)/engine_worker.o: $(SRCDIR)/engine/worker.cpp
	$(ECHO) -n Compiling $(SRCDIR)/engine/worker.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/engine/worker.cpp -o $(OBJECTDIR)/engine_worker.o
	$(ECHO) done.

$(OBJECTDIR)/engine_worker_singlefile.o: $(SRCDIR)/engine/worker_singlefile.cpp
	$(ECHO) -n Compiling $(SRCDIR)/engine/worker_singlefile.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/engine/worker_singlefile.cpp -o $(OBJECTDIR)/engine_worker_singlefile.o
	$(ECHO) done.

$(OBJECTDIR)/job_convert.o: $(SRCDIR)/jobs/engine/convert.cpp
	$(ECHO) -n Compiling $(SRCDIR)/jobs/engine/convert.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/jobs/engine/convert.cpp -o $(OBJECTDIR)/job_convert.o
	$(ECHO) done.

$(OBJECTDIR)/job_adddirectory.o: $(SRCDIR)/jobs/joblist/adddirectory.cpp
	$(ECHO) -n Compiling $(SRCDIR)/jobs/joblist/adddirectory.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/jobs/joblist/adddirectory.cpp -o $(OBJECTDIR)/job_adddirectory.o
	$(ECHO) done.

$(OBJECTDIR)/job_addfiles.o: $(SRCDIR)/jobs/joblist/addfiles.cpp
	$(ECHO) -n Compiling $(SRCDIR)/jobs/joblist/addfiles.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/jobs/joblist/addfiles.cpp -o $(OBJECTDIR)/job_addfiles.o
	$(ECHO) done.

$(OBJECTDIR)/job_addtracks.o: $(SRCDIR)/jobs/joblist/addtracks.cpp
	$(ECHO) -n Compiling $(SRCDIR)/jobs/joblist/addtracks.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/jobs/joblist/addtracks.cpp -o $(OBJECTDIR)/job_addtracks.o
	$(ECHO) done.

$(OBJECTDIR)/job_removeall.o: $(SRCDIR)/jobs/joblist/removeall.cpp
	$(ECHO) -n Compiling $(SRCDIR)/jobs/joblist/removeall.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/jobs/joblist/removeall.cpp -o $(OBJECTDIR)/job_removeall.o
	$(ECHO) done.

$(OBJECTDIR)/job_removedisc.o: $(SRCDIR)/jobs/joblist/removedisc.cpp
	$(ECHO) -n Compiling $(SRCDIR)/jobs/joblist/removedisc.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/jobs/joblist/removedisc.cpp -o $(OBJECTDIR)/job_removedisc.o
	$(ECHO) done.

$(OBJECTDIR)/job_checkforupdates.o: $(SRCDIR)/jobs/other/checkforupdates.cpp
	$(ECHO) -n Compiling $(SRCDIR)/jobs/other/checkforupdates.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/jobs/other/checkforupdates.cpp -o $(OBJECTDIR)/job_checkforupdates.o
	$(ECHO) done.

$(OBJECTDIR)/job.o: $(SRCDIR)/jobs/job.cpp
	$(ECHO) -n Compiling $(SRCDIR)/jobs/job.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/jobs/job.cpp -o $(OBJECTDIR)/job.o
	$(ECHO) done.

$(OBJECTDIR)/jobmanager.o: $(SRCDIR)/jobs/jobmanager.cpp
	$(ECHO) -n Compiling $(SRCDIR)/jobs/jobmanager.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/jobs/jobmanager.cpp -o $(OBJECTDIR)/jobmanager.o
	$(ECHO) done.

$(OBJECTDIR)/tools_encoding.o: $(SRCDIR)/tools/encoding.cpp
	$(ECHO) -n Compiling $(SRCDIR)/tools/encoding.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/tools/encoding.cpp -o $(OBJECTDIR)/tools_encoding.o
	$(ECHO) done.

$(OBJECTDIR)/bonkenc.o: $(SRCDIR)/bonkenc.cpp
	$(ECHO) -n Compiling $(SRCDIR)/bonkenc.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/bonkenc.cpp -o $(OBJECTDIR)/bonkenc.o
	$(ECHO) done.

$(OBJECTDIR)/config.o: $(SRCDIR)/config.cpp
	$(ECHO) -n Compiling $(SRCDIR)/config.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/config.cpp -o $(OBJECTDIR)/config.o
	$(ECHO) done.

$(OBJECTDIR)/dllinterfaces.o: $(SRCDIR)/dllinterfaces.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dllinterfaces.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dllinterfaces.cpp -o $(OBJECTDIR)/dllinterfaces.o
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

$(OBJECTDIR)/charset.o: $(SRCDIR)/dialogs/charset.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/charset.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/charset.cpp -o $(OBJECTDIR)/charset.o
	$(ECHO) done.

$(OBJECTDIR)/error.o: $(SRCDIR)/dialogs/error.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/error.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/error.cpp -o $(OBJECTDIR)/error.o
	$(ECHO) done.

$(OBJECTDIR)/overwrite.o: $(SRCDIR)/dialogs/overwrite.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/overwrite.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/overwrite.cpp -o $(OBJECTDIR)/overwrite.o
	$(ECHO) done.

$(OBJECTDIR)/playback.o: $(SRCDIR)/playback.cpp
	$(ECHO) -n Compiling $(SRCDIR)/playback.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/playback.cpp -o $(OBJECTDIR)/playback.o
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

$(OBJECTDIR)/config_encoders.o: $(SRCDIR)/dialogs/config/config_encoders.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/config/config_encoders.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/config/config_encoders.cpp -o $(OBJECTDIR)/config_encoders.o
	$(ECHO) done.

$(OBJECTDIR)/config_interface.o: $(SRCDIR)/dialogs/config/config_interface.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/config/config_interface.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/config/config_interface.cpp -o $(OBJECTDIR)/config_interface.o
	$(ECHO) done.

$(OBJECTDIR)/config_language.o: $(SRCDIR)/dialogs/config/config_language.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/config/config_language.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/config/config_language.cpp -o $(OBJECTDIR)/config_language.o
	$(ECHO) done.

$(OBJECTDIR)/config_playlists.o: $(SRCDIR)/dialogs/config/config_playlists.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/config/config_playlists.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/config/config_playlists.cpp -o $(OBJECTDIR)/config_playlists.o
	$(ECHO) done.

$(OBJECTDIR)/config_resources.o: $(SRCDIR)/dialogs/config/config_resources.cpp
	$(ECHO) -n Compiling $(SRCDIR)/dialogs/config/config_resources.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/dialogs/config/config_resources.cpp -o $(OBJECTDIR)/config_resources.o
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

$(OBJECTDIR)/layer_tooltip.o: $(SRCDIR)/gui/layer_tooltip.cpp
	$(ECHO) -n Compiling $(SRCDIR)/gui/layer_tooltip.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/gui/layer_tooltip.cpp -o $(OBJECTDIR)/layer_tooltip.o
	$(ECHO) done.

$(OBJECTDIR)/main_joblist.o: $(SRCDIR)/gui/main_joblist.cpp
	$(ECHO) -n Compiling $(SRCDIR)/gui/main_joblist.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/gui/main_joblist.cpp -o $(OBJECTDIR)/main_joblist.o
	$(ECHO) done.

$(OBJECTDIR)/main_threads.o: $(SRCDIR)/gui/main_threads.cpp
	$(ECHO) -n Compiling $(SRCDIR)/gui/main_threads.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/gui/main_threads.cpp -o $(OBJECTDIR)/main_threads.o
	$(ECHO) done.

$(OBJECTDIR)/player.o: $(SRCDIR)/gui/player.cpp
	$(ECHO) -n Compiling $(SRCDIR)/gui/player.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/gui/player.cpp -o $(OBJECTDIR)/player.o
	$(ECHO) done.

$(OBJECTDIR)/console.o: $(SRCDIR)/loader/console.cpp
	$(ECHO) -n Compiling $(SRCDIR)/loader/console.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/loader/console.cpp -o $(OBJECTDIR)/console.o
	$(ECHO) done.

$(OBJECTDIR)/gui.o: $(SRCDIR)/loader/gui.cpp
	$(ECHO) -n Compiling $(SRCDIR)/loader/gui.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/loader/gui.cpp -o $(OBJECTDIR)/gui.o
	$(ECHO) done.

$(OBJECTDIR)/autorelease_none.o: $(SRCDIR)/support/autorelease_none.cpp
	$(ECHO) -n Compiling $(SRCDIR)/support/autorelease_none.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/support/autorelease_none.cpp -o $(OBJECTDIR)/autorelease_none.o
	$(ECHO) done.

$(OBJECTDIR)/autorelease_osx.o: $(SRCDIR)/support/autorelease_osx.mm
	$(ECHO) -n Compiling $(SRCDIR)/support/autorelease_osx.mm...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/support/autorelease_osx.mm -o $(OBJECTDIR)/autorelease_osx.o
	$(ECHO) done.

$(OBJECTDIR)/notification_none.o: $(SRCDIR)/support/notification_none.cpp
	$(ECHO) -n Compiling $(SRCDIR)/support/notification_none.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/support/notification_none.cpp -o $(OBJECTDIR)/notification_none.o
	$(ECHO) done.

$(OBJECTDIR)/notification_osx.o: $(SRCDIR)/support/notification_osx.mm
	$(ECHO) -n Compiling $(SRCDIR)/support/notification_osx.mm...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/support/notification_osx.mm -o $(OBJECTDIR)/notification_osx.o
	$(ECHO) done.

$(OBJECTDIR)/notification_udev.o: $(SRCDIR)/support/notification_udev.cpp
	$(ECHO) -n Compiling $(SRCDIR)/support/notification_udev.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/support/notification_udev.cpp -o $(OBJECTDIR)/notification_udev.o
	$(ECHO) done.

$(OBJECTDIR)/notification_win32.o: $(SRCDIR)/support/notification_win32.cpp
	$(ECHO) -n Compiling $(SRCDIR)/support/notification_win32.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(SRCDIR)/support/notification_win32.cpp -o $(OBJECTDIR)/notification_win32.o
	$(ECHO) done.

$(OBJECTDIR)/resources.o: $(RESOURCEDIR)/resources.rc $(INCLUDEDIR)/resources.h $(BINRESDIR)/freac.ico
	$(ECHO) -n Compiling $(RESOURCEDIR)/resources.rc...
	$(RESCOMP) $(RESCOMP_OPTS) $(RESOURCEDIR)/resources.rc -o $(OBJECTDIR)/resources.o
	$(ECHO) done.

$(OBJECTDIR)/resources_vd.o: $(RESOURCEDIR)/resources_vd.rc $(INCLUDEDIR)/resources.h $(BINRESDIR)/freac.ico
	$(ECHO) -n Compiling $(RESOURCEDIR)/resources_vd.rc...
	$(RESCOMP) $(RESCOMP_OPTS) $(RESOURCEDIR)/resources_vd.rc -o $(OBJECTDIR)/resources_vd.o
	$(ECHO) done.
