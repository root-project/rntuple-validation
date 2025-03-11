ROOT_EXE := $(shell which root.exe)
ifeq ($(ROOT_EXE),)
$(error Could not find root.exe)
endif

.PHONY: all
all:
	$(MAKE) write
	$(MAKE) read

# This assumes there is no whitespace in any of the paths...
WRITE_C := $(sort $(shell find . -name write.C))
READ_C := $(sort $(shell find . -name read.C))

.PHONY: write
write:
	@$(foreach c,$(WRITE_C),$(ROOT_EXE) -q -l $(c) &&) true

.PHONY: read
read:
	@$(foreach c,$(READ_C),$(ROOT_EXE) -q -l $(c) &&) true
