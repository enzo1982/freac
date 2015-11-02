# Makefile for fre:ac

include $(dir $(firstword $(MAKEFILE_LIST)))/Makefile-options

CDK	= $(SRCDIR)/cdk

INCLUDE	= $(SRCDIR)/include
OBJECTS	= objects
SRC	= src

BIN	= bin
LIB	= $(CDK)/lib

ifeq ($(BUILD_WIN32),True)
ifeq ($(BUILD_X86_64),True)
	BIN = $(CURDIR)/bin64
	LIB = $(CDK)/lib64
endif
endif

RESOURCES  = resources
BINRES	   = $(RESOURCES)/binary

DLLOBJECTS = $(OBJECTS)/cddb.o $(OBJECTS)/cddbbatch.o $(OBJECTS)/cddbcache.o $(OBJECTS)/cddbinfo.o $(OBJECTS)/cddblocal.o $(OBJECTS)/cddbremote.o $(OBJECTS)/cddb_extsettings.o $(OBJECTS)/cddb_manage.o $(OBJECTS)/cddb_managequeries.o $(OBJECTS)/cddb_managesubmits.o $(OBJECTS)/cddb_multimatch.o $(OBJECTS)/cddb_query.o $(OBJECTS)/cddb_submit.o $(OBJECTS)/dialog_config.o $(OBJECTS)/config_cddb.o $(OBJECTS)/config_encoders.o $(OBJECTS)/config_interface.o $(OBJECTS)/config_language.o $(OBJECTS)/config_playlists.o $(OBJECTS)/config_resources.o $(OBJECTS)/config_tags.o $(OBJECTS)/config_verification.o $(OBJECTS)/configcomponent.o $(OBJECTS)/configentry.o $(OBJECTS)/adddirectory.o $(OBJECTS)/addpattern.o $(OBJECTS)/charset.o $(OBJECTS)/error.o $(OBJECTS)/overwrite.o $(OBJECTS)/engine_converter.o $(OBJECTS)/engine_decoder.o $(OBJECTS)/engine_encoder.o $(OBJECTS)/engine_verifier.o $(OBJECTS)/engine_worker.o $(OBJECTS)/engine_worker_singlefile.o $(OBJECTS)/layer_tooltip.o $(OBJECTS)/main_joblist.o $(OBJECTS)/main_threads.o $(OBJECTS)/player.o $(OBJECTS)/job_convert.o $(OBJECTS)/job_adddirectory.o $(OBJECTS)/job_addfiles.o $(OBJECTS)/job_addtracks.o $(OBJECTS)/job_removeall.o $(OBJECTS)/job_removedisc.o $(OBJECTS)/job_checkforupdates.o $(OBJECTS)/job.o $(OBJECTS)/jobmanager.o $(OBJECTS)/bonkenc.o $(OBJECTS)/config.o $(OBJECTS)/dllinterfaces.o $(OBJECTS)/joblist.o $(OBJECTS)/playback.o $(OBJECTS)/progress.o $(OBJECTS)/startconsole.o $(OBJECTS)/startgui.o $(OBJECTS)/utilities.o

ifeq ($(BUILD_WIN32),True)
	DLLOBJECTS += $(OBJECTS)/autorelease_none.o
	DLLOBJECTS += $(OBJECTS)/notification_win32.o
else ifeq ($(BUILD_LINUX),True)
	DLLOBJECTS += $(OBJECTS)/autorelease_none.o
	DLLOBJECTS += $(OBJECTS)/notification_udev.o
else ifeq ($(BUILD_OSX),True)
	DLLOBJECTS += $(OBJECTS)/autorelease_osx.o
	DLLOBJECTS += $(OBJECTS)/notification_osx.o
else
	DLLOBJECTS += $(OBJECTS)/autorelease_none.o
	DLLOBJECTS += $(OBJECTS)/notification_none.o
endif

ifeq ($(BUILD_WIN32),True)
	ifeq ($(BUILD_VIDEO_DOWNLOADER),True)
		RESOBJECTS = $(OBJECTS)/resources_vd.o
	else
		RESOBJECTS = $(OBJECTS)/resources.o
	endif
