.PHONY: doc
doc: TARGET=doc
doc:
	@mkdir -p "$(GEN_INCLUDES_DIR)" # suppress warning from doxygen
	$(run-in-gen)
