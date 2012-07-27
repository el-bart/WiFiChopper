ifeq (,$(filter $(GEN_BASE_DIR)/%,$(basename $(CURDIR))))
# part invoked in source dir

# this will be application
COMPONENT_NAME:=$(shell basename $(CURDIR))
TEST_PROGRAM_NAME:=$(COMPONENT_NAME).t
export COMPONENT_NAME \
       TEST_PROGRAM_NAME

# look for sources in this lib
include $(MAKEFILES_COMMON_BASE_DIR)/source_finder.mk

include $(MAKEFILES_COMMON_BASE_DIR)/targets.mk

else
# part invoked in gen dir

# include user_pre makefile, if it exists
-include $(THIS_SRC_BASE_DIR)/user_pre.mk

include $(MAKEFILES_COMMON_BASE_DIR)/$(COMPONENT_TYPE).mk

# add ourselves to VPATH
VPATH+=$(THIS_SRC_BASE_DIR)

include $(MAKEFILES_COMMON_BASE_DIR)/build_macros.mk
include $(MAKEFILES_COMMON_BASE_DIR)/rules.mk
include $(MAKEFILES_COMMON_BASE_DIR)/deps.mk

# include user_post makefile, if it exists
-include $(THIS_SRC_BASE_DIR)/user_post.mk

endif
