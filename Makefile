ROOT_EXE ?= $(shell which root.exe)

# directory arguments can be empty -> no sudirectories will be created, everything will be stored directly in those folders
DICT_DIR := $(shell pwd)/dict/$(if $(dict_dir),$(dict_dir)/)
WRITE_DIR := $(shell pwd)/write/$(if $(write_dir),$(write_dir)/)
READ_DIR := $(shell pwd)/read/$(if $(write_dir),$(write_dir)/)$(if $(read_dir),$(read_dir)/)
export DICT_DIR

.PHONY: all
all: check
	$(MAKE) dict
	$(MAKE) write
	$(MAKE) read

# This assumes there is no whitespace in any of the paths...
DICT_MAKEFILE_DIR := $(sort $(shell find */ -name Makefile -printf "%h\n"))
WRITE_C := $(sort $(shell find . -name write.C))
READ_C := $(sort $(shell find . -name read.C))

# run each Makefile in subdirectories to create dictionaries
.PHONY: dict
dict:: check $(DICT_MAKEFILE_DIR)
$(DICT_MAKEFILE_DIR):: $(DICT_DIR)
	@$(MAKE) -C $@
$(DICT_DIR)::
	@echo -e "\nStarting dict target - Storing dictionaries in: '$@'" && mkdir -p $@

# run each write.C file in subdirectories to create .root files
.PHONY: write
write:: check $(WRITE_C)
$(WRITE_C):: $(WRITE_DIR)
	@LD_LIBRARY_PATH="$${LD_LIBRARY_PATH:+$$LD_LIBRARY_PATH:}$(DICT_DIR)" $(ROOT_EXE) -q -l '$@("$(WRITE_DIR)$(subst /,.,$(shell dirname $@)).root")'
$(WRITE_DIR)::
	@echo -e "\nStarting write target - Storing root files in: '$@'" && mkdir -p $@

# run each read.C file in subdirectories to create .json files
.PHONY: read
read:: check $(READ_C)
$(READ_C):: $(READ_DIR)
	@LD_LIBRARY_PATH="$${LD_LIBRARY_PATH:+$$LD_LIBRARY_PATH:}$(DICT_DIR)" $(ROOT_EXE) -q -l \
	'$@("$(WRITE_DIR)/$(subst /,.,$(shell dirname $@)).root", "$(READ_DIR)$(subst /,.,$(shell dirname $@)).json")'
$(READ_DIR)::
	@echo -e "\nStarting read target - Storing json files in: '$@'" && mkdir -p $@

.PHONY: check
check::
	@test -x "$(ROOT_EXE)" || { echo "Could not find root.exe"; exit 1; }

.PHONY: validate
validate:
	@if [ "$(source_scripts)" = "" ]; then\
        echo "Source scripts missing"; exit 1;\
    fi

#	run make dict to create dictionaries
	@echo "-- Creating dictionaries --"
	@for s_path in $(source_scripts); do \
		(bash -c "source $$s_path && ROOT_VERSION=\$$(root-config --version) && $(MAKE) dict dict_dir=\$$ROOT_VERSION") || exit $$?; \
	done

#	run make write to create binaries
	@echo "-- Creating binaries --"
	@for s_path in $(source_scripts); do \
		(bash -c "source $$s_path && ROOT_VERSION=\$$(root-config --version) && $(MAKE) write dict_dir=\$$ROOT_VERSION write_dir=\$$ROOT_VERSION") || exit $$?; \
	done
	
#	run make read to do cross-validation
	@echo "-- Creating .json files --"
	@for s_path in $(source_scripts); do \
		for w_dir in write/*; do \
			(bash -c "source $$s_path && ROOT_VERSION=\$$(root-config --version) && $(MAKE) read dict_dir=\$$ROOT_VERSION write_dir=$$(basename $$w_dir) read_dir=\$$ROOT_VERSION") || exit $$?; \
		done; \
	done
	@echo Finished
