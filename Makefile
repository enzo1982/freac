# Makefile for fre:ac

include $(dir $(firstword $(MAKEFILE_LIST)))/Makefile-options

INCLUDE	  = $(SRCDIR)/include
OBJECTS	  = objects
RESOURCES = resources
SRC	  = src

BIN	  = bin
LIB	  = lib

ifeq ($(BUILD_WIN32),True)
ifeq ($(BUILD_X86_64),True)
	BIN = bin64
	LIB = lib64
endif
endif

DLLOBJECTS  = $(OBJECTS)/cddb.o $(OBJECTS)/cddbbatch.o $(OBJECTS)/cddbcache.o $(OBJECTS)/cddbinfo.o $(OBJECTS)/cddblocal.o $(OBJECTS)/cddbremote.o
DLLOBJECTS += $(OBJECTS)/cddb_extsettings.o $(OBJECTS)/cddb_manage.o $(OBJECTS)/cddb_managequeries.o $(OBJECTS)/cddb_managesubmits.o $(OBJECTS)/cddb_multimatch.o $(OBJECTS)/cddb_query.o $(OBJECTS)/cddb_submit.o
DLLOBJECTS += $(OBJECTS)/dialog_config.o $(OBJECTS)/config_cddb.o $(OBJECTS)/config_dsp.o $(OBJECTS)/config_encoders.o $(OBJECTS)/config_interface.o $(OBJECTS)/config_language.o $(OBJECTS)/config_playlists.o $(OBJECTS)/config_resources.o $(OBJECTS)/config_tags.o $(OBJECTS)/config_verification.o $(OBJECTS)/configcomponent.o $(OBJECTS)/configentry.o
DLLOBJECTS += $(OBJECTS)/adddirectory.o $(OBJECTS)/addpattern.o $(OBJECTS)/charset.o $(OBJECTS)/error.o $(OBJECTS)/format.o $(OBJECTS)/overwrite.o
DLLOBJECTS += $(OBJECTS)/engine_component.o $(OBJECTS)/engine_converter.o $(OBJECTS)/engine_decoder.o $(OBJECTS)/engine_encoder.o $(OBJECTS)/engine_locking.o $(OBJECTS)/engine_processor.o $(OBJECTS)/engine_verifier.o $(OBJECTS)/engine_worker.o $(OBJECTS)/engine_worker_singlefile.o
DLLOBJECTS += $(OBJECTS)/edit_folder.o $(OBJECTS)/layer_tooltip.o $(OBJECTS)/main_joblist.o $(OBJECTS)/main_threads.o $(OBJECTS)/player.o
DLLOBJECTS += $(OBJECTS)/job_convert.o $(OBJECTS)/job_addfiles.o $(OBJECTS)/job_addfolders.o $(OBJECTS)/job_addtracks.o $(OBJECTS)/job_removeall.o $(OBJECTS)/job_removedisc.o $(OBJECTS)/job_checkforupdates.o $(OBJECTS)/job.o $(OBJECTS)/jobmanager.o
DLLOBJECTS += $(OBJECTS)/config.o $(OBJECTS)/dllinterfaces.o $(OBJECTS)/freac.o $(OBJECTS)/joblist.o $(OBJECTS)/playback.o $(OBJECTS)/progress.o $(OBJECTS)/startconsole.o $(OBJECTS)/startgui.o $(OBJECTS)/utilities.o

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

ifeq ($(BUILD_HAIKU),True)
	RESFILES = $(RESOURCES)/resources.rsrc
endif

EXEOBJECTS = $(OBJECTS)/gui.o
CMDOBJECTS = $(OBJECTS)/console.o

EXENAME	= $(BIN)/freac$(EXECUTABLE)
CMDNAME	= $(BIN)/freaccmd$(EXECUTABLE)
DLLNAME	= $(BIN)/freac$(SHARED)
LIBNAME	= $(OBJECTS)/libfreac.a

CCOPTS	     = -I$(INCLUDE) -fvisibility=hidden -c
LDOPTS	     = -lstdc++

REMOVER	     = rm
REMOVER_OPTS = -f

ifeq ($(BUILD_VIDEO_DOWNLOADER),True)
	CCOPTS			+= -D BUILD_VIDEO_DOWNLOADER
endif