endif

EXEOBJECTS = $(OBJECTS)/gui.o
CMDOBJECTS = $(OBJECTS)/console.o

EXENAME	= $(BIN)/freac$(EXECUTABLE)
CMDNAME	= $(BIN)/freaccmd$(EXECUTABLE)
DLLNAME	= $(BIN)/freac$(SHARED)
LIBNAME	= $(OBJECTS)/libfreac.a

COMPILER      = gcc
RESCOMP	      = windres
LINKER	      = gcc
REMOVER	      = rm
ECHO	      = echo
COMPILER_OPTS = -I$(INCLUDE) -fvisibility=hidden -c
RESCOMP_OPTS  = -O coff
LINKER_OPTS   = -lstdc++
REMOVER_OPTS  = -f

ifeq ($(BUILD_FREEBSD),True)
	COMPILER = clang
	LINKER	 = clang
endif

ifeq ($(BUILD_VIDEO_DOWNLOADER),True)
	COMPILER_OPTS		+= -D BUILD_VIDEO_DOWNLOADER
endif

ifeq ($(BUILD_WIN32),True)
	COMPILER_OPTS		+= -DUNICODE -D_UNICODE -I$(CDK)/include

	LINKER_OPTS		+= -L$(LIB) -lsmooth -Wl,--dynamicbase,--nxcompat

	LINKER_OPTS_DLL		+= --shared -lboca -lws2_32 -Wl,--out-implib,$(LIBNAME)
	LINKER_OPTS_GUI		+= -mwindows

	ifeq ($(BUILD_X86),True)
		RESCOMP_OPTS	 += --target=pe-i386
	else ifeq ($(BUILD_X86_64),True)
		RESCOMP_OPTS	 += --target=pe-x86-64
	endif
else
	COMPILER_OPTS		+= -fPIC

	ifneq ($(BUILD_SOLARIS),True)
	ifneq ($(BUILD_HAIKU),True)
		ifneq ($(BUILD_QNX),True)
			CCOPTS += -pthread
		endif

		ifneq ($(BUILD_LINUX),True)
		ifneq ($(BUILD_OSX),True)
			ifeq ($(BUILD_NETBSD),True)
				CCOPTS += -I/usr/pkg/include
			else
				CCOPTS += -I/usr/local/include
			endif
		endif
		endif
	endif
	endif

	LINKER_OPTS		+= -L$(prefix)/lib -lsmooth-$(SMOOTHVER)

	LINKER_OPTS_DLL		+= -lboca-$(BOCAVER)

	ifeq ($(BUILD_OSX),True)
		LINKER_OPTS_DLL	+= -dynamiclib -framework Cocoa -framework IOKit -Wl,-dylib_install_name,freac$(SHARED) -Wl,-headerpad,80
	else
		LINKER_OPTS_DLL	+= --shared
	endif

	ifeq ($(BUILD_OPENBSD),True)
		LINKER_OPTS_GUI	+= -L/usr/X11R6/lib -L/usr/local/lib -logg -lvorbis -lvorbisfile
		LINKER_OPTS_CMD	+= -L/usr/X11R6/lib -L/usr/local/lib -logg -lvorbis -lvorbisfile
	endif
endif

.PHONY: all folders install uninstall clean
.SILENT:

all: folders $(DLLOBJECTS) $(EXEOBJECTS) $(CMDOBJECTS) $(RESOBJECTS) $(DLLNAME) $(EXENAME) $(CMDNAME)

folders:
	mkdir -p $(BIN) $(OBJECTS)

