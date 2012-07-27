.PHONY: profile
profile: CXXFLAGS+=$(PRF_FLAGS)
profile: CFLAGS  +=$(PRF_FLAGS)
profile: LDFLAGS +=$(PRF_LDFLAGS)
profile: TARGET=all
profile:
	$(run-in-gen)
