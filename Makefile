ROOT_EXE ?= $(shell which root.exe)
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
dict:: $(DICT_MAKEFILE_DIR)
$(DICT_MAKEFILE_DIR)::
	@$(MAKE) -C $@

.PHONY: write
write:: $(WRITE_C)
$(WRITE_C)::
	@LD_LIBRARY_PATH=$(shell dirname $@) $(ROOT_EXE) -q -l $@

.PHONY: read
read:: $(READ_C)
$(READ_C)::
	@LD_LIBRARY_PATH=$(shell dirname $@) $(ROOT_EXE) -q -l $@
