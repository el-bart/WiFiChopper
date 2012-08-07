.PHONY: test
test:: test-$(COMPONENT_PART)

.PHONY: mtest
mtest:: mtest-$(COMPONENT_PART)

.PHONY: doc
doc:: doc-$(COMPONENT_PART)


html/index.html:: Doxyfile $(ALL_MODE_SOURCES)
	@echo "DOXY  $(COMPONENT_NAME)/makedoc"
	@if $(DOXYGEN) 2>&1 | grep '' 1>&2 ; then rm -f "$@" ; false ; else true ; fi

Doxyfile:
	@echo "DOXY  $(COMPONENT_NAME)/$@"
	rm -f "$@"
	$(DOXYGEN) -g "$@.def" > /dev/null
	sed -e "s:^\(INPUT \+=\).*:\1 $(THIS_SRC_BASE_DIR):" \
		-e "s:^\(RECURSIVE \+=\).*:\1 YES:" \
		-e "s:^\(EXCLUDE_PATTERNS \+=\).*:\1 *.mt.?pp *.t.?pp:" \
		-e "s:^\(SHOW_DIRECTORIES \+=\).*:\1 YES:" \
		-e "s:^\(QUIET \+=\).*:\1 YES:" \
		-e "s:^\(WARN_NO_PARAMDOC \+=\).*:\1 YES:" \
		-e "s:^\(INCLUDE_PATH \+=\).*:\1 $(THIS_SRC_BASE_DIR) $(GEN_INCLUDES_DIR):" \
		-e "s:^\(HAVE_DOT \+=\).*:\1 YES:" \
		-e "s:^\(UML_LOOK \+=\).*:\1 YES:" \
		-e "s:^\(GENERATE_LATEX \+=\).*:\1 NO:" \
		"$@.def" > "$@.mod"
	mv "$@.mod" "$@"

LIBS_GEN_DEPS:=$(wildcard $(DEP_LIBS_WC)) $(GEN_LIBS_DIR)/$(LIBRARY_NAME)

%.mt: %.mt.oxx $(LIBS_GEN_DEPS) $(LIBRARY_NAME)
	@echo "LD    $@"
	$(LD) $(LDFLAGS) -o $@ $(FORCE_LINK_SYMBOLS) $@.oxx -l$(COMPONENT_NAME) $(LINK_LIBS) $(END_LINK_LIBS)

$(CBIN_MTEST)::
	@echo "mtest taget is not implemented for C sources"
	@false


.PHONY: copy_testdata
copy_testdata:
	cp -purv "$(THIS_SRC_FEATURES_TESTDATA_DIR)" . 2>/dev/null | \
	  sed 's:^.* -> ...\(.*\).$$:COPY  \1:'

$(TEST_PROGRAM_NAME):: $(CXXOBJS_TEST) $(COBJS_TEST) $(LIBS_GEN_DEPS) $(LIBRARY_NAME)
	@echo "LD    $@"
	$(LD) $(LDFLAGS) -o $@ $(CXXOBJS_TEST) $(COBJS_TEST) $(FORCE_LINK_SYMBOLS) -l$(COMPONENT_NAME) \
		$(LINK_LIBS) $(END_LINK_LIBS)

LIBRARY_OBJ_DEPS:=$(CXXOBJS_NOMAIN) $(COBJS_NOMAIN)
LIBRARY_DEPS    :=$(LIBRARY_OBJ_DEPS) $(GEN_LIBS_DIR)/$(LIBRARY_NAME) $(LIBS_GEN_DEPS) $(DEP_WLD_LINK)
ifeq (static,$(LIBRARY_TYPE))
$(LIBRARY_NAME):: $(LIBRARY_DEPS)
	@echo "AR    $@"
	rm -f $@
	$(AR) -r $@ $(LIBRARY_OBJ_DEPS)
endif
ifeq (dynamic,$(LIBRARY_TYPE))
CXXFLAGS+=-fPIC
CFLAGS  +=-fPIC
LDFLAGS +=-fPIC
$(LIBRARY_NAME):: LDFLAGS+=-shared
$(LIBRARY_NAME):: $(LIBRARY_DEPS)
	@echo "LD    $@"
	$(LD) $(LDFLAGS) -o $@ $(LIBRARY_OBJ_DEPS) $(LINK_LIBS) $(END_LINK_LIBS)
endif

# ensure there is proper link in gen
$(GEN_LIBS_DIR)/$(LIBRARY_NAME):
	ln -sf "$(CURDIR)/$(LIBRARY_NAME)" "$(GEN_LIBS_DIR)"


#
# TEST
#
.PHONY: test-MKPUBLIC
test-MKPUBLIC:: $(PUBLIC_HEADERS)
.PHONY: test-OBJECTS
test-OBJECTS:: $(LIBRARY_OBJ_DEPS) $(CXXOBJS_TEST) $(COBJS_TEST)
.PHONY: test-LINK
test-LINK:: copy_testdata
test-LINK:: $(LIBRARY_NAME)
test-LINK:: $(TEST_PROGRAM_NAME)

#
# MTEST
#
.PHONY: mtest-MKPUBLIC
mtest-MKPUBLIC:: $(PUBLIC_HEADERS)
.PHONY: mtest-OBJECTS
mtest-OBJECTS:: $(LIBRARY_OBJ_DEPS) $(CXXOBJS_MTEST) $(COBJS_MTEST)
.PHONY: mtest-LINK
mtest-LINK:: copy_testdata
mtest-LINK:: $(LIBRARY_NAME)
mtest-LINK:: $(CXXBIN_MTEST) $(CBIN_MTEST)

#
# DOC
#
.PHONY: doc-MKPUBLIC
doc-MKPUBLIC:: $(PUBLIC_HEADERS)
.PHONY: doc-LINK
doc-LINK:: html/index.html
.PHONY: doc-OBJECTS
doc-OBJECTS::
