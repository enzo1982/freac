########## fre:ac directory makefile ##########

FREAC_PATH = ../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(FREAC_PATH)/Makefile-options

FOLDERS = donate langnotify protocols tagedit

ifeq ($(USE_V8),True)
	FOLDERS += youtube
endif

.PHONY: $(FOLDERS)

all: $(FOLDERS)

$(FOLDERS):
	+ $(call makein,$@)

clean: $(foreach FOLDER,$(FOLDERS),$(FOLDER)##clean)

$(foreach FOLDER,$(FOLDERS),$(FOLDER)##clean):
	$(call cleanin,$(subst ##clean,,$@))

install: $(foreach FOLDER,$(FOLDERS),$(FOLDER)##install)

$(foreach FOLDER,$(FOLDERS),$(FOLDER)##install):
	$(call makein,$(subst ##install,,$@),install)

uninstall: $(foreach FOLDER,$(FOLDERS),$(FOLDER)##uninstall)

$(foreach FOLDER,$(FOLDERS),$(FOLDER)##uninstall):
	$(call makein,$(subst ##uninstall,,$@),uninstall)
