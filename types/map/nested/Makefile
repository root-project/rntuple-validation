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

all: $(DICT_DIR)libNestedMap.so

$(DICT_DIR)NestedMap.cxx: NestedMap.hxx LinkDef.h
	$(ROOTCLING) -f $@ $^ 

$(DICT_DIR)libNestedMap.so: $(DICT_DIR)NestedMap.cxx
	$(CXX) -shared -fPIC -o $@ $^ $(CXXFLAGS) $(LDFLAGS) -I $$(pwd)

clean:
	$(RM) NestedMap.cxx NestedMap_rdict.pcm libNestedMap.so
