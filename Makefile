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

DLLOBJECTS = $(OBJECTS)/cddb.o $(OBJECTS)/cddbbatch.o $(OBJECTS)/cddbcache.o $(OBJECTS)/cddbinfo.o $(OBJECTS)/cddblocal.o $(OBJECTS)/cddbremote.o $(OBJECTS)/cddb_extsettings.o $(OBJECTS)/cddb_manage.o $(OBJECTS)/cddb_managequeries.o $(OBJECTS)/cddb_managesubmits.o $(OBJECTS)/cddb_multimatch.o $(OBJECTS)/cddb_query.o $(OBJECTS)/cddb_submit.o $(OBJECTS)/dialog_config.o $(OBJECTS)/config_cddb.o $(OBJECTS)/config_encoders.o $(OBJECTS)/config_interface.o $(OBJECTS)/config_language.o $(OBJECTS)/config_playlists.o $(OBJECTS)/config_resources.o $(OBJECTS)/config_tags.o $(OBJECTS)/config_verification.o $(OBJECTS)/configcomponent.o $(OBJECTS)/configentry.o $(OBJECTS)/adddirectory.o $(OBJECTS)/addpattern.o $(OBJECTS)/charset.o $(OBJECTS)/error.o $(OBJECTS)/overwrite.o $(OBJECTS)/engine_converter.o $(OBJECTS)/engine_decoder.o $(OBJECTS)/engine_encoder.o $(OBJECTS)/engine_verifier.o $(OBJECTS)/engine_worker.o $(OBJECTS)/engine_worker_singlefile.o $(OBJECTS)/edit_folder.o $(OBJECTS)/layer_tooltip.o $(OBJECTS)/main_joblist.o $(OBJECTS)/main_threads.o $(OBJECTS)/player.o $(OBJECTS)/job_convert.o $(OBJECTS)/job_adddirectory.o $(OBJECTS)/job_addfiles.o $(OBJECTS)/job_addtracks.o $(OBJECTS)/job_removeall.o $(OBJECTS)/job_removedisc.o $(OBJECTS)/job_checkforupdates.o $(OBJECTS)/job.o $(OBJECTS)/jobmanager.o $(OBJECTS)/bonkenc.o $(OBJECTS)/config.o $(OBJECTS)/dllinterfaces.o $(OBJECTS)/joblist.o $(OBJECTS)/playback.o $(OBJECTS)/progress.o $(OBJECTS)/startconsole.o $(OBJECTS)/startgui.o $(OBJECTS)/utilities.o

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

RESCOMP	     = windres
REMOVER	     = rm
CCOPTS	     = -I$(INCLUDE) -fvisibility=hidden -c
LDOPTS	     = -lstdc++
RESCOMP_OPTS = -O coff
REMOVER_OPTS = -f

ifeq ($(BUILD_VIDEO_DOWNLOADER),True)
	CCOPTS			+= -D BUILD_VIDEO_DOWNLOADER
endif

ifeq ($(BUILD_WIN32),True)
	CCOPTS			+= -DUNICODE -D_UNICODE -I$(CDK)/include

	LDOPTS			+= -L$(LIB) -lsmooth -Wl,--dynamicbase,--nxcompat

	LDOPTS_DLL		+= --shared -lboca -lws2_32 -Wl,--out-implib,$(LIBNAME)
	LDOPTS_GUI		+= -mwindows

	ifeq ($(BUILD_X86),True)
		RESCOMP_OPTS	+= --target=pe-i386
	else ifeq ($(BUILD_X86_64),True)
		RESCOMP_OPTS	+= --target=pe-x86-64
	endif
else
	CCOPTS			+= -fPIC

	ifneq ($(BUILD_SOLARIS),True)
	ifneq ($(BUILD_HAIKU),True)
		ifneq ($(BUILD_QNX),True)
			CCOPTS	+= -pthread
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

	LDOPTS			+= -L$(prefix)/lib -lsmooth-$(SMOOTHVER)

	LDOPTS_DLL		+= -lboca-$(BOCAVER)

	ifeq ($(BUILD_OSX),True)
		LDOPTS_DLL	+= -dynamiclib -framework Cocoa -framework IOKit -Wl,-dylib_install_name,freac$(SHARED) -Wl,-headerpad,80
	else
		LDOPTS_DLL	+= --shared
	endif

	ifeq ($(BUILD_OPENBSD),True)
		LDOPTS_GUI	+= -L/usr/X11R6/lib -L/usr/local/lib -logg -lvorbis -lvorbisfile
		LDOPTS_CMD	+= -L/usr/X11R6/lib -L/usr/local/lib -logg -lvorbis -lvorbisfile
	endif
