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

all: libNestedUnorderedMultimap.so

NestedUnorderedMultimap.cxx: NestedUnorderedMultimap.hxx LinkDef.h
	$(ROOTCLING) -f $@ $^

libNestedUnorderedMultimap.so: NestedUnorderedMultimap.cxx
	$(CXX) -shared -fPIC -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

clean:
	$(RM) NestedUnorderedMultimap.cxx NestedUnorderedMultimap_rdict.pcm libNestedUnorderedMultimap.so
