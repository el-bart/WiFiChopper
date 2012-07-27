# include all required features
include $(MAKEFILES_COMMON_BASE_DIR)/version_check.mk

# force doing some extra stuff before building starts
.PHONY: pre_build
pre_build: $(BUILD_PROCESS_VERSION_TIMESTAMP) $(GEN_WORK_ROOT)/features_build_list.txt $(GEN_WORK_ROOT_SHORT)

# write down list of features buing build in given directory
$(GEN_WORK_ROOT)/features_build_list.txt:
	@echo "$(FEATURES)" | xargs -n 1 echo | sort | uniq > "$@"

# link result of last build to give it short name
$(GEN_WORK_ROOT_SHORT): $(GEN_WORK_ROOT)/..
	@rm -f $(GEN_WORK_ROOT_SHORT)
	@ln -s $(GEN_WORK_ROOT) $(GEN_WORK_ROOT_SHORT)