ifeq ($(BUILD_WIN32),True)
	CCOPTS			+= -I$(CDK_INSTALL_PATH)/include

	LDOPTS			+= -L$(CDK_INSTALL_PATH)/$(LIB) -lsmooth -Wl,--dynamicbase,--nxcompat

	LDOPTS_DLL		+= --shared -lboca -lws2_32 -Wl,--out-implib,$(LIBNAME)
	LDOPTS_GUI		+= -mwindows

	RESCOMP			 = windres
	RESCOMP_OPTS		 = -O coff

	ifeq ($(BUILD_X86),True)
		RESCOMP_OPTS	+= --target=pe-i386
	else ifeq ($(BUILD_X86_64),True)
		RESCOMP_OPTS	+= --target=pe-x86-64
	endif
else
	CCOPTS			+= -fPIC

	ifneq ($(BUILD_SOLARIS),True)
	ifneq ($(BUILD_HAIKU),True)
		CCOPTS	+= -pthread

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

	ifeq ($(BUILD_HAIKU),True)
		RESCOMP		 = rc
		RESLINKER	 = xres
	endif
endif

.PHONY: all folders resources install uninstall clean clean_resources

all: folders $(DLLOBJECTS) $(EXEOBJECTS) $(CMDOBJECTS) $(RESOBJECTS) $(RESFILES) $(DLLNAME) $(EXENAME) $(CMDNAME) resources

	+ $(call makein,components)

folders:
	mkdir -p $(BIN) $(OBJECTS)

resources: folders
ifeq ($(BUILD_WIN32),True)
	mkdir -p $(BIN)/icons

	cp $(SRCDIR)/icons/freac.png $(BIN)/icons

	cp -r $(SRCDIR)/icons/conversion $(BIN)/icons
	cp -r $(SRCDIR)/icons/freedb $(BIN)/icons
	cp -r $(SRCDIR)/icons/joblist $(BIN)/icons
	cp -r $(SRCDIR)/icons/other $(BIN)/icons
	cp -r $(SRCDIR)/icons/player $(BIN)/icons
	cp -r $(SRCDIR)/icons/select $(BIN)/icons
	cp -r $(SRCDIR)/icons/settings $(BIN)/icons

	cp -r $(SRCDIR)/i18n/lang $(BIN)
	cp -r $(SRCDIR)/i18n/manual $(BIN)
endif

install: folders $(DLLOBJECTS) $(EXEOBJECTS) $(CMDOBJECTS) $(RESOBJECTS) $(DLLNAME) $(EXENAME) $(CMDNAME)
ifneq ($(BUILD_WIN32),True)
	$(INSTALL) -d $(DESTDIR)$(bindir)

	$(INSTALL_PROGRAM) $(EXENAME) $(DESTDIR)$(bindir)
	$(INSTALL_PROGRAM) $(CMDNAME) $(DESTDIR)$(bindir)

	$(INSTALL) -d $(DESTDIR)$(libdir)/freac

	$(INSTALL_DATA) $(DLLNAME) $(DESTDIR)$(libdir)/freac

	$(INSTALL) -d $(DESTDIR)$(datadir)/freac
	$(INSTALL) -d $(DESTDIR)$(datadir)/freac/icons

	cp -r $(SRCDIR)/i18n/lang $(DESTDIR)$(datadir)/freac
	chmod -R a=rX,u=rwX $(DESTDIR)$(datadir)/freac/lang

	$(INSTALL_DATA) $(SRCDIR)/icons/freac.png $(DESTDIR)$(datadir)/freac/icons

	cp -r $(SRCDIR)/icons/conversion $(DESTDIR)$(datadir)/freac/icons
	chmod -R a=rX,u=rwX $(DESTDIR)$(datadir)/freac/icons/conversion

	cp -r $(SRCDIR)/icons/freedb $(DESTDIR)$(datadir)/freac/icons
	chmod -R a=rX,u=rwX $(DESTDIR)$(datadir)/freac/icons/freedb

	cp -r $(SRCDIR)/icons/joblist $(DESTDIR)$(datadir)/freac/icons
	chmod -R a=rX,u=rwX $(DESTDIR)$(datadir)/freac/icons/joblist

	cp -r $(SRCDIR)/icons/other $(DESTDIR)$(datadir)/freac/icons
	chmod -R a=rX,u=rwX $(DESTDIR)$(datadir)/freac/icons/other

	cp -r $(SRCDIR)/icons/player $(DESTDIR)$(datadir)/freac/icons
	chmod -R a=rX,u=rwX $(DESTDIR)$(datadir)/freac/icons/player

	cp -r $(SRCDIR)/icons/select $(DESTDIR)$(datadir)/freac/icons
	chmod -R a=rX,u=rwX $(DESTDIR)$(datadir)/freac/icons/select

	cp -r $(SRCDIR)/icons/settings $(DESTDIR)$(datadir)/freac/icons
	chmod -R a=rX,u=rwX $(DESTDIR)$(datadir)/freac/icons/settings

	$(INSTALL) -d $(DESTDIR)$(datadir)/doc/freac

	$(INSTALL_DATA) $(SRCDIR)/Readme* $(DESTDIR)$(datadir)/doc/freac

	cp -r $(SRCDIR)/i18n/manual $(DESTDIR)$(datadir)/doc/freac
	chmod -R a=rX,u=rwX $(DESTDIR)$(datadir)/doc/freac/manual

