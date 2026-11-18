########## fre:ac component makefile ##########

# Change these variables to fit your project:
TARGET	  = cddb
TYPE	  = metadata

FREAC_PATH = ../../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(FREAC_PATH)/Makefile-options

# Enter object files here:
OBJECTS	  = cddb.o cddbbatch.o cddbcache.o cddbconfig.o cddbinfo.o cddblocal.o cddbremote.o config.o convert.o metadatacddb.o
OBJECTS	 += dialogs/extsettings.o dialogs/manage.o dialogs/managesubmits.o

# Enter additional defines here:
DEFINE	  =

# Enter additional library dependencies here:
LIBS	  =

# Enter addition commands for targets all and clean here:
ifneq ("$(SRCDIR)","$(CURDIR)")
	ALLCMD1   = mkdir -p dialogs
	ALLCMD2   =
	CLEANCMD1 = rmdir dialogs || true
	CLEANCMD2 =
	INSTCMD1  =
	INSTCMD2  = 
	UINSTCMD1 =
	UINSTCMD2 =
endif

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(FREAC_PATH)/Makefile-commands
