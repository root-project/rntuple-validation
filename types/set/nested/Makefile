CXX=g++
CXXFLAGS_ROOT=$(shell root-config --cflags)
ifeq ($(CXXFLAGS_ROOT),)
  $(error cannot find root-config: make sure to source thisroot.sh)
endif
CXXFLAGS=-Wall $(CXXFLAGS_ROOT)
LDFLAGS=$(shell root-config --libs)

.PHONY: all clean

all: NestedSet.cxx libNestedSet.so

NestedSet.cxx: NestedSet.hxx LinkDef.h
	rootcling -f $@ $^

libNestedSet.so: NestedSet.cxx
	$(CXX) -shared -fPIC -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f NestedSet.cxx NestedSet_rdict.pcm libNestedSet.so
