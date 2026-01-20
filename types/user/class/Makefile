CXX=g++
CXXFLAGS_ROOT=$(shell root-config --cflags)
ifeq ($(CXXFLAGS_ROOT),)
  $(error cannot find root-config: make sure to source thisroot.sh)
endif
CXXFLAGS=-Wall $(CXXFLAGS_ROOT)
LDFLAGS=$(shell root-config --libs)

.PHONY: all clean

all: UserClass.cxx libUserClass.so

UserClass.cxx: UserClass.hxx LinkDef.h
	rootcling -f $@ $^

libUserClass.so: UserClass.cxx
	$(CXX) -shared -fPIC -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f UserClass.cxx UserClass_rdict.pcm libUserClass.so