install: all
ifneq ($(BUILD_WIN32),True)
	$(INSTALL) -d $(DESTDIR)$(bindir)

	$(INSTALL_PROGRAM) $(EXENAME) $(DESTDIR)$(bindir)
	$(INSTALL_PROGRAM) $(CMDNAME) $(DESTDIR)$(bindir)

	$(INSTALL) -d $(DESTDIR)$(libdir)/freac

	$(INSTALL_DATA) $(DLLNAME) $(DESTDIR)$(libdir)/freac

	$(INSTALL) -d $(DESTDIR)$(datadir)/freac

	cp -r $(SRCDIR)/bin/icons $(DESTDIR)$(datadir)/freac
	chmod -R a=rX,u=rwX $(DESTDIR)$(datadir)/freac/icons

	cp -r $(SRCDIR)/bin/lang $(DESTDIR)$(datadir)/freac
	chmod -R a=rX,u=rwX $(DESTDIR)$(datadir)/freac/lang

	$(INSTALL_DATA) $(SRCDIR)/bin/freac.pci $(DESTDIR)$(datadir)/freac

	$(INSTALL) -d $(DESTDIR)$(datadir)/doc/freac

	$(INSTALL_DATA) $(SRCDIR)/Readme* $(DESTDIR)$(datadir)/doc/freac

	cp -r $(SRCDIR)/bin/manual $(DESTDIR)$(datadir)/doc/freac
	chmod -R a=rX,u=rwX $(DESTDIR)$(datadir)/doc/freac/manual
endif

uninstall:
ifneq ($(BUILD_WIN32),True)
	rm -f $(DESTDIR)$(bindir)/freac
	rm -f $(DESTDIR)$(bindir)/freaccmd

	rm -f $(DESTDIR)$(libdir)/freac/freac$(SHARED)
	rm -f -r $(DESTDIR)$(libdir)/freac

	rm -f -r $(DESTDIR)$(datadir)/freac/icons
	rm -f -r $(DESTDIR)$(datadir)/freac/lang

	rm -f $(DESTDIR)$(datadir)/freac/freac.pci
	rm -f -r $(DESTDIR)$(datadir)/freac

	rm -f $(DESTDIR)$(datadir)/doc/freac/Readme*
	rm -f -r $(DESTDIR)$(datadir)/doc/freac/manual

	rm -f -r $(DESTDIR)$(datadir)/doc/freac
endif

clean:
	$(ECHO) -n Cleaning directories...
	$(REMOVER) $(REMOVER_OPTS) $(DLLOBJECTS) $(EXEOBJECTS) $(CMDOBJECTS) $(RESOBJECTS) $(DLLNAME) $(EXENAME) $(CMDNAME) $(LIBNAME)
	rmdir $(BIN) $(OBJECTS) 2> /dev/null || true
	$(ECHO) done.

$(DLLNAME): $(DLLOBJECTS)
	$(ECHO) Linking $(DLLNAME)...
	$(LINKER) $(DLLOBJECTS) $(LINKER_OPTS) $(LINKER_OPTS_DLL) $(LDFLAGS) -o $@
ifeq ($(BUILD_WIN32),True)
ifeq ($(BUILD_X86),True)
	countbuild BuildNumber
endif
endif
	$(ECHO) done.

$(EXENAME): $(EXEOBJECTS) $(RESOBJECTS)
	$(ECHO) -n Linking $(EXENAME)...
	$(LINKER) $(EXEOBJECTS) $(RESOBJECTS) $(LINKER_OPTS) $(LINKER_OPTS_GUI) $(LDFLAGS) -o $@
ifeq ($(BUILD_HAIKU),True)
	xres -o $(EXENAME) resources/binary/freac.rsrc
endif
	$(ECHO) done.

$(CMDNAME): $(CMDOBJECTS) $(RESOBJECTS)
	$(ECHO) -n Linking $(CMDNAME)...
	$(LINKER) $(CMDOBJECTS) $(RESOBJECTS) $(LINKER_OPTS) $(LINKER_OPTS_CMD) $(LDFLAGS) -o $@
ifeq ($(BUILD_HAIKU),True)
	xres -o $(CMDNAME) resources/binary/freac.rsrc
endif
	$(ECHO) done.

$(OBJECTS)/%.o: $(SRC)/%.cpp
	$(ECHO) -n Compiling $<...
	$(COMPILER) $(COMPILER_OPTS) $(CXXFLAGS) $< -o $@
	$(ECHO) done.

