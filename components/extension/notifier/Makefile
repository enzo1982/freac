########## fre:ac component makefile ##########

# Change these variables to fit your project:
TARGET	  = notifier
TYPE	  = extension
VERSION	  = 1.0

FREAC_PATH = ../../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(FREAC_PATH)/Makefile-options

# Enter object files here:
OBJECTS	  = config.o notifier.o player.o

# Enter additional defines here:
DEFINE	  =

# Enter additional library dependencies here:
LIBS	  =

# Enter addition commands for targets all and clean here:
ALLCMD1   =
ALLCMD2   =
CLEANCMD1 =
CLEANCMD2 =
INSTCMD1  = cp -r $(SRCDIR)/freac.extension.notifier $(DESTDIR)$(libdir)/freac
INSTCMD2  = chmod -R a=rX,u=rwX $(DESTDIR)$(libdir)/freac/freac.extension.notifier
UINSTCMD1 = rm -f -r $(DESTDIR)$(libdir)/freac/freac.extension.notifier
UINSTCMD2 =

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(FREAC_PATH)/Makefile-commands
