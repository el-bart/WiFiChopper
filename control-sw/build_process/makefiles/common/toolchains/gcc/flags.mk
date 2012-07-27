# compiler flags
CMN_FLAGS:=-Wall -Wextra -pipe
OPT_FLAGS:=-O3 -DNDEBUG -Werror -march=native
DBG_FLAGS:=-g3
PRF_FLAGS:=-g $(OPT_FLAGS) -pg

# linker flags
CMN_LDFLAGS:=-rdynamic
DBG_LDFLAGS:=
OPT_LDFLAGS:=
PRF_LDFLAGS:=$(OPT_LDFLAGS) -pg
