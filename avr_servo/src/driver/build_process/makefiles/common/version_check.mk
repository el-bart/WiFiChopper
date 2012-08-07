BUILD_PROCESS_VERSION_TIMESTAMP:=$(GEN_BASE_DIR)/build_process_version_check.ts

# this code checks if build process' files didn't changed since initial
# build, and if so, inform user that (s)he should rebuild software
$(BUILD_PROCESS_VERSION_TIMESTAMP): $(wildcard $(BUILD_CONFIG_BASE_DIR)/*.mk)
$(BUILD_PROCESS_VERSION_TIMESTAMP): $(wildcard $(CODE_BASE_DIR)/*)
$(BUILD_PROCESS_VERSION_TIMESTAMP): $(wildcard $(CODE_BASE_DIR)/*/*)
$(BUILD_PROCESS_VERSION_TIMESTAMP): $(wildcard $(MAKEFILES_BASE_DIR)/*.mk)
$(BUILD_PROCESS_VERSION_TIMESTAMP): $(wildcard $(MAKEFILES_BASE_DIR)/basic_mks/*)
$(BUILD_PROCESS_VERSION_TIMESTAMP): $(wildcard $(MAKEFILES_COMMON_BASE_DIR)/*.mk)
$(BUILD_PROCESS_VERSION_TIMESTAMP): $(wildcard $(MAKEFILES_PROFILES_BASE_DIR)/*.mk)
$(BUILD_PROCESS_VERSION_TIMESTAMP): $(wildcard $(MAKEFILES_COMPONENT_MODS_DIR)/*.mk)
$(BUILD_PROCESS_VERSION_TIMESTAMP): $(wildcard $(MAKEFILES_TOOLCHAINS_BASE_DIR)/*/*.mk)
$(BUILD_PROCESS_VERSION_TIMESTAMP): $(wildcard $(SCRIPTS_BASE_DIR)/*)
	@if test -f "$@" ; \
		then \
			echo "--------------------------------------------------------------------------" >&2 ; \
			echo "WARNING: BUILD PROCESS VERSION CHANGED SINCE LAST BUILD" >&2 ;\
			echo "WARNING: you're **STORNGLY** advised to remove current gen directory from:" >&2 ; \
			echo "WARNING: $(GEN_BASE_DIR)" >&2 ; \
			echo "" >&2 ; \
			echo "if you understand what has happened and you are SURE this is false alarm" >&2 ; \
			echo "you can supprese this message by removing following file:" >&2 ; \
			echo "$@" >&2 ; \
			echo "" >&2 ; \
			echo "note: this message may inform on change in compilation flags as well." >&2 ; \
			echo "--------------------------------------------------------------------------" >&2 ; \
			false ; \
		else \
			echo "dependent files are:"         >  "$@" ; \
			echo "$^" | sed 's: \+:\n:g' | sort >> "$@" ; \
		fi
