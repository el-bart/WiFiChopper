include $(MAKEFILES_TOOLCHAINS_BASE_DIR)/$(TC)/tools.mk

# check version of this toolchain
INTEL_VERSION:=$(shell $(CC) -dumpversion | sed -e 's:\.: * 10000 + 100 * :' -e 's:\.: + :' | xargs expr)

# check gcc version (icc uses its libs)
GCC_VER:=$(shell g++ -dumpversion)

#
# show warning when using ccache with old intel compiler
#
ifneq (,$(WITH_CCACHE))
# this warning is true only for ICC releaseses < 12.0.0
ifeq (oops,$(shell test $(INTEL_VERSION) -lt 120000 && echo 'oops'))
 $(shell echo "-----------------------------------------------------------------------------------" >&2)
 $(shell echo "                 USING INTEL's TOOLCHAIN WITH CCACHE ENABLED"                        >&2)
 $(shell echo "WARNING: Intel's compiler in version before 12.0.0 is broken - it does not work"     >&2)
 $(shell echo "WARNING: well with ccache enabled. your should disable it in build_config/config.mk" >&2)
 $(shell echo "WARNING: file or upgrade to version 12.0.0 or newer. otherwise you might get"        >&2)
 $(shell echo "WARNING: strange errors from make, about being unable to generate dependency for"    >&2)
 $(shell echo "WARNING: */.ccache/*.ii files, etc..."                                               >&2)
 $(shell echo "WARNING: you have been warned."                                                      >&2)
 $(shell echo "-----------------------------------------------------------------------------------" >&2)
endif # too old (i.e. buggy) icc version
endif # with ccache

#
# warn about GCC 4.5/C++0x libs for intel (required when GCC>=4.5 is default one)
#
ifeq (oops,$(shell test $(INTEL_VERSION) -lt 120004 && echo 'oops'))
# this warning is true only for GCC releaseses > 4.4
ifeq (oops,$(shell echo '$(GCC_VER)' | grep -q '^4\.[56789]' && echo 'oops'))
$(shell echo "--------------------------------------------------------------------------------------------------" >&2)
$(shell echo "         COMPILING WITH ICC<12.0.4, USING GCC>=4.5 LIBS - THIS MAY LEAD TO PROBLEMS"                >&2)
$(shell echo "WARNING: gcc 4.5 is (as for now) incompatible with icc, since it uses implicit c++0x in"            >&2)
$(shell echo "WARNING: header files (intel does not make such an assumption). program probably will not compile." >&2)
$(shell echo "WARNING: to fix this issue try installing gcc 4.4 or newer icc."                                    >&2)
$(shell echo "WARNING: you may want to use gcc 4.4 libs instead (via -I in config.mk)"                            >&2)
$(shell echo "--------------------------------------------------------------------------------------------------" >&2)
endif # gcc>=4.5
endif # intel<12.0.4
