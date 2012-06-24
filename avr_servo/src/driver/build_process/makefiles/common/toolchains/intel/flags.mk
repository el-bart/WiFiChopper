# compiler flags
CMN_FLAGS:=-Wall -pipe -I/usr/include/$(shell $(CC) -dumpmachine)
OPT_FLAGS:=-O3 -DNDEBUG -Werror -xhost
DBG_FLAGS:=-g3
PRF_FLAGS:=-g $(OPT_FLAGS) -pg

# linker flags
CMN_LDFLAGS:=-rdynamic
DBG_LDFLAGS:=
OPT_LDFLAGS:=
PRF_LDFLAGS:=$(OPT_LDFLAGS) -pg
