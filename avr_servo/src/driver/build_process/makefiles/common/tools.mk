# include proper toolchain
include $(MAKEFILES_TOOLCHAINS_BASE_DIR)/$(TC)/tools.mk

# distcc support, if requested
ifneq (,$(WITH_DISTCC))
  DISTCC:=distcc
  CC    :=$(DISTCC) $(CC)
  CXX   :=$(DISTCC) $(CXX)
endif

# ccache support, if requested
ifneq (,$(WITH_CCACHE))
  CCACHE:=ccache
  CC    :=$(CCACHE) $(CC)
  CXX   :=$(CCACHE) $(CXX)
endif

export CC \
       CXX \
       AR \
       RANLIB \
       NM \
       LD \
       STRIP \
       DOXYGEN
