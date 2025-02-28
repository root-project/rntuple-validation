CXX=g++
CXXFLAGS_ROOT=$(shell root-config --cflags)
ifeq ($(CXXFLAGS_ROOT),)
  $(error cannot find root-config: make sure to source thisroot.sh)
endif
CXXFLAGS=-Wall $(CXXFLAGS_ROOT)
LDFLAGS=$(shell root-config --libs)

.PHONY: all clean

all: NestedUnorderedSet.cxx libNestedUnorderedSet.so

NestedUnorderedSet.cxx: NestedUnorderedSet.hxx LinkDef.h
	rootcling -f $@ $^

libNestedUnorderedSet.so: NestedUnorderedSet.cxx
	$(CXX) -shared -fPIC -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f NestedUnorderedSet.cxx NestedUnorderedSet_rdict.pcm libNestedUnorderedSet.so
