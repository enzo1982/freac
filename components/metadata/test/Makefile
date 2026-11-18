########## fre:ac metadata component tests ##########

FREAC_PATH = ../../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(FREAC_PATH)/Makefile-options

TARGETS	= cddb$(EXECUTABLE) musicbrainz$(EXECUTABLE)

CCOPTS	= -I $(FREAC_PATH)/include

ifeq ($(BUILD_WIN32),True)
	LDOPTS = -lsmooth -lboca
else
	LDOPTS = -L$(prefix)/lib -lsmooth-$(SMOOTHVER) -lboca-$(BOCAVER)

	ifneq ($(BUILD_OSX),True)
		CCOPTS += -I/usr/local/include
		LDOPTS += -L/usr/local/lib
	endif
endif

ifneq ("$(CDK_INSTALL_PATH)","")
	CCOPTS += -I"$(CDK_INSTALL_PATH)"/include
	LDOPTS += -L"$(CDK_INSTALL_PATH)"/$(LIBDIR)
endif

CDDB_PATH	= ../cddb
CDDB_OBJECTS	= cddb_cddb.o cddb_cddbbatch.o cddb_cddbcache.o cddb_cddbconfig.o cddb_cddbinfo.o cddb_cddblocal.o cddb_cddbremote.o cddb_convert.o

MUSICBRAINZ_PATH	= ../musicbrainz
MUSICBRAINZ_OBJECTS	= musicbrainz_config.o musicbrainz_discid.o musicbrainz_network.o musicbrainz_parser.o

all: $(TARGETS)

cddb$(EXECUTABLE): cddb.o $(CDDB_OBJECTS)
	$(LD) -o $@ cddb.o $(CDDB_OBJECTS) $(LDOPTS) $(LDFLAGS)

cddb_%.o: $(CDDB_PATH)/%.cpp
	$(CXX) $(CCOPTS) $(CXXFLAGS) -c $< -o $@

musicbrainz$(EXECUTABLE): musicbrainz.o $(MUSICBRAINZ_OBJECTS)
	$(LD) -o $@ musicbrainz.o $(MUSICBRAINZ_OBJECTS) $(LDOPTS) $(LDFLAGS)

musicbrainz_%.o: $(MUSICBRAINZ_PATH)/%.cpp
	$(CXX) $(CCOPTS) $(CXXFLAGS) -c $< -o $@

# BoCA loads the components from the boca folder next to the program,
# so check copies the metadata components of fre:ac there. Build fre:ac
# first.
check: components $(TARGETS)
	./cddb$(EXECUTABLE)
	./musicbrainz$(EXECUTABLE)

components:
	mkdir -p boca
	cp $(FREAC_PATH)/$(BINDIR)/boca/freac_metadata_*$(SHARED) boca

clean:
	rm -f $(TARGETS) *.o
	rm -rf boca

.PHONY: check components clean

.cpp.o:
	$(CXX) $(CCOPTS) $(CXXFLAGS) -c $< -o $@