ifneq ($(BUILD_OSX),True)
ifneq ($(BUILD_HAIKU),True)
	$(INSTALL) -d $(DESTDIR)$(datadir)/applications/
	$(INSTALL) -d $(DESTDIR)$(datadir)/metainfo/

	$(INSTALL_DATA) $(SRCDIR)/metadata/org.freac.freac.desktop $(DESTDIR)$(datadir)/applications
	$(INSTALL_DATA) $(SRCDIR)/metadata/org.freac.freac.appdata.xml $(DESTDIR)$(datadir)/metainfo

	$(INSTALL) -d $(DESTDIR)$(datadir)/icons/hicolor/64x64/apps/
	$(INSTALL) -d $(DESTDIR)$(datadir)/icons/hicolor/128x128/apps/

	$(INSTALL_DATA) $(SRCDIR)/icons/freac-64x64.png $(DESTDIR)$(datadir)/icons/hicolor/64x64/apps/org.freac.freac.png
	$(INSTALL_DATA) $(SRCDIR)/icons/freac.png $(DESTDIR)$(datadir)/icons/hicolor/128x128/apps/org.freac.freac.png
endif
endif

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

	rm -f -r $(DESTDIR)$(datadir)/freac

	rm -f $(DESTDIR)$(datadir)/doc/freac/Readme*
	rm -f -r $(DESTDIR)$(datadir)/doc/freac/manual

	rm -f -r $(DESTDIR)$(datadir)/doc/freac

ifneq ($(BUILD_OSX),True)
ifneq ($(BUILD_HAIKU),True)
	rm -f $(DESTDIR)$(datadir)/applications/org.freac.freac.desktop
	rm -f $(DESTDIR)$(datadir)/metainfo/org.freac.freac.appdata.xml

	rm -f $(DESTDIR)$(datadir)/icons/hicolor/64x64/apps/org.freac.freac.png
	rm -f $(DESTDIR)$(datadir)/icons/hicolor/128x128/apps/org.freac.freac.png
endif
endif

	$(call makein,components,uninstall)
endif

clean: clean_resources
	$(REMOVER) $(REMOVER_OPTS) $(DLLOBJECTS) $(EXEOBJECTS) $(CMDOBJECTS) $(RESOBJECTS) $(RESFILES) $(DLLNAME) $(EXENAME) $(CMDNAME) $(LIBNAME)
ifneq ($(SRCDIR),$(CURDIR))
	rmdir $(BIN) $(OBJECTS) 2> /dev/null || true
endif

	+ $(call cleanin,components)

clean_resources:
ifeq ($(BUILD_WIN32),True)
	rm -f -r $(BIN)/icons

	rm -f -r $(BIN)/lang
	rm -f -r $(BIN)/manual
endif

$(DLLNAME): $(DLLOBJECTS)
	$(LD) $(DLLOBJECTS) $(LDOPTS) $(LDOPTS_DLL) $(LDFLAGS) -o $@
ifeq ($(findstring release,$(config)),release)
	-countbuild BuildNumber
endif

$(EXENAME): $(EXEOBJECTS) $(RESOBJECTS) $(RESFILES)
	$(LD) $(EXEOBJECTS) $(RESOBJECTS) $(LDOPTS) $(LDOPTS_GUI) $(LDFLAGS) -o $@
ifeq ($(BUILD_HAIKU),True)
	$(RESLINKER) -o $@ $(RESFILES)
endif

$(CMDNAME): $(CMDOBJECTS) $(RESOBJECTS)
	$(LD) $(CMDOBJECTS) $(RESOBJECTS) $(LDOPTS) $(LDOPTS_CMD) $(LDFLAGS) -o $@

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

$(OBJECTS)/%.o: $(RESOURCES)/%.rc $(INCLUDE)/resources.h
	$(RESCOMP) $(RESCOMP_OPTS) -o $@ $<

$(RESOURCES)/%.rsrc: $(RESOURCES)/%.rdef
	$(RESCOMP) $(RESCOMP_OPTS) -o $@ $<
