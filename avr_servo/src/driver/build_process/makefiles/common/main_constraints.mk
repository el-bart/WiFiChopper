#
# is any profile set?
#
PROFILES_LIST:=$(shell cd "$(MAKEFILES_PROFILES_BASE_DIR)" && ls | sed 's:\.mk::g' | xargs echo)
ifneq ($(PROFILE),$(findstring $(PROFILE),$(PROFILES_LIST)))
$(error PROFILE not set (aviable are: $(PROFILES_LIST)))
endif # PROFILE not set

#
# is any toolchain set?
#
TOOLCHAINS_LIST:=$(shell cd "$(MAKEFILES_TOOLCHAINS_BASE_DIR)" && ls | xargs echo -n)
ifneq ($(TC),$(findstring $(TC),$(TOOLCHAINS_LIST)))
$(error toolchain (TC) not set (aviable are: $(TOOLCHAINS_LIST)))
endif # TC not set

#
# is any mode set?
#
ifeq (,$(MODE))
$(error mode (MODE) not set)
endif # TC not set

#
# does given features exist?
#
ALL_EXISTING_FEATURES_LIST:=$(shell for c in `cat $(COMPONENTS_LISTS) 2>/dev/null` ; do ls "$$c/features/modes/features" 2>/dev/null ; done )
NON_EXISTING_FEATURES_LIST:=$(filter-out $(ALL_EXISTING_FEATURES_LIST),$(FEATURES))
ifneq ($(NON_EXISTING_FEATURES_LIST),)
$(error following features does NOT exist: $(NON_EXISTING_FEATURES_LIST) (NOTE: available are: $(ALL_EXISTING_FEATURES_LIST)))
endif

#
# are there any toolchain-specific limitations?
#
-include $(MAKEFILES_TOOLCHAINS_BASE_DIR)/$(TC)/constraints.mk