endif

.PHONY: all folders install uninstall clean

all: folders $(DLLOBJECTS) $(EXEOBJECTS) $(CMDOBJECTS) $(RESOBJECTS) $(DLLNAME) $(EXENAME) $(CMDNAME)

	+ $(call makein,components)

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

	$(call makein,components,install)
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

	$(call makein,components,uninstall)
endif

clean:
	$(REMOVER) $(REMOVER_OPTS) $(DLLOBJECTS) $(EXEOBJECTS) $(CMDOBJECTS) $(RESOBJECTS) $(DLLNAME) $(EXENAME) $(CMDNAME) $(LIBNAME)
ifneq ($(SRCDIR),$(CURDIR))
	rmdir $(BIN) $(OBJECTS) 2> /dev/null || true
endif

	+ $(call cleanin,components)

$(DLLNAME): $(DLLOBJECTS)
	$(LD) $(DLLOBJECTS) $(LDOPTS) $(LDOPTS_DLL) $(LDFLAGS) -o $@
ifeq ($(BUILD_WIN32),True)
ifeq ($(BUILD_X86),True)
	countbuild BuildNumber
endif
endif

$(EXENAME): $(EXEOBJECTS) $(RESOBJECTS)
	$(LD) $(EXEOBJECTS) $(RESOBJECTS) $(LDOPTS) $(LDOPTS_GUI) $(LDFLAGS) -o $@
ifeq ($(BUILD_HAIKU),True)
	xres -o $(EXENAME) resources/binary/freac.rsrc
endif

$(CMDNAME): $(CMDOBJECTS) $(RESOBJECTS)
	$(LD) $(CMDOBJECTS) $(RESOBJECTS) $(LDOPTS) $(LDOPTS_CMD) $(LDFLAGS) -o $@
ifeq ($(BUILD_HAIKU),True)
	xres -o $(CMDNAME) resources/binary/freac.rsrc
endif

$(OBJECTS)/%.o: $(SRC)/%.cpp
	$(CXX) $(CCOPTS) $(CXXFLAGS) $< -o $@

$(OBJECTS)/%.o: $(SRC)/cddb/%.cpp
	$(CXX) $(CCOPTS) $(CXXFLAGS) $< -o $@

$(OBJECTS)/%.o: $(SRC)/dialogs/%.cpp
	$(CXX) $(CCOPTS) $(CXXFLAGS) $< -o $@

$(OBJECTS)/cddb_%.o: $(SRC)/dialogs/cddb/%.cpp
	$(CXX) $(CCOPTS) $(CXXFLAGS) $< -o $@

$(OBJECTS)/dialog_%.o: $(SRC)/dialogs/config/%.cpp
	$(CXX) $(CCOPTS) $(CXXFLAGS) $< -o $@

$(OBJECTS)/%.o: $(SRC)/dialogs/config/%.cpp
	$(CXX) $(CCOPTS) $(CXXFLAGS) $< -o $@

$(OBJECTS)/engine_%.o: $(SRC)/engine/%.cpp
	$(CXX) $(CCOPTS) $(CXXFLAGS) $< -o $@

$(OBJECTS)/%.o: $(SRC)/gui/%.cpp
	$(CXX) $(CCOPTS) $(CXXFLAGS) $< -o $@

$(OBJECTS)/%.o: $(SRC)/jobs/%.cpp
	$(CXX) $(CCOPTS) $(CXXFLAGS) $< -o $@

$(OBJECTS)/job_%.o: $(SRC)/jobs/engine/%.cpp
	$(CXX) $(CCOPTS) $(CXXFLAGS) $< -o $@

$(OBJECTS)/job_%.o: $(SRC)/jobs/joblist/%.cpp
	$(CXX) $(CCOPTS) $(CXXFLAGS) $< -o $@

$(OBJECTS)/job_%.o: $(SRC)/jobs/other/%.cpp
	$(CXX) $(CCOPTS) $(CXXFLAGS) $< -o $@

$(OBJECTS)/%.o: $(SRC)/loader/%.cpp
	$(CXX) $(CCOPTS) $(CXXFLAGS) $< -o $@

$(OBJECTS)/%.o: $(SRC)/support/%.cpp
	$(CXX) $(CCOPTS) $(CXXFLAGS) $< -o $@

$(OBJECTS)/%.o: $(SRC)/support/%.mm
	$(OBJCXX) $(CCOPTS) $(OBJCXXFLAGS) $< -o $@

$(OBJECTS)/%.o: $(RESOURCES)/%.rc $(INCLUDE)/resources.h $(BINRES)/freac.ico
	$(RESCOMP) $(RESCOMP_OPTS) $< -o $@
