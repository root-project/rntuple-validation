ROOT_EXE := $(shell which root.exe)
ifeq ($(ROOT_EXE),)
$(error Could not find root.exe)
endif

.PHONY: all
all:
	$(MAKE) dict
	$(MAKE) write
	$(MAKE) read

# This assumes there is no whitespace in any of the paths...
DICT_MAKEFILE_DIR := $(sort $(shell find */ -name Makefile -printf "%h\n"))
WRITE_C := $(sort $(shell find . -name write.C))
READ_C := $(sort $(shell find . -name read.C))

.PHONY: dict
dict:
	@$(foreach d,$(DICT_MAKEFILE_DIR),make -C $(d) &&) true

.PHONY: write
write:
	@$(foreach c,$(WRITE_C),LD_LIBRARY_PATH=$(shell dirname $(c)) $(ROOT_EXE) -q -l $(c) &&) true

.PHONY: read
read:
	@$(foreach c,$(READ_C),LD_LIBRARY_PATH=$(shell dirname $(c)) $(ROOT_EXE) -q -l $(c) &&) true
