# has C sources?
ifneq (,$(CDEPS))
-include $(CDEPS)
endif

# has C++ sources?
ifneq (,$(CXXDEPS))
-include $(CXXDEPS)
endif

$(PUBLIC_HEADERS): $(ALL_HEADERS)
	@echo "MKPUB $(COMPONENT_NAME)"
	@cd "$(THIS_SRC_BASE_DIR)" && \
		"$(SCRIPTS_BASE_DIR)/link_public_headers" "$(GEN_INCLUDES_DIR)"
	@echo "$^" > "$@"
