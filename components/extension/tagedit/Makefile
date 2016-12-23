########## fre:ac component makefile ##########

# Change these variables to fit your project:
TARGET	  = tagedit
TYPE	  = extension
VERSION	  = 1.0

FREAC_PATH = ../../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(FREAC_PATH)/Makefile-options

# Enter object files here:
OBJECTS	  = chooser/chooser.o chooser/chooser_albums.o chooser/chooser_files.o chooser/chooser_files_tree.o chooser/chooser_files_utilities.o chooser/chooser_tracks.o editor/editor.o editor/editor_advanced_number.o editor/editor_advanced_text.o editor/editor_advanced.o editor/editor_basic.o editor/editor_basic_cover.o editor/editor_details.o editor/editor_other.o layer.o tagedit.o tagspec.o utilities.o

# Enter additional defines here:
DEFINE	  =

# Enter additional library dependencies here:
LIBS	  =

# Enter addition commands for targets all and clean here:
ifneq ($(SRCDIR),$(CURDIR))
	ALLCMD1   = mkdir -p chooser editor
	ALLCMD2   =
	CLEANCMD1 = rmdir chooser editor || true
	CLEANCMD2 =
	INSTCMD1  =
	INSTCMD2  = 
	UINSTCMD1 =
	UINSTCMD2 =
endif

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(FREAC_PATH)/Makefile-commands
