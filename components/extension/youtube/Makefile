########## fre:ac component makefile ##########

# Change these variables to fit your project:
TARGET	  = youtube
TYPE	  = extension
VERSION	  = 1.0

FREAC_PATH = ../../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(FREAC_PATH)/Makefile-options

# Disable PowerPC builds of this component on macOS for now.
ifeq ($(BUILD_OSX),True)
	BUILD_PPC   = False
	BUILD_PPC64 = False
endif

# Enter object files here:
OBJECTS	  = config.o converter.o layer.o scriptutilities.o video.o videolist.o videolistentry.o videosite.o youtube.o

# Enter additional defines here:
DEFINE	  = -DV8_USE_UNSAFE_HANDLES

# Enter additional library dependencies here:
ifeq ($(BUILD_WIN32),True)
	LIBS	  = -lsmooth-js
else
	LIBS	  = -lsmooth-js-$(SMOOTHVER)
endif

# Enter addition commands for targets all and clean here:
ALLCMD1   =
ALLCMD2   =
CLEANCMD1 =
CLEANCMD2 =
INSTCMD1  = cp -r "$(SRCDIR)"/freac.extension.youtube "$(DESTDIR)"$(libdir)/freac
INSTCMD2  = chmod -R a=rX,u=rwX "$(DESTDIR)"$(libdir)/freac/freac.extension.youtube
UINSTCMD1 = rm -f -r "$(DESTDIR)"$(libdir)/freac/freac.extension.youtube
UINSTCMD2 =

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(FREAC_PATH)/Makefile-commands
