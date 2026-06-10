ROOT_EXE ?= $(shell which root.exe)
ifeq ($(ROOT_EXE),)
$(error Could not find root.exe)
endif

# directory arguments can be empty -> no sudirectories will be created, everything will be stored directly in those folders
DICT_DIR := $(shell pwd)/dict/$(if $(dict_dir),$(dict_dir)/)
WRITE_DIR := $(shell pwd)/write/$(if $(write_dir),$(write_dir)/)
READ_DIR := $(shell pwd)/read/$(if $(write_dir),$(write_dir)/)$(if $(read_dir),$(read_dir)/)
export DICT_DIR

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
$(DICT_MAKEFILE_DIR):: $(DICT_DIR)
	@$(MAKE) -C $@
$(DICT_DIR)::
	@mkdir -p $@
	$(info Storing dictionaries in: '$@')

.PHONY: write
write:: $(WRITE_C)
$(WRITE_C):: $(WRITE_DIR)
	@LD_LIBRARY_PATH="$${LD_LIBRARY_PATH:+$$LD_LIBRARY_PATH:}$(DICT_DIR)" $(ROOT_EXE) -q -l '$@("$(WRITE_DIR)$(subst /,.,$(shell dirname $@)).root")'
$(WRITE_DIR)::
	@mkdir -p $@
	$(info Storing root files in: '$@')

.PHONY: read
read:: $(READ_C)
$(READ_C):: $(READ_DIR)
	@LD_LIBRARY_PATH="$${LD_LIBRARY_PATH:+$$LD_LIBRARY_PATH:}$(DICT_DIR)" $(ROOT_EXE) -q -l \
	'$@("$(WRITE_DIR)/$(subst /,.,$(shell dirname $@)).root", "$(READ_DIR)$(subst /,.,$(shell dirname $@)).json")'
$(READ_DIR)::
	@mkdir -p $@
	$(info Storing root files in: '$@')
