ROOT_EXE ?= $(shell which root.exe)
PYTHON3 ?= $(shell which python3)
ASSET_VERSION := 1.0.0
ROOT_ASSET := RNTuple-ROOT-v$(ASSET_VERSION).zip 
JSON_ASSET := Validation-JSON-v$(ASSET_VERSION).zip

# directory arguments can be empty -> no sudirectories will be created, everything will be stored directly in those folders
DICT_DIR := $(shell pwd)/dict/$(if $(dict_dir),$(dict_dir)/)
WRITE_DIR := $(shell pwd)/write/$(if $(write_dir),$(write_dir)/)
READ_DIR := $(shell pwd)/read/$(if $(write_dir),$(write_dir)/)$(if $(read_dir),$(read_dir)/)
READ_DIR_JSROOT := $(shell pwd)/read/$(if $(write_dir),$(write_dir)/)jsroot/
export DICT_DIR

.PHONY: all
all: check
	$(MAKE) dict
	$(MAKE) write
	$(MAKE) read

# This assumes there is no whitespace in any of the paths...
DICT_MAKEFILE_DIR := $(sort $(shell find */ -name Makefile -not -path "node_modules/*" -printf "%h\n"))
WRITE_C := $(sort $(shell find . -name write.C))
READ_C := $(sort $(shell find . -name read.C))
READ_MJS := $(sort $(shell find . -name read.mjs))

# run each Makefile in subdirectories to create dictionaries
.PHONY: dict $(DICT_MAKEFILE_DIR) $(DICT_DIR)
dict: check $(DICT_MAKEFILE_DIR)
$(DICT_MAKEFILE_DIR): $(DICT_DIR)
	@$(MAKE) -C $@
$(DICT_DIR):
	$(info ###### Starting dict target - Storing dictionaries in: '$@' ######)
	@mkdir -p $@

# run each write.C file in subdirectories to create .root files
.PHONY: write $(WRITE_C) $(WRITE_DIR)
write: check $(WRITE_C)
$(WRITE_C): $(WRITE_DIR)
	@LD_LIBRARY_PATH="$${LD_LIBRARY_PATH:+$$LD_LIBRARY_PATH:}$(DICT_DIR)" $(ROOT_EXE) -q -l '$@("$(WRITE_DIR)$(subst /,.,$(shell dirname $@)).root")'
$(WRITE_DIR):
	$(info ###### Starting write target - Storing root files in: '$@' ######)
	@mkdir -p $@

# run each read.C file in subdirectories to create .json files
.PHONY: read $(READ_C) $(READ_DIR)
read: check $(READ_C)
$(READ_C): $(READ_DIR)
	@LD_LIBRARY_PATH="$${LD_LIBRARY_PATH:+$$LD_LIBRARY_PATH:}$(DICT_DIR)" $(ROOT_EXE) -q -l \
	'$@("$(WRITE_DIR)$(subst /,.,$(shell dirname $@)).root", "$(READ_DIR)$(subst /,.,$(shell dirname $@)).json")'
$(READ_DIR):
	$(info ###### Starting read target - Storing json files in: '$@' ######)
	@mkdir -p $@

.PHONY: read_jsroot $(READ_MJS) $(READ_DIR_JSROOT)
read_jsroot: $(READ_MJS)
$(READ_MJS): $(READ_DIR_JSROOT)
	@echo Processing $@
	@node $@ $(WRITE_DIR)$(subst jsroot.,,$(subst /,.,$(shell dirname $@)).root) $(READ_DIR_JSROOT)$(subst jsroot.,,$(subst /,.,$(shell dirname $@))).json
$(READ_DIR_JSROOT):
	$(info  ###### Storing json files created by JSROOT in: '$@' ######)
	@mkdir -p $@

.PHONY: check
check:
	@test -x "$(ROOT_EXE)" || { echo "Could not find root.exe"; exit 1; }

.PHONY: validate
validate:
	@if [ "$(source_scripts)" = "" ]; then\
        echo "Source scripts missing"; exit 1;\
    fi

#	run make dict to create dictionaries
	@for s_path in $(source_scripts); do \
		(bash -c "source $$s_path && ROOT_VERSION=\$$(root-config --version) && $(MAKE) dict dict_dir=\$$ROOT_VERSION") || exit $$?; \
	done

#	run make write to create binaries
	@for s_path in $(source_scripts); do \
		(bash -c "source $$s_path && ROOT_VERSION=\$$(root-config --version) && $(MAKE) write dict_dir=\$$ROOT_VERSION write_dir=\$$ROOT_VERSION") || exit $$?; \
	done
	
#	run make read to do cross-validation
	@for s_path in $(source_scripts); do \
		bash -c "source $$s_path && ROOT_VERSION=\$$(root-config --version) && \
			for w_dir in write/*; do \
				$(MAKE) read dict_dir=\$$ROOT_VERSION write_dir=\$$(basename \$$w_dir) read_dir=\$$ROOT_VERSION || exit \$$?; \
			done" || exit $$?; \
	done

.PHONY: download
download: 
	$(info Downloading and unpacking assets from GitHub..)
	@wget -q -N -P ./assets https://github.com/root-project/rntuple-validation/releases/download/v$(ASSET_VERSION)/$(ROOT_ASSET)
	@mkdir -p write && unzip -n -q -d write/$(basename $(ROOT_ASSET)) assets/$(ROOT_ASSET)
	@wget -q -N -P ./assets https://github.com/root-project/rntuple-validation/releases/download/v$(ASSET_VERSION)/$(JSON_ASSET)
	@mkdir -p read/$(basename $(ROOT_ASSET)) && unzip -n -q -d read/$(basename $(ROOT_ASSET))/$(basename $(JSON_ASSET)) assets/$(JSON_ASSET)

.PHONY: export_html
export_html:
	$(PYTHON3) scripts/cross_validation.py
