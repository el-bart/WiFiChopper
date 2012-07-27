.PHONY: debug
debug: CXXFLAGS+=$(DBG_FLAGS)
debug: CFLAGS  +=$(DBG_FLAGS)
debug: LDFLAGS +=$(DBG_LDFLAGS)
debug: TARGET=all
debug:
	$(run-in-gen)
