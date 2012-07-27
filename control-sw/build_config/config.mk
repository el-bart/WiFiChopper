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
export FEATURES:=

# project-specific flags
export USER_OPT_FLAGS:=
export USER_DBG_FLAGS:=
export USER_PRF_FLAGS:=
# project-specific link flags
export USER_OPT_LDFLAGS:=
export USER_DBG_LDFLAGS:=
export USER_PRF_LDFLAGS:=
