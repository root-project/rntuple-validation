CXX=g++
CXXFLAGS_ROOT=$(shell root-config --cflags)
ifeq ($(CXXFLAGS_ROOT),)
  $(error cannot find root-config: make sure to source thisroot.sh)
endif
CXXFLAGS=-Wall $(CXXFLAGS_ROOT)
LDFLAGS=$(shell root-config --libs)

.PHONY: all clean

all: NestedMultiset.cxx libNestedMultiset.so

NestedMultiset.cxx: NestedMultiset.hxx LinkDef.h
	rootcling -f $@ $^

libNestedMultiset.so: NestedMultiset.cxx
	$(CXX) -shared -fPIC -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f NestedMultiset.cxx NestedMultiset_rdict.pcm libNestedMultiset.so
