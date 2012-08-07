.PHONY: test
test: CXXFLAGS+=$(DBG_FLAGS)
test: CFLAGS  +=$(DBG_FLAGS)
test: LDFLAGS +=$(DBG_LDFLAGS)
test: TARGET=test
test:
	$(run-in-gen)