$(OBJECTS)/%.o: $(SRC)/cddb/%.cpp
	$(ECHO) -n Compiling $<...
	$(COMPILER) $(COMPILER_OPTS) $(CXXFLAGS) $< -o $@
	$(ECHO) done.

$(OBJECTS)/%.o: $(SRC)/dialogs/%.cpp
	$(ECHO) -n Compiling $<...
	$(COMPILER) $(COMPILER_OPTS) $(CXXFLAGS) $< -o $@
	$(ECHO) done.

$(OBJECTS)/cddb_%.o: $(SRC)/dialogs/cddb/%.cpp
	$(ECHO) -n Compiling $<...
	$(COMPILER) $(COMPILER_OPTS) $(CXXFLAGS) $< -o $@
	$(ECHO) done.

$(OBJECTS)/dialog_%.o: $(SRC)/dialogs/config/%.cpp
	$(ECHO) -n Compiling $<...
	$(COMPILER) $(COMPILER_OPTS) $(CXXFLAGS) $< -o $@
	$(ECHO) done.

$(OBJECTS)/%.o: $(SRC)/dialogs/config/%.cpp
	$(ECHO) -n Compiling $<...
	$(COMPILER) $(COMPILER_OPTS) $(CXXFLAGS) $< -o $@
	$(ECHO) done.

$(OBJECTS)/engine_%.o: $(SRC)/engine/%.cpp
	$(ECHO) -n Compiling $<...
	$(COMPILER) $(COMPILER_OPTS) $(CXXFLAGS) $< -o $@
	$(ECHO) done.

$(OBJECTS)/%.o: $(SRC)/gui/%.cpp
	$(ECHO) -n Compiling $<...
	$(COMPILER) $(COMPILER_OPTS) $(CXXFLAGS) $< -o $@
	$(ECHO) done.

$(OBJECTS)/%.o: $(SRC)/jobs/%.cpp
	$(ECHO) -n Compiling $<...
	$(COMPILER) $(COMPILER_OPTS) $(CXXFLAGS) $< -o $@
	$(ECHO) done.

$(OBJECTS)/job_%.o: $(SRC)/jobs/engine/%.cpp
	$(ECHO) -n Compiling $<...
	$(COMPILER) $(COMPILER_OPTS) $(CXXFLAGS) $< -o $@
	$(ECHO) done.

$(OBJECTS)/job_%.o: $(SRC)/jobs/joblist/%.cpp
	$(ECHO) -n Compiling $<...
	$(COMPILER) $(COMPILER_OPTS) $(CXXFLAGS) $< -o $@
	$(ECHO) done.

$(OBJECTS)/job_%.o: $(SRC)/jobs/other/%.cpp
	$(ECHO) -n Compiling $<...
	$(COMPILER) $(COMPILER_OPTS) $(CXXFLAGS) $< -o $@
	$(ECHO) done.

$(OBJECTS)/%.o: $(SRC)/loader/%.cpp
	$(ECHO) -n Compiling $<...
	$(COMPILER) $(COMPILER_OPTS) $(CXXFLAGS) $< -o $@
	$(ECHO) done.

$(OBJECTS)/%.o: $(SRC)/support/%.cpp
	$(ECHO) -n Compiling $<...
	$(COMPILER) $(COMPILER_OPTS) $(CXXFLAGS) $< -o $@
	$(ECHO) done.

$(OBJECTS)/%.o: $(SRC)/support/%.mm
	$(ECHO) -n Compiling $<...
	$(COMPILER) $(COMPILER_OPTS) $(OBJCXXFLAGS) $< -o $@
	$(ECHO) done.

$(OBJECTS)/%.o: $(RESOURCES)/%.rc $(INCLUDE)/resources.h $(BINRES)/freac.ico
	$(ECHO) -n Compiling $<...
	$(RESCOMP) $(RESCOMP_OPTS) $< -o $@
	$(ECHO) done.
