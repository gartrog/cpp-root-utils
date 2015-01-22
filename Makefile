
SDIR=src
ODIR=obj
LDIR=lib
BINDIR=bin

SOLIBNAME  := Utils
SOLIB = $(LDIR)/lib$(SOLIBNAME).so

IDIR = -Iinclude
LIBS=-L./$(LDIR)
CXX = g++
LD  = g++
CFLAGSCINT  = -O2 -Wall -Wextra -Wno-unused-parameter $(IDIR) -fPIC
CFLAGS  = $(CFLAGSCINT) -g -c -std=c++11
LDFLAGS = -shared -fPIC -Wl,-soname,lib$(SOLIBNAME).so $(shell root-config --ldflags)

ROOTCFLAGS=$(shell root-config --cflags)
ROOTLIBS=$(shell root-config --libs)

lib_src     = $(wildcard $(SDIR)/*.cxx)
lib_obj_tmp = $(patsubst %.cxx, %.o, $(lib_src))
lib_obj     = $(patsubst $(SDIR)/%, $(ODIR)/%, $(lib_obj_tmp))

all : dir bin so
so : $(LDIR) $(DEPFILES) $(SOLIB)
dir : $(ODIR) $(LDIR)
bin:

$(ODIR)/%.o: $(SDIR)/%.cxx
	$(CXX) -o $@ $< $(ROOTCFLAGS) $(CFLAGS)

$(ODIR):
	  mkdir -p $(ODIR)

$(LDIR):
	  mkdir -p $(LDIR)

$(BINDIR):
	  mkdir -p $(BINDIR)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(ODIR)/*.d $(ODIR)/*Dict* core* $(LDIR)/*.so *_C.so $(BINDIR)/*

$(SOLIB): $(lib_obj)
	$(LD) $(LDFLAGS) $^ $(ROOTLIBS) -o $@

