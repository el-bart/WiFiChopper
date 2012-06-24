#
# if 'release' profile is in use warn about -Werror disable
#
ifeq ($(PROFILE),release)
$(shell echo "------------------------------------------------------------------------------------" >&2)
$(shell echo "              FLAG -Werror DISABLED FOR CLANG IN RELEASE PROFILE"                     >&2)
$(shell echo "WARNING: CLANG does not properly parse some valid C++ constructions, thus stopping"   >&2)
$(shell echo "WARNING: compilation on warning does not make sense. having this said '-Werror' flag" >&2)
$(shell echo "WARNING: is disabled for now (i.e. until CLANG is fixed)."                            >&2)
$(shell echo "------------------------------------------------------------------------------------" >&2)
endif # release

#
# stop build for 'profile' target, that is not supported by this toolchain
#
ifeq ($(PROFILE),profile)
$(error CLANG currently does NOT support profiling)
endif # profile
