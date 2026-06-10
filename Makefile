ROOT_EXE ?= $(shell which root.exe)
ifeq ($(ROOT_EXE),)
$(error Could not find root.exe)
endif

define move_files
    mkdir -p $(1)_files && \
    find . -name "*.$(1)" -not -path "./$(1)_files/*" -exec mv {} "$(1)_files" \;
endef

ver :=
DICT_DIR := $(shell pwd)/dict/$(ver)/
export DICT_DIR

.PHONY: all
all:
	$(MAKE) dict
	$(MAKE) write
	$(MAKE) read
	$(MAKE) store

# This assumes there is no whitespace in any of the paths...
DICT_MAKEFILE_DIR := $(sort $(shell find */ -name Makefile -printf "%h\n"))
WRITE_C := $(sort $(shell find . -name write.C))
READ_C := $(sort $(shell find . -name read.C))

.PHONY: dict
dict:: $(DICT_MAKEFILE_DIR)
$(DICT_MAKEFILE_DIR):: $(DICT_DIR)
	@$(MAKE) -C $@
$(DICT_DIR)::
	@mkdir -p $@
	$(info Storing dictionaries in: '$@')

.PHONY: write
write:: $(WRITE_C)
$(WRITE_C)::
	@LD_LIBRARY_PATH="$${LD_LIBRARY_PATH:+$$LD_LIBRARY_PATH:}$(or $(DICT_DIR),$(shell dirname $@))" $(ROOT_EXE) -q -l $@

.PHONY: read
read:: $(READ_C)
$(READ_C)::
	@LD_LIBRARY_PATH="$${LD_LIBRARY_PATH:+$$LD_LIBRARY_PATH:}$(or $(DICT_DIR),$(shell dirname $@))" $(ROOT_EXE) -q -l $@

.PHONY: store
store:
	@$(call move_files,root)
	@$(call move_files,json)
