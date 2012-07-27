#
# NOTES
# -Qunused-arguments suppresses warning on unused compilation flags - should not bee needed in the future.
# -Werror            should be enabled for the release mode as soon as clang will be more standard-compliant.
# -O3                though clang supports -O4 as the more aggressive optimisations it does not seems to work.
#

# compiler flags
CMN_FLAGS:=-Wall -Wextra -pipe -Qunused-arguments -I/usr/include/$(shell gcc -dumpmachine)
#OPT_FLAGS:=-O3 -DNDEBUG -Werror -march=native
OPT_FLAGS:=-O3 -DNDEBUG -march=native
DBG_FLAGS:=-g3
PRF_FLAGS:=-g $(OPT_FLAGS) -pg

# linker flags
CMN_LDFLAGS:=-rdynamic
DBG_LDFLAGS:=
OPT_LDFLAGS:=
PRF_LDFLAGS:=$(OPT_LDFLAGS) -pg
