.PHONY: release
release: CXXFLAGS+=$(OPT_FLAGS)
release: CFLAGS  +=$(OPT_FLAGS)
release: LDFLAGS +=$(OPT_LDFLAGS)
release: TARGET=all
release: STRIP_BINARY=1
release:
	$(run-in-gen)
