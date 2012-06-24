# makro for common calls to build choosen component.
define build-this-component

	# build given component
	+$(MAKE) $(MFLAGS) -C "$(COMPONENT_DIR)" \
		LINK_LIBS="$(LINK_LIBS)" \
		DEP_LIBS_WC="$(DEP_LIBS_WC)" \
		COMPONENT_PART="$(COMPONENT_PART)" \
		DEP_WLD_COMPILE="$(DEP_WLD_COMPILE)" \
		DEP_WLD_LINK="$(DEP_WLD_LINK)" \
		"$(PROFILE)"

endef
#
# automatization of dependencies generation
#
# interlib dependencies finder
MAKE_COMPONENTDEPS_SCRIPT:=$(SCRIPTS_BASE_DIR)/componentdeps
# this is relative path to deps of each component
COMPONENT_DEPS_REL_PATH=$(SRC_FEATURES_DEPS_FULL_SURFIX)/$(PROFILE)
# shell expanding variable to find deps
FIND_DEPS_FOR_THIS_COMPONENT=$(shell \
    $(GETDEPS_SCRIPT) $(THIS_COMPONENT) $(COMPONENT_DEPS_REL_PATH))
