CXX=g++
CXXFLAGS_ROOT=$(shell root-config --cflags)
ifeq ($(CXXFLAGS_ROOT),)
  $(error cannot find root-config: make sure to source thisroot.sh)
endif
CXXFLAGS=-Wall $(CXXFLAGS_ROOT)
LDFLAGS=$(shell root-config --libs)

.PHONY: all clean

all: NestedUnorderedMultiset.cxx libNestedUnorderedMultiset.so

NestedUnorderedMultiset.cxx: NestedUnorderedMultiset.hxx LinkDef.h
	rootcling -f $@ $^

libNestedUnorderedMultiset.so: NestedUnorderedMultiset.cxx
	$(CXX) -shared -fPIC -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f NestedUnorderedMultiset.cxx NestedUnorderedMultiset_rdict.pcm libNestedUnorderedMultiset.so
