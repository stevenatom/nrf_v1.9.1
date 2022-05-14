COMPONENT_NAME=memfault_sdk_regions

SRC_FILES = \
  $(MFLT_COMPONENTS_DIR)/panics/src/memfault_coredump_sdk_regions.c

TEST_SRC_FILES = \
  $(MFLT_TEST_SRC_DIR)/test_memfault_coredump_sdk_regions.cpp

CPPUTEST_CPPFLAGS += -DMEMFAULT_COREDUMP_COLLECT_LOG_REGIONS=1
CPPUTEST_CPPFLAGS += -DMEMFAULT_COREDUMP_COLLECT_HEAP_STATS=1

include $(CPPUTEST_MAKFILE_INFRA)
