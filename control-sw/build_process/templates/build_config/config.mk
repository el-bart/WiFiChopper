#
# project specific configuration file
# to be placed in main project directory
#

# set default mode
export MODE:=default

# set default profile
export PROFILE:=test

# set default toolchain
export TC:=gcc

# enable distcc
#export WITH_DISTCC:=1

# enable ccache
export WITH_CCACHE:=1

# default features set
export FEATURES:= \
    ans2 \
    ft1

# project-specific flags
export USER_OPT_FLAGS:=-DSOME_PROJECT_SPECIFIC_OPTION
export USER_DBG_FLAGS:=-DSOME_PROJECT_SPECIFIC_OPTION
export USER_PRF_FLAGS:=-DSOME_PROJECT_SPECIFIC_OPTION
# project-specific link flags
export USER_OPT_LDFLAGS:=-lstdc++
export USER_DBG_LDFLAGS:=-lstdc++
export USER_PRF_LDFLAGS:=-lstdc++
