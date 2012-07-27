# remember base directory for sources of current lib/app
THIS_SRC_BASE_DIR:=$(CURDIR)
export THIS_SRC_BASE_DIR

include $(MAKEFILES_BASE_DIR)/common/dirs_features.mk


#
# find source files in all sub-directories
#

# find all sources selected by MODE
MODE_EXPRESSION :=$(shell cd "$(THIS_SRC_FEATURES_MODES_DIR)" && cat $(MODE)) \
                  $(shell [ -d "$(THIS_SRC_FEATURES_SPLIT_DIR)" ] && cd "$(THIS_SRC_FEATURES_SPLIT_DIR)" && cat /dev/null $(FEATURES) 2>/dev/null | xargs echo )
ALL_MODE_SOURCES:=$(wildcard $(MODE_EXPRESSION))
export ALL_MODE_SOURCES

ALL_SOURCES_C   :=$(filter %.c,   $(ALL_MODE_SOURCES))
ALL_SOURCES_CXX :=$(filter %.cpp, $(ALL_MODE_SOURCES))
ALL_SOURCES     :=$(ALL_SOURCES_C) $(ALL_SOURCES_CXX)
export ALL_SOURCES \
       ALL_SOURCES_C \
       ALL_SOURCES_CXX

ALL_HEADERS_C   :=$(filter %.h,   $(ALL_MODE_SOURCES))
ALL_HEADERS_CXX :=$(filter %.hpp, $(ALL_MODE_SOURCES))
ALL_HEADERS     :=$(ALL_HEADERS_C) $(ALL_HEADERS_CXX)
export ALL_HEADERS \
       ALL_HEADERS_C \
       ALL_HEADERS_CXX

CSRCS_TEST  :=$(filter %.t.c,   $(ALL_SOURCES_C)  )
CXXSRCS_TEST:=$(filter %.t.cpp, $(ALL_SOURCES_CXX))
export CSRCS_TEST \
       CXXSRCS_TEST

CSRCS_MTEST  :=$(filter %.mt.c,   $(ALL_SOURCES_C)  )
CXXSRCS_MTEST:=$(filter %.mt.cpp, $(ALL_SOURCES_CXX))
export CSRCS_MTEST \
       CXXSRCS_MTEST

CSRCS  :=$(filter-out $(CSRCS_TEST)   $(CSRCS_MTEST),   $(ALL_SOURCES_C)  )
CXXSRCS:=$(filter-out $(CXXSRCS_TEST) $(CXXSRCS_MTEST), $(ALL_SOURCES_CXX))
export CSRCS \
       CXXSRCS

ifneq (,$(CSRCS))
CSRCS_NOMAIN  :=$(shell grep -L '^int main' $(CSRCS)   )
endif # ALL_SOURCES_C
ifneq (,$(CXXSRCS))
CXXSRCS_NOMAIN:=$(shell grep -L '^int main' $(CXXSRCS) )
endif # ALL_SOURCES_CXX
export CSRCS_NOMAIN \
       CXXSRCS_NOMAIN

CSRCS_MAIN  :=$(filter-out $(CSRCS_NOMAIN),   $(CSRCS)  )
CXXSRCS_MAIN:=$(filter-out $(CXXSRCS_NOMAIN), $(CXXSRCS))

# directories containing source files
SOURCE_DIRS:=$(shell { for f in $(ALL_SOURCES) ; \
					  do dirname $$f; done; } | sort | uniq )
export SOURCE_DIRS


#
# make object names from source files
#

COBJS  :=$(CSRCS:.c=.o)
CXXOBJS:=$(CXXSRCS:.cpp=.oxx)
export COBJS \
       CXXOBJS

# obj files without 'main()':
COBJS_NOMAIN  :=$(CSRCS_NOMAIN:.c=.o)
CXXOBJS_NOMAIN:=$(CXXSRCS_NOMAIN:.cpp=.oxx)
export COBJS_NOMAIN \
       CXXOBJS_NOMAIN

# obj files containing 'main()' (there should by just one BTW...)
COBJS_MAIN  :=$(CSRCS_MAIN:.c=.o)
CXXOBJS_MAIN:=$(CXXSRCS_MAIN:.cpp=.oxx)
export COBJS_MAIN \
       CXXOBJS_MAIN

# test object files:
COBJS_TEST  :=$(CSRCS_TEST:.c=.o)
CXXOBJS_TEST:=$(CXXSRCS_TEST:.cpp=.oxx)
export COBJS_TEST \
       CXXOBJS_TEST

# mtest object files:
COBJS_MTEST  :=$(CSRCS_MTEST:.c=.o)
CXXOBJS_MTEST:=$(CXXSRCS_MTEST:.cpp=.oxx)
export COBJS_MTEST \
       CXXOBJS_MTEST

# mtest output files:
CBIN_MTEST  :=$(CSRCS_MTEST:.c=)
CXXBIN_MTEST:=$(CXXSRCS_MTEST:.cpp=)
export CBIN_MTEST \
       CXXBIN_MTEST

# dep files:
CDEPS  :=$(ALL_SOURCES_C:.c=.o.d)
CXXDEPS:=$(ALL_SOURCES_CXX:.cpp=.oxx.dxx)
export CDEPS \
       CXXDEPS
