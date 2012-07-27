.PHONY: mtest
mtest: CXXFLAGS+=$(DBG_FLAGS)
mtest: CFLAGS  +=$(DBG_FLAGS)
mtest: LDFLAGS +=$(DBG_LDFLAGS)
mtest: TARGET=mtest
mtest:
	$(run-in-gen)
