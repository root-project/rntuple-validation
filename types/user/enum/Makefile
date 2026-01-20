CXX=g++
CXXFLAGS_ROOT=$(shell root-config --cflags)
ifeq ($(CXXFLAGS_ROOT),)
  $(error cannot find root-config: make sure to source thisroot.sh)
endif
CXXFLAGS=-Wall $(CXXFLAGS_ROOT)
LDFLAGS=$(shell root-config --libs)

.PHONY: all clean

all: UserEnum.cxx libUserEnum.so

UserEnum.cxx: UserEnum.hxx LinkDef.h
	rootcling -f $@ $^

libUserEnum.so: UserEnum.cxx
	$(CXX) -shared -fPIC -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f UserEnum.cxx UserEnum_rdict.pcm libUserEnum.so
