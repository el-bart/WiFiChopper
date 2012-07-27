# note: public_headers are NOT needed for applications

.PHONY: all
all:: all-$(COMPONENT_PART)

include $(MAKEFILES_COMMON_BASE_DIR)/lib_app_common.mk

MAIN_OBJS:=$(COBJS_MAIN) $(CXXOBJS_MAIN)
$(APPLICATION_NAME):: $(LIBRARY_NAME) $(MAIN_OBJS) $(LIBS_GEN_DEPS)
	@echo "LD    $@"
	$(LD) $(LDFLAGS) -o $@ $(MAIN_OBJS) $(FORCE_LINK_SYMBOLS) \
		-l$(COMPONENT_NAME) $(LINK_LIBS) $(END_LINK_LIBS)
	if test -n "$(STRIP_BINARY)" ; \
	then \
	  echo "STRIP $@" ; \
	  $(STRIP) $@ || rm -f "$@" ; \
	fi

#
# ALL
#
.PHONY: all-MKPUBLIC
all-MKPUBLIC::
.PHONY: all-OBJECTS
all-OBJECTS:: $(LIBRARY_OBJ_DEPS) $(MAIN_OBJS)
.PHONY: all-LINK
all-LINK:: $(APPLICATION_NAME)
