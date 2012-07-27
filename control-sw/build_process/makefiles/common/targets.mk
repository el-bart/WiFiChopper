# makro for common calls
define run-in-gen
	# make gen/ dir structure for source files
	GEN_NOW="$(GEN_WORK_ROOT)/`basename $(CURDIR)`" && \
		mkdir -p "$$GEN_NOW" && cd "$$GEN_NOW" && \
		mkdir -p . $(SOURCE_DIRS)
	# make gen dir
	mkdir -p "$(GEN_LIBS_DIR)"
	# run
	+$(MAKE) $(MFLAGS) -C "$(GEN_WORK_ROOT)/`basename $(CURDIR)`" \
		-f "$(CURDIR)/Makefile" TARGET=$@ $(TARGET) \
		STRIP_BINARY=$(STRIP_BINARY) \
		DEP_LIBS_WC="$(DEP_LIBS_WC)" \
		END_LINK_LIBS="$(END_LINK_LIBS)"
endef

# inside lib/app we can include any header (not only public ones)
CXXFLAGS+=-I$(CURDIR)
CFLAGS  +=-I$(CURDIR)

# external variables are passed by upper make

# this should not happen
.PHONY: all
all:
	@echo
	@echo 'error - no correct PROFILE has been specified'
	@echo
	@exit 1

# include toolchain-specific flags
include $(MAKEFILES_TOOLCHAINS_BASE_DIR)/$(TC)/flags.mk
# add project-specific flags
OPT_FLAGS+=$(USER_OPT_FLAGS)
DBG_FLAGS+=$(USER_DBG_FLAGS)
PRF_FLAGS+=$(USER_PRF_FLAGS)
# add project-specific link flags
OPT_LDFLAGS+=$(USER_OPT_LDFLAGS)
DBG_LDFLAGS+=$(USER_DBG_LDFLAGS)
PRF_LDFLAGS+=$(USER_PRF_LDFLAGS)
# add common flags, for all profiles
CXXFLAGS+=$(CMN_FLAGS)
CFLAGS  +=$(CMN_FLAGS)
LDFLAGS +=$(CMN_LDFLAGS)

# include profiles from extrnal files
include $(wildcard $(MAKEFILES_PROFILES_BASE_DIR)/*.mk)
