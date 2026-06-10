ROOT_CONFIG ?= $(shell which root-config)
ifeq ($(ROOT_CONFIG),)
$(error Could not find root-config)
endif
ROOTCLING ?= $(shell which rootcling)
ifeq ($(ROOTCLING),)
$(error Could not find rootcling)
endif

CXX := $(shell $(ROOT_CONFIG) --cxx)
CXXFLAGS_ROOT := $(shell $(ROOT_CONFIG) --cflags)
CXXFLAGS := -Wall $(CXXFLAGS_ROOT)
LDFLAGS := $(shell $(ROOT_CONFIG) --libs)

.PHONY: all clean

all: $(DICT_DIR)libNestedUnorderedMultiset.so

$(DICT_DIR)NestedUnorderedMultiset.cxx: NestedUnorderedMultiset.hxx LinkDef.h
	$(ROOTCLING) -f $@ $^

$(DICT_DIR)libNestedUnorderedMultiset.so: $(DICT_DIR)NestedUnorderedMultiset.cxx
	$(CXX) -shared -fPIC -o $@ $^ $(CXXFLAGS) $(LDFLAGS) -I $$(pwd)

clean:
	$(RM) NestedUnorderedMultiset.cxx NestedUnorderedMultiset_rdict.pcm libNestedUnorderedMultiset.so
